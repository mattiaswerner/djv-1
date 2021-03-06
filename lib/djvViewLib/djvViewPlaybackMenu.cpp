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

//! \file djvViewPlaybackMenu.cpp

#include <djvViewPlaybackMenu.h>

#include <djvViewPlaybackActions.h>

#include <QApplication>

//------------------------------------------------------------------------------
// djvViewPlaybackMenuPrivate
//------------------------------------------------------------------------------

struct djvViewPlaybackMenuPrivate
{};

//------------------------------------------------------------------------------
// djvViewPlaybackMenu
//------------------------------------------------------------------------------

djvViewPlaybackMenu::djvViewPlaybackMenu(
    djvViewAbstractActions * actions,
    QWidget *                parent) :
    djvViewAbstractMenu(actions, parent),
    _p(new djvViewPlaybackMenuPrivate)
{
    // Create the menus.

    Q_FOREACH(QAction * action,
        actions->group(djvViewPlaybackActions::PLAYBACK_GROUP)->actions())
        addAction(action);

    addAction(actions->action(djvViewPlaybackActions::PLAYBACK_TOGGLE));

    QMenu * loopMenu = addMenu(qApp->translate("djvViewPlaybackMenu", "&Loop"));
    Q_FOREACH(QAction * action,
        actions->group(djvViewPlaybackActions::LOOP_GROUP)->actions())
        loopMenu->addAction(action);

    addAction(actions->action(djvViewPlaybackActions::EVERY_FRAME));

    addSeparator();

    Q_FOREACH(QAction * action,
        actions->group(djvViewPlaybackActions::FRAME_GROUP)->actions())
        addAction(action);

    addSeparator();

    Q_FOREACH(QAction * action,
        actions->group(djvViewPlaybackActions::IN_OUT_GROUP)->actions())
        addAction(action);

    addSeparator();

    QMenu * layoutMenu = addMenu(qApp->translate("djvViewPlaybackMenu", "Layout"));
    Q_FOREACH(QAction * action,
        actions->group(djvViewPlaybackActions::LAYOUT_GROUP)->actions())
        layoutMenu->addAction(action);

    // Initialize.

    setTitle(qApp->translate("djvViewPlaybackMenu", "&Playback"));
}

djvViewPlaybackMenu::~djvViewPlaybackMenu()
{
    delete _p;
}
