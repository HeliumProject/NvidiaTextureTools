// Copyright (c) 2009-2011 Ignacio Castano <castano@gmail.com>
// Copyright (c) 2007-2009 NVIDIA Corporation -- Ignacio Castano <icastano@nvidia.com>
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

#ifndef NVTT_COMPRESSORDX9_H
#define NVTT_COMPRESSORDX9_H

#include "CompressorDXT.h"

namespace nv
{
    struct ColorBlock;

    // Fast CPU compressors.
    struct FastCompressorDXT1 : public FixedBlockCompressor
    {
        virtual void compressBlock(ColorBlock & rgba, nvtt::AlphaMode alphaMode, const nvtt::CompressionOptions::Private & compressionOptions, void * output);
        virtual uint blockSize() const { return 8; }
    };

    struct FastCompressorDXT1a : public FixedBlockCompressor
    {
        virtual void compressBlock(ColorBlock & rgba, nvtt::AlphaMode alphaMode, const nvtt::CompressionOptions::Private & compressionOptions, void * output);
        virtual uint blockSize() const { return 8; }
    };

    struct FastCompressorDXT3 : public FixedBlockCompressor
    {
        virtual void compressBlock(ColorBlock & rgba, nvtt::AlphaMode alphaMode, const nvtt::CompressionOptions::Private & compressionOptions, void * output);
        virtual uint blockSize() const { return 16; }
    };

    struct FastCompressorDXT5 : public FixedBlockCompressor
    {
        virtual void compressBlock(ColorBlock & rgba, nvtt::AlphaMode alphaMode, const nvtt::CompressionOptions::Private & compressionOptions, void * output);
        virtual uint blockSize() const { return 16; }
    };

    struct FastCompressorDXT5n : public FixedBlockCompressor
    {
        virtual void compressBlock(ColorBlock & rgba, nvtt::AlphaMode alphaMode, const nvtt::CompressionOptions::Private & compressionOptions, void * output);
        virtual uint blockSize() const { return 16; }
    };


    // Normal CPU compressors.
#if 0
    struct CompressorDXT1 : public ColorSetCompressor
    {
        virtual void compressBlock(ColorSet & set, nvtt::AlphaMode alphaMode, const nvtt::CompressionOptions::Private & compressionOptions, void * output);
        virtual uint blockSize() const { return 8; }
    };
#else
    struct CompressorDXT1 : public FixedBlockCompressor
    {
        virtual void compressBlock(ColorBlock & rgba, nvtt::AlphaMode alphaMode, const nvtt::CompressionOptions::Private & compressionOptions, void * output);
        virtual uint blockSize() const { return 8; }
    };
#endif

    struct CompressorDXT1a : public FixedBlockCompressor
    {
        virtual void compressBlock(ColorBlock & rgba, nvtt::AlphaMode alphaMode, const nvtt::CompressionOptions::Private & compressionOptions, void * output);
        virtual uint blockSize() const { return 8; }
    };

    struct CompressorDXT1_Luma : public FixedBlockCompressor
    {
        virtual void compressBlock(ColorBlock & rgba, nvtt::AlphaMode alphaMode, const nvtt::CompressionOptions::Private & compressionOptions, void * output);
        virtual uint blockSize() const { return 8; }
    };

    struct CompressorDXT3 : public FixedBlockCompressor
    {
        virtual void compressBlock(ColorBlock & rgba, nvtt::AlphaMode alphaMode, const nvtt::CompressionOptions::Private & compressionOptions, void * output);
        virtual uint blockSize() const { return 16; }
    };

    struct CompressorDXT5 : public FixedBlockCompressor
    {
        virtual void compressBlock(ColorBlock & rgba, nvtt::AlphaMode alphaMode, const nvtt::CompressionOptions::Private & compressionOptions, void * output);
        virtual uint blockSize() const { return 16; }
    };

    struct CompressorDXT5n : public FixedBlockCompressor
    {
        virtual void compressBlock(ColorBlock & rgba, nvtt::AlphaMode alphaMode, const nvtt::CompressionOptions::Private & compressionOptions, void * output);
        virtual uint blockSize() const { return 16; }
    };


    // External compressors.
#if defined(HAVE_ATITC)
    struct AtiCompressorDXT1 : public CompressorInterface
    {
        virtual void compress(nvtt::InputFormat inputFormat, nvtt::AlphaMode alphaMode, uint w, uint h, uint d, void * data, const nvtt::CompressionOptions::Private & compressionOptions, const nvtt::OutputOptions::Private & outputOptions);
    };

    struct AtiCompressorDXT5 : public CompressorInterface
    {
        virtual void compress(nvtt::InputFormat inputFormat, nvtt::AlphaMode alphaMode, uint w, uint h, uint d, void * data, const nvtt::CompressionOptions::Private & compressionOptions, const nvtt::OutputOptions::Private & outputOptions);
    };
#endif

#if defined(HAVE_SQUISH)
    struct SquishCompressorDXT1 : public CompressorInterface
    {
        virtual void compress(nvtt::InputFormat inputFormat, nvtt::AlphaMode alphaMode, uint w, uint h, uint d, void * data, const nvtt::CompressionOptions::Private & compressionOptions, const nvtt::OutputOptions::Private & outputOptions);
    };
#endif

#if defined(HAVE_D3DX)
    struct D3DXCompressorDXT1 : public CompressorInterface
    {
        virtual void compress(nvtt::InputFormat inputFormat, nvtt::AlphaMode alphaMode, uint w, uint h, uint d, void * data, const nvtt::CompressionOptions::Private & compressionOptions, const nvtt::OutputOptions::Private & outputOptions);
    };
#endif

#if defined(HAVE_STB)
    struct StbCompressorDXT1 : public FixedBlockCompressor
    {
        virtual void compressBlock(ColorBlock & rgba, nvtt::AlphaMode alphaMode, const nvtt::CompressionOptions::Private & compressionOptions, void * output);
        virtual uint blockSize() const { return 8; }
    };
#endif

} // nv namespace


#endif // NVTT_COMPRESSORDX9_H
