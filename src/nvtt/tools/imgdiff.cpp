// Copyright NVIDIA Corporation 2007 -- Ignacio Castano <icastano@nvidia.com>
// 
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

#include "cmdline.h"

#include "nvmath/Color.h"
#include "nvmath/Vector.inl"

#include "nvimage/Image.h"
#include "nvimage/DirectDrawSurface.h"

#include "nvcore/StrLib.h"
#include "nvcore/StdStream.h"

#include <math.h>


static bool loadImage(nv::Image & image, const char * fileName)
{
	if (nv::strCaseCmp(nv::Path::extension(fileName), ".dds") == 0)
	{
		nv::DirectDrawSurface dds(fileName);
		if (!dds.isValid())
		{
			printf("The file '%s' is not a valid DDS file.\n", fileName);
			return false;
		}
		
		dds.mipmap(&image, 0, 0); // get first image
	}
	else
	{
		// Regular image.
		if (!image.load(fileName))
		{
			printf("The file '%s' is not a supported image type.\n", fileName);
			return false;
		}
	}

	return true;
}

struct Error
{
	Error()
	{
		samples = 0;
		mabse = 0.0f;
		maxabse = 0.0f;
		mse = 0.0f;
	}

    // @@ This has poor precision...
	void addSample(double e)
	{
		samples++;
		mabse += fabs(e);
		maxabse = nv::max(maxabse, fabs(e));
		mse += e * e;
	}

	void done()
	{
		mabse /= samples;
		mse /= samples;
		rmse = sqrt(mse);
		psnr = (rmse == 0) ? 999.0 : 20.0 * log10(255.0 / rmse);
	}

	void print()
	{
		printf("  Mean absolute error: %f\n", mabse);
		printf("  Max absolute error: %f\n", maxabse);
        printf("  Mean squared error: %f\n", mse);
		printf("  Root mean squared error: %f\n", rmse);
		printf("  Peak signal to noise ratio in dB: %f\n", psnr);
	}

	int samples;
	double mabse;
	double maxabse;
	double mse;
	double rmse;
	double psnr;
};

struct NormalError
{
	NormalError()
	{
		samples = 0;
		ade = 0.0f;
		mse = 0.0f;
	}

	void addSample(nv::Color32 o, nv::Color32 c)
	{
		nv::Vector3 vo = nv::Vector3(o.r, o.g, o.b);
		nv::Vector3 vc = nv::Vector3(c.r, c.g, c.b);

		// Unpack and normalize.
		vo = nv::normalize(2.0f * (vo / 255.0f) - 1.0f);
		vc = nv::normalize(2.0f * (vc / 255.0f) - 1.0f);

		ade += acosf(nv::clamp(dot(vo, vc), -1.0f, 1.0f));
		mse += lengthSquared((vo - vc) * (255 / 2.0f));
		
		samples++;
	}

	void done()
	{
		if (samples)
		{
			ade /= samples;
			mse /= samples * 3;
			rmse = sqrtf(mse);
			psnr = (rmse == 0) ? 999.0f : 20.0f * log10(255.0f / rmse);
		}
	}

	void print()
	{
		printf("  Angular deviation error: %f\n", ade);
		printf("  Root mean squared error: %f\n", rmse);
		printf("  Peak signal to noise ratio in dB: %f\n", psnr);
	}

	int samples;
	float ade;
	float mse;
	float rmse;
	float psnr;
};

static float luma(const nv::Color32 & c) {
    return 0.299f * float(c.r) + 0.587f * float(c.g) + 0.114f * float(c.b);
    //return 0.25f * float(c.r) + 0.5f * float(c.g) + 0.25f * float(c.b);
    //return 0.333f * float(c.r) + 0.334f * float(c.g) + 0.333f * float(c.b);
    //return 0.1f * float(c.r) + 0.8f * float(c.g) + 0.1f * float(c.g);
}


int main(int argc, char *argv[])
{
	MyAssertHandler assertHandler;
	MyMessageHandler messageHandler;

	bool compareNormal = false;
	bool compareAlpha = false;

	nv::Path input0;
	nv::Path input1;
	nv::Path output;

	// Parse arguments.
	for (int i = 1; i < argc; i++)
	{
		// Input options.
		if (strcmp("-normal", argv[i]) == 0)
		{
			compareNormal = true;
		}
		if (strcmp("-alpha", argv[i]) == 0)
		{
			compareAlpha = true;
		}

		else if (argv[i][0] != '-')
		{
			input0 = argv[i];

			if (i+1 < argc && argv[i+1][0] != '-') {
				input1 = argv[i+1];
			}

			break;
		}
	}

	if (input0.isNull() || input1.isNull())
	{
		printf("NVIDIA Texture Tools - Copyright NVIDIA Corporation 2007\n\n");
		
		printf("usage: nvimgdiff [options] original_file updated_file [output]\n\n");
		
		printf("Diff options:\n");
		printf("  -normal \tCompare images as if they were normal maps.\n");
		printf("  -alpha  \tCompare alpha weighted images.\n");

		return 1;
	}

	nv::Image image0, image1;
	if (!loadImage(image0, input0.str())) return 0;
	if (!loadImage(image1, input1.str())) return 0;

	const uint w0 = image0.width();
	const uint h0 = image0.height();
	const uint w1 = image1.width();
	const uint h1 = image1.height();
	const uint w = nv::min(w0, w1);
	const uint h = nv::min(h0, h1);

	// Compute errors.
	Error error_r;
	Error error_g;
	Error error_b;
	Error error_a;
    Error error_luma;
	Error error_total;
	NormalError error_normal;

	for (uint i = 0; i < h; i++)
	{
		for (uint e = 0; e < w; e++)
		{
			const nv::Color32 c0(image0.pixel(e, i));
			const nv::Color32 c1(image1.pixel(e, i));

			double r = float(c0.r - c1.r);
			double g = float(c0.g - c1.g);
			double b = float(c0.b - c1.b);
			double a = float(c0.a - c1.a);
            
			error_r.addSample(r);
			error_g.addSample(g);
			error_b.addSample(b);
			error_a.addSample(a);

            double l0 = luma(c0);
            double l1 = luma(c1);

            error_luma.addSample(l0 - l1);

            double d = sqrt(r*r + g*g + b*b);

			if (compareAlpha) {
                d *= c0.a / 255.0;
			}

            error_total.addSample(d);

			if (compareNormal) {
				error_normal.addSample(c0, c1);
			}
		}
	}

	error_r.done();
	error_g.done();
	error_b.done();
	error_a.done();
    error_luma.done();
	error_total.done();
	error_normal.done();
	

	printf("Image size compared: %dx%d\n", w, h);
	if (w != w0 || w != w1 || h != h0 || h != h1) {
		printf("--- NOTE: only the overlap between the 2 images (%d,%d) and (%d,%d) was compared\n", w0, h0, w1, h1);
	}
	printf("Total pixels: %d\n", w*h);

	printf("Color:\n");
	error_total.print();

	printf("Luma:\n");
	error_luma.print();

	if (compareNormal)
	{
		printf("Normal:\n");
		error_normal.print();
	}

	if (compareAlpha)
	{
		printf("Alpha:\n");
		error_a.print();
	}

	// @@ Write image difference.
	
	return 0;
}

