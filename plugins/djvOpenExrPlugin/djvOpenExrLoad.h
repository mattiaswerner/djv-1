//------------------------------------------------------------------------------
// Copyright (c) 2004-2015 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

//! \file djvOpenExrLoad.h

#ifndef DJV_OPENEXR_LOAD_H
#define DJV_OPENEXR_LOAD_H

#include <djvOpenExr.h>

#include <djvFileInfo.h>
#include <djvImageIo.h>

#include <ImfInputFile.h>

//! \addtogroup djvOpenExrPlugin
//@{

//------------------------------------------------------------------------------
//! \class djvOpenExrLoad
//!
//! This class provides an OpenEXR loader.
//!
//! \todo Add support for luminance/chroma images.
//------------------------------------------------------------------------------

class djvOpenExrLoad : public djvImageLoad
{
public:

    //! Constructor.

    djvOpenExrLoad(const djvOpenExr::Options &, djvImageContext *);

    //! Destructor.

    virtual ~djvOpenExrLoad();

    virtual void open(const djvFileInfo &, djvImageIoInfo &)
        throw (djvError);

    virtual void read(djvImage &, const djvImageIoFrameInfo &)
        throw (djvError);

    virtual void close() throw (djvError);
    
private:

    void _open(const QString &, djvImageIoInfo &) throw (djvError);

    djvOpenExr::Options        _options;
    djvFileInfo                _file;
    Imf::InputFile *           _f;
    djvBox2i                   _displayWindow;
    djvBox2i                   _dataWindow;
    djvBox2i                   _intersectedWindow;
    QVector<djvOpenExr::Layer> _layers;
    djvPixelData               _tmp;
    bool                       _fast;
};

//@} // djvOpenExrPlugin

#endif // DJV_OPENEXR_LOAD_H

