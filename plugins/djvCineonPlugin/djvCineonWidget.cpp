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

//! \file djvCineonWidget.cpp

#include <djvCineonWidget.h>

#include <djvFloatEditSlider.h>
#include <djvGuiContext.h>
#include <djvIntEditSlider.h>
#include <djvPrefsGroupBox.h>
#include <djvStyle.h>

#include <djvImageIo.h>
#include <djvSignalBlocker.h>

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QVBoxLayout>

extern "C"
{

DJV_PLUGIN_EXPORT djvPlugin * djvImageIoWidgetEntry(djvCoreContext * context)
{
    return new djvCineonWidgetPlugin(context);
}

} // extern "C"

//------------------------------------------------------------------------------
// djvCineonWidget
//------------------------------------------------------------------------------

djvCineonWidget::djvCineonWidget(djvImageIo * plugin, djvGuiContext * context) :
    djvImageIoWidget(plugin, context),
    _inputColorProfileLayout (0),
    _inputColorProfileWidget (0),
    _inputBlackPointWidget   (0),
    _inputWhitePointWidget   (0),
    _inputGammaWidget        (0),
    _inputSoftClipWidget     (0),
    _outputColorProfileLayout(0),
    _outputColorProfileWidget(0),
    _outputBlackPointWidget  (0),
    _outputWhitePointWidget  (0),
    _outputGammaWidget       (0)
{
    //DJV_DEBUG("djvCineonWidget::djvCineonWidget");

    // Create the widgets.

    _inputColorProfileWidget = new QComboBox;
    _inputColorProfileWidget->addItems(djvCineon::colorProfileLabels());
    _inputColorProfileWidget->setSizePolicy(
        QSizePolicy::Fixed, QSizePolicy::Fixed);
    
    _inputBlackPointWidget = new djvIntEditSlider(context);
    _inputBlackPointWidget->setRange(0, 1023);

    _inputWhitePointWidget = new djvIntEditSlider(context);
    _inputWhitePointWidget->setRange(0, 1023);

    _inputGammaWidget = new djvFloatEditSlider(context);
    _inputGammaWidget->setRange(0.01, 4.0);

    _inputSoftClipWidget = new djvIntEditSlider(context);
    _inputSoftClipWidget->setRange(0, 50);
    
    _outputColorProfileWidget = new QComboBox;
    _outputColorProfileWidget->addItems(djvCineon::colorProfileLabels());
    _outputColorProfileWidget->setSizePolicy(
        QSizePolicy::Fixed, QSizePolicy::Fixed);

    _outputBlackPointWidget = new djvIntEditSlider(context);
    _outputBlackPointWidget->setRange(0, 1023);

    _outputWhitePointWidget = new djvIntEditSlider(context);
    _outputWhitePointWidget->setRange(0, 1023);

    _outputGammaWidget = new djvFloatEditSlider(context);
    _outputGammaWidget->setRange(0.01, 4.0);

    // Layout the widgets.

    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->setSpacing(context->style()->sizeMetric().largeSpacing);

    djvPrefsGroupBox * prefsGroupBox = new djvPrefsGroupBox(
        qApp->translate("djvCineonWidget", "Input Color Profile"),
        qApp->translate("djvCineonWidget", "Set the color profile used when loading Cineon images."),
        context);
    _inputColorProfileLayout = prefsGroupBox->createLayout();
    _inputColorProfileLayout->addRow(
        qApp->translate("djvCineonWidget", "Profile:"),
        _inputColorProfileWidget);
    _inputColorProfileLayout->addRow(
        qApp->translate("djvCineonWidget", "Black:"),
        _inputBlackPointWidget);
    _inputColorProfileLayout->addRow(
        qApp->translate("djvCineonWidget", "White:"),
        _inputWhitePointWidget);
    _inputColorProfileLayout->addRow(
        qApp->translate("djvCineonWidget", "Gamma:"),
        _inputGammaWidget);
    _inputColorProfileLayout->addRow(
        qApp->translate("djvCineonWidget", "Soft clip:"),
        _inputSoftClipWidget);
    layout->addWidget(prefsGroupBox);
    
    prefsGroupBox = new djvPrefsGroupBox(
        qApp->translate("djvCineonWidget", "Output Color Profile"),
        qApp->translate("djvCineonWidget", "Set the color profile used when saving Cineon images."),
        context);
    _outputColorProfileLayout = prefsGroupBox->createLayout();
    _outputColorProfileLayout->addRow(
        qApp->translate("djvCineonWidget", "Profile:"),
        _outputColorProfileWidget);
    _outputColorProfileLayout->addRow(
        qApp->translate("djvCineonWidget", "Black:"),
        _outputBlackPointWidget);
    _outputColorProfileLayout->addRow(
        qApp->translate("djvCineonWidget", "White:"),
        _outputWhitePointWidget);
    _outputColorProfileLayout->addRow(
        qApp->translate("djvCineonWidget", "Gamma:"),
        _outputGammaWidget);
    layout->addWidget(prefsGroupBox);

    layout->addStretch();

    // Initialize.

    _inputBlackPointWidget->setDefaultValue(
        djvCineon::Options().inputFilmPrint.black);
    _inputWhitePointWidget->setDefaultValue(
        djvCineon::Options().inputFilmPrint.white);
    _inputGammaWidget->setDefaultValue(
        djvCineon::Options().inputFilmPrint.gamma);
    _inputSoftClipWidget->setDefaultValue(
        djvCineon::Options().inputFilmPrint.softClip);

    _outputBlackPointWidget->setDefaultValue(
        djvCineon::Options().outputFilmPrint.black);
    _outputWhitePointWidget->setDefaultValue(
        djvCineon::Options().outputFilmPrint.white);
    _outputGammaWidget->setDefaultValue(
        djvCineon::Options().outputFilmPrint.gamma);

    QStringList tmp;
    tmp = plugin->option(
        plugin->options()[djvCineon::INPUT_COLOR_PROFILE_OPTION]);
    tmp >> _options.inputColorProfile;
    tmp = plugin->option(
        plugin->options()[djvCineon::INPUT_FILM_PRINT_OPTION]);
    tmp >> _options.inputFilmPrint;
    tmp = plugin->option(
        plugin->options()[djvCineon::OUTPUT_COLOR_PROFILE_OPTION]);
    tmp >> _options.outputColorProfile;
    tmp = plugin->option(
        plugin->options()[djvCineon::OUTPUT_FILM_PRINT_OPTION]);
    tmp >> _options.outputFilmPrint;

    widgetUpdate();

    // Setup the callbacks.
    
    connect(
        plugin,
        SIGNAL(optionChanged(const QString &)),
        SLOT(pluginCallback(const QString &)));
    
    connect(
        _inputColorProfileWidget,
        SIGNAL(activated(int)),
        SLOT(inputColorProfileCallback(int)));

    connect(
        _inputBlackPointWidget,
        SIGNAL(valueChanged(int)),
        SLOT(inputBlackPointCallback(int)));

    connect(
        _inputWhitePointWidget,
        SIGNAL(valueChanged(int)),
        SLOT(inputWhitePointCallback(int)));

    connect(
        _inputGammaWidget,
        SIGNAL(valueChanged(double)),
        SLOT(inputGammaCallback(double)));

    connect(
        _inputSoftClipWidget,
        SIGNAL(valueChanged(int)),
        SLOT(inputSoftClipCallback(int)));

    connect(
        _outputColorProfileWidget,
        SIGNAL(activated(int)),
        SLOT(outputColorProfileCallback(int)));

    connect(
        _outputBlackPointWidget,
        SIGNAL(valueChanged(int)),
        SLOT(outputBlackPointCallback(int)));

    connect(
        _outputWhitePointWidget,
        SIGNAL(valueChanged(int)),
        SLOT(outputWhitePointCallback(int)));

    connect(
        _outputGammaWidget,
        SIGNAL(valueChanged(double)),
        SLOT(outputGammaCallback(double)));
}

