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

//! \file djvGlslTestOpsManager.h

#ifndef DJV_GLSL_TEST_OPS_MANAGER_H
#define DJV_GLSL_TEST_OPS_MANAGER_H

#include <djvSpeed.h>

#include <QObject>

class djvGlslTestAbstractOp;
class djvGlslTestAbstractOpFactory;
class djvGlslTestContext;

//------------------------------------------------------------------------------
// djvGlslTestOpsManager
//------------------------------------------------------------------------------

class djvGlslTestOpsManager : public QObject
{
    Q_OBJECT
    
public:

    explicit djvGlslTestOpsManager(djvGlslTestContext *, QObject * parent = 0);
    
    virtual ~djvGlslTestOpsManager();

    const QList<djvGlslTestAbstractOpFactory *> factories() const;

    const QList<djvGlslTestAbstractOp *> ops() const;

    int currentIndex() const;
    
    djvGlslTestAbstractOpFactory * currentFactory() const;
    
    djvGlslTestAbstractOp * currentOp() const;
    
public Q_SLOTS:

    void setCurrentIndex(int);
    
Q_SIGNALS:

    void currentIndexChanged(int);

    void opChanged();
    
private:

    QList<djvGlslTestAbstractOpFactory *> _factories;
    QList<djvGlslTestAbstractOp *>        _ops;
    int                                   _currentIndex;
};

#endif // DJV_GLSL_TEST_OPS_MANAGER_H
