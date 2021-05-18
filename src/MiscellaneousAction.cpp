#include "MiscellaneousAction.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>

using namespace hdps::gui;

MiscellaneousAction::MiscellaneousAction(DimensionsViewerPlugin* dimensionsViewerPlugin) :
    PluginAction(dimensionsViewerPlugin, "Miscellaneous"),
    _advancedSettingsEnabled(this, "Advanced settings"),
    _showDimensionNames(this, "Show dimension names")
{
}

MiscellaneousAction::Widget::Widget(QWidget* parent, MiscellaneousAction* miscellaneousAction) :
    WidgetAction::Widget(parent, miscellaneousAction),
    _layout()
{
    setLayout(&_layout);

    auto groupBox = new QGroupBox("Miscellaneous");

    _layout.addWidget(groupBox);

    groupBox->setLayout(new QVBoxLayout());
    groupBox->layout()->addWidget(new StandardAction::CheckBox(this, &miscellaneousAction->_advancedSettingsEnabled));
    groupBox->layout()->addWidget(new StandardAction::CheckBox(this, &miscellaneousAction->_showDimensionNames));
}