djvCineonWidget::~djvCineonWidget()
{
    //DJV_DEBUG("djvCineonWidget::~djvCineonWidget");
}

void djvCineonWidget::resetPreferences()
{
    _options = djvCineon::Options();
    
    pluginUpdate();
    widgetUpdate();
}

void djvCineonWidget::pluginCallback(const QString & option)
{
    try
    {
        QStringList tmp;
        tmp = plugin()->option(option);

        if (0 == option.compare(plugin()->options()[
            djvCineon::INPUT_COLOR_PROFILE_OPTION], Qt::CaseInsensitive))
                tmp >> _options.inputColorProfile;
        else if (0 == option.compare(plugin()->options()[
            djvCineon::INPUT_FILM_PRINT_OPTION], Qt::CaseInsensitive))
                tmp >> _options.inputFilmPrint;
        else if (0 == option.compare(plugin()->options()[
            djvCineon::OUTPUT_COLOR_PROFILE_OPTION], Qt::CaseInsensitive))
                tmp >> _options.outputColorProfile;
        else if (0 == option.compare(plugin()->options()[
            djvCineon::OUTPUT_FILM_PRINT_OPTION], Qt::CaseInsensitive))
                tmp >> _options.outputFilmPrint;
    }
    catch (const QString &)
    {}

    widgetUpdate();
}

