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

//! \file djvGlslTestApplication.cpp

#include <djvGlslTestApplication.h>

#include <QTimer>

djvGlslTestApplication::djvGlslTestApplication(int & argc, char ** argv) :
    QApplication(argc, argv)
{
    //DJV_DEBUG("djvGlslTestApplication::djvGlslTestApplication");
    
#   if QT_VERSION < 0x050000
    setStyle(new QPlastiqueStyle);
#   else
    setStyle("fusion");
#   endif

    // Create the context.
    
    _context.reset(new djvGlslTestContext);
    
    // Parse the command line.
    
    if (! _context->commandLine(argc, argv))
    {
        QTimer::singleShot(0, this, SLOT(commandLineExit()));
    }
    else
    {
        QTimer::singleShot(0, this, SLOT(work()));
    }
}

void djvGlslTestApplication::commandLineExit()
{
    exit(1);
}

void djvGlslTestApplication::work()
{
    //DJV_DEBUG("djvGlslTestApplication::work");
    
    _imageLoad.reset(new djvGlslTestImageLoad(_context.data()));
    
    try
    {
        djvFileInfo fileInfo = _context->fileInfo();
        
        if (fileInfo.isSequenceValid())
        {
            fileInfo.setType(djvFileInfo::SEQUENCE);
        }
        
        _imageLoad->load(fileInfo);
    }
    catch (const djvError & error)
    {
        _context->printError(error);
        
        exit(1);
        
        return;
    }

    _context->setValid(true);
    
    _opsManager.reset(new djvGlslTestOpsManager(_context.data()));
    
    _playback.reset(new djvGlslTestPlayback);
    _playback->setSequence(_imageLoad->info().sequence);
    
    _mainWindow.reset(new djvGlslTestMainWindow(
        _imageLoad.data(),
        _opsManager.data(),
        _playback.data(),
        _context.data()));
    
    //_playback->setPlayback(djvGlslTestPlayback::FORWARD);
    
    _mainWindow->show();
}

int main(int argc, char ** argv)
{
    return djvGlslTestApplication(argc, argv).exec();
}

