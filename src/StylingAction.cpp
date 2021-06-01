#include "StylingAction.h"
#include "ChannelAction.h"
#include "DimensionsViewerPlugin.h"
#include "Application.h"

#include <QDebug>
#include <QLabel>

using namespace hdps;
using namespace hdps::gui;

StylingAction::StylingAction(ChannelAction* channelAction) :
    PluginAction(channelAction->getDimensionsViewerPlugin(), "Styling"),
    _showRangeAction(this, "Show range"),
    _colorAction(this, "Color"),
    _opacityAction(this, "Opacity")
{
    _showRangeAction.setCheckable(true);
    _showRangeAction.setChecked(true);

    _opacityAction.setMinimum(1.0);
    _opacityAction.setMaximum(100.0);
    _opacityAction.setSuffix("%");
    _opacityAction.setValue(100.0);

    const auto colorStepDegrees = 360.0f / 10.0f;
    const auto defaultColor     = QColor::fromHsl(channelAction->getIndex() * 40, 100, 100);

    _colorAction.setColor(defaultColor);
    _colorAction.setColor(defaultColor);

    const auto updateShowRange = [this, channelAction]() {
        _showRangeAction.setEnabled(!channelAction->isDifferential());
    };

    connect(&channelAction->getProfileTypeAction(), &OptionAction::currentIndexChanged, [this, updateShowRange](const std::int32_t& currentIndex) {
        updateShowRange();
    });

    updateShowRange();
}

StylingAction::Widget::Widget(QWidget* parent, StylingAction* stylingAction) :
    WidgetAction::Widget(parent, stylingAction),
    _layout()
{
    auto popupPushButton = new gui::PopupPushButton(this);

    popupPushButton->setIcon(Application::getIconFont("FontAwesome").getIcon("brush"));

    _layout.addWidget(popupPushButton);

    popupPushButton->setWidget(new StylingAction::PopupWidget(this, stylingAction));
    popupPushButton->setPopupAlignment(gui::PopupPushButton::PopupAlignmentFlag::TopLeft);

    setLayout(&_layout);
}

StylingAction::PopupWidget::PopupWidget(QWidget* parent, StylingAction* stylingAction) :
    WidgetAction::PopupWidget(parent, stylingAction),
    _layout()
{
    auto showRangeWidget    = new StandardAction::CheckBox(this, &stylingAction->_showRangeAction);
    auto colorWidget        = new ColorAction::Widget(this, &stylingAction->_colorAction, false);
    auto opacityWidget      = new DecimalAction::Widget(this, &stylingAction->_opacityAction, DecimalAction::Widget::Configuration::SpinBoxSlider);

    _layout.addWidget(showRangeWidget, 0, 1);
    _layout.addWidget(new QLabel("Color"), 1, 0);
    _layout.addWidget(colorWidget, 1, 1);
    _layout.addWidget(new QLabel("Opacity"), 2, 0);
    _layout.addWidget(opacityWidget, 2, 1);

    setLayout(&_layout);
}