void djvCineonWidget::inputColorProfileCallback(int in)
{
    _options.inputColorProfile = static_cast<djvCineon::COLOR_PROFILE>(in);

    pluginUpdate();
}

void djvCineonWidget::inputBlackPointCallback(int in)
{
    _options.inputFilmPrint.black = in;

    pluginUpdate();
}

void djvCineonWidget::inputWhitePointCallback(int in)
{
    _options.inputFilmPrint.white = in;

    pluginUpdate();
}

void djvCineonWidget::inputGammaCallback(double in)
{
    _options.inputFilmPrint.gamma = in;

    pluginUpdate();
}

void djvCineonWidget::inputSoftClipCallback(int in)
{
    _options.inputFilmPrint.softClip = in;

    pluginUpdate();
}

void djvCineonWidget::outputColorProfileCallback(int in)
{
    _options.outputColorProfile = static_cast<djvCineon::COLOR_PROFILE>(in);

    pluginUpdate();
}

void djvCineonWidget::outputBlackPointCallback(int in)
{
    _options.outputFilmPrint.black = in;

    pluginUpdate();
}

void djvCineonWidget::outputWhitePointCallback(int in)
{
    _options.outputFilmPrint.white = in;

    pluginUpdate();
}

void djvCineonWidget::outputGammaCallback(double in)
{
    _options.outputFilmPrint.gamma = in;

    pluginUpdate();
}

void djvCineonWidget::pluginUpdate()
{
    //DJV_DEBUG("djvCineonWidget::pluginUpdate");

    QStringList tmp;
    tmp << _options.inputColorProfile;
    plugin()->setOption(
        plugin()->options()[djvCineon::INPUT_COLOR_PROFILE_OPTION], tmp);
    tmp << _options.inputFilmPrint;
    plugin()->setOption(
        plugin()->options()[djvCineon::INPUT_FILM_PRINT_OPTION], tmp);
    tmp << _options.outputColorProfile;
    plugin()->setOption(
        plugin()->options()[djvCineon::OUTPUT_COLOR_PROFILE_OPTION], tmp);
    tmp << _options.outputFilmPrint;
    plugin()->setOption(
        plugin()->options()[djvCineon::OUTPUT_FILM_PRINT_OPTION], tmp);
}

