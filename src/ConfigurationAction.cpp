#include "ConfigurationAction.h"
#include "ChannelAction.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>

using namespace hdps::gui;

ConfigurationAction::ConfigurationAction(DimensionsViewerPlugin* dimensionsViewerPlugin) :
    PluginAction(dimensionsViewerPlugin, "Configuration"),
    _channels(),
    _showAdvancedSettings(this, "Advanced settings"),
    _showDimensionNames(this, "Show dimension names")
{
    _channels << new ChannelAction(this, 0, false, "", Qt::red);
    _channels << new ChannelAction(this, 1, false, "", Qt::red);
    _channels << new ChannelAction(this, 2, false, "", Qt::red);

    /*
    registerDataEventByType(PointType, [this](hdps::DataEvent* dataEvent) {
        if (dataEvent->getType() == EventType::DataAdded) {
            const auto dataSetName = static_cast<DataAddedEvent*>(dataEvent)->dataSetName;

            _configurations.insert(dataSetName, new ConfigurationAction(_dimensionsViewerPlugin, dataSetName));

            _currentDataset.setOptions(_configurations.keys());
        }
    });

    connect(&_currentDataset, &OptionAction::currentTextChanged, this, &SettingsAction::selectConfiguration);
    */
}

ConfigurationAction::Widget::Widget(QWidget* parent, ConfigurationAction* configurationAction) :
    WidgetAction::Widget(parent, configurationAction),
    _layout(),
    _channelsGroupBox("Channels"),
    _channelsGroupBoxLayout(),
    _miscellaneousGroupBox("Miscellaneous"),
    _miscellaneousGroupBoxLayout()
{
    setLayout(&_layout);

    _layout.addWidget(&_channelsGroupBox);
    _layout.addWidget(&_miscellaneousGroupBox);

    _channelsGroupBox.setLayout(&_channelsGroupBoxLayout);

    const auto addChannels = [this, configurationAction]() {
        for (auto channel : configurationAction->_channels)
            _channelsGroupBoxLayout.addWidget(channel->createWidget(this));
    };

    addChannels();

    _miscellaneousGroupBox.setLayout(&_miscellaneousGroupBoxLayout);

    _miscellaneousGroupBoxLayout.addWidget(new StandardAction::CheckBox(this, &configurationAction->_showAdvancedSettings));
    _miscellaneousGroupBoxLayout.addWidget(new StandardAction::CheckBox(this, &configurationAction->_showDimensionNames));
    _miscellaneousGroupBoxLayout.addStretch(1);
}