void djvCineonWidget::widgetUpdate()
{
    //DJV_DEBUG("djvCineonWidget::widgetUpdate");

    djvSignalBlocker signalBlocker(QObjectList() <<
        _inputColorProfileWidget <<
        _inputBlackPointWidget <<
        _inputWhitePointWidget <<
        _inputGammaWidget <<
        _inputSoftClipWidget <<
        _outputColorProfileWidget <<
        _outputBlackPointWidget <<
        _outputWhitePointWidget <<
        _outputGammaWidget);
    
    _inputBlackPointWidget->setVisible(
        djvCineon::COLOR_PROFILE_AUTO == _options.inputColorProfile ||
        djvCineon::COLOR_PROFILE_FILM_PRINT == _options.inputColorProfile);
    _inputColorProfileLayout->labelForField(_inputBlackPointWidget)->setVisible(
        djvCineon::COLOR_PROFILE_AUTO == _options.inputColorProfile ||
        djvCineon::COLOR_PROFILE_FILM_PRINT == _options.inputColorProfile);
    _inputWhitePointWidget->setVisible(
        djvCineon::COLOR_PROFILE_AUTO == _options.inputColorProfile ||
        djvCineon::COLOR_PROFILE_FILM_PRINT == _options.inputColorProfile);
    _inputColorProfileLayout->labelForField(_inputWhitePointWidget)->setVisible(
        djvCineon::COLOR_PROFILE_AUTO == _options.inputColorProfile ||
        djvCineon::COLOR_PROFILE_FILM_PRINT == _options.inputColorProfile);
    _inputGammaWidget->setVisible(
        djvCineon::COLOR_PROFILE_AUTO == _options.inputColorProfile ||
        djvCineon::COLOR_PROFILE_FILM_PRINT == _options.inputColorProfile);
    _inputColorProfileLayout->labelForField(_inputGammaWidget)->setVisible(
        djvCineon::COLOR_PROFILE_AUTO == _options.inputColorProfile ||
        djvCineon::COLOR_PROFILE_FILM_PRINT == _options.inputColorProfile);
    _inputSoftClipWidget->setVisible(
        djvCineon::COLOR_PROFILE_AUTO == _options.inputColorProfile ||
        djvCineon::COLOR_PROFILE_FILM_PRINT == _options.inputColorProfile);
    _inputColorProfileLayout->labelForField(_inputSoftClipWidget)->setVisible(
        djvCineon::COLOR_PROFILE_AUTO == _options.inputColorProfile ||
        djvCineon::COLOR_PROFILE_FILM_PRINT == _options.inputColorProfile);

    _outputBlackPointWidget->setVisible(
        djvCineon::COLOR_PROFILE_AUTO == _options.outputColorProfile ||
        djvCineon::COLOR_PROFILE_FILM_PRINT == _options.outputColorProfile);
    _outputColorProfileLayout->labelForField(_outputBlackPointWidget)->setVisible(
        djvCineon::COLOR_PROFILE_AUTO == _options.outputColorProfile ||
        djvCineon::COLOR_PROFILE_FILM_PRINT == _options.outputColorProfile);
    _outputWhitePointWidget->setVisible(
        djvCineon::COLOR_PROFILE_AUTO == _options.outputColorProfile ||
        djvCineon::COLOR_PROFILE_FILM_PRINT == _options.outputColorProfile);
    _outputColorProfileLayout->labelForField(_outputWhitePointWidget)->setVisible(
        djvCineon::COLOR_PROFILE_AUTO == _options.outputColorProfile ||
        djvCineon::COLOR_PROFILE_FILM_PRINT == _options.outputColorProfile);
    _outputGammaWidget->setVisible(
        djvCineon::COLOR_PROFILE_AUTO == _options.outputColorProfile ||
        djvCineon::COLOR_PROFILE_FILM_PRINT == _options.outputColorProfile);
    _outputColorProfileLayout->labelForField(_outputGammaWidget)->setVisible(
        djvCineon::COLOR_PROFILE_AUTO == _options.outputColorProfile ||
        djvCineon::COLOR_PROFILE_FILM_PRINT == _options.outputColorProfile);

    _inputColorProfileWidget->setCurrentIndex(_options.inputColorProfile);
    _inputBlackPointWidget->setValue(_options.inputFilmPrint.black);
    _inputWhitePointWidget->setValue(_options.inputFilmPrint.white);
    _inputGammaWidget->setValue(_options.inputFilmPrint.gamma);
    _inputSoftClipWidget->setValue(_options.inputFilmPrint.softClip);

    _outputColorProfileWidget->setCurrentIndex(_options.outputColorProfile);
    _outputBlackPointWidget->setValue(_options.outputFilmPrint.black);
    _outputWhitePointWidget->setValue(_options.outputFilmPrint.white);
    _outputGammaWidget->setValue(_options.outputFilmPrint.gamma);
}

//------------------------------------------------------------------------------
// djvCineonWidgetPlugin
//------------------------------------------------------------------------------

djvCineonWidgetPlugin::djvCineonWidgetPlugin(djvCoreContext * context) :
    djvImageIoWidgetPlugin(context)
{}

djvImageIoWidget * djvCineonWidgetPlugin::createWidget(djvImageIo * plugin) const
{
    return new djvCineonWidget(plugin, guiContext());
}

QString djvCineonWidgetPlugin::pluginName() const
{
    return djvCineon::staticName;
}
