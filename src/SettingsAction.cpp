#include "SettingsAction.h"
#include "ChannelAction.h"
#include "DimensionsViewerPlugin.h"

#include <event/Event.h>

#include <QDebug>
#include <QLabel>

using namespace hdps;
using namespace hdps::gui;

SettingsAction::SettingsAction(DimensionsViewerPlugin& dimensionsViewerPlugin) :
    WidgetAction(&dimensionsViewerPlugin, "SettingsAction"),
    _dimensionsViewerPlugin(dimensionsViewerPlugin),
    _layersAction(*this),
    _dimensionsAction(*this),
    _subsamplingAction(*this),
    _spec(),
    _isLoading(false)
{
    _spec["modified"]           = 0;
    _spec["showDimensionNames"] = true;

    connect(&_dimensionsAction.getShowNamesAction(), &ToggleAction::toggled, [this](bool state) {
        _spec["modified"] = _spec["modified"].toInt() + 1;
        _spec["showDimensionNames"] = state;
    });
}

DimensionsViewerPlugin& SettingsAction::getDimensionsViewerPlugin()
{
    return _dimensionsViewerPlugin;
}

QVariantMap SettingsAction::getSpec()
{
    QVariantMap channels;

    for (std::int32_t rowIndex = 0; rowIndex < _dimensionsViewerPlugin.getLayersModel().rowCount(); rowIndex++) {
        auto layer = static_cast<Layer*>(_dimensionsViewerPlugin.getLayersModel().index(rowIndex, 0).internalPointer());

        auto& channelAction = layer->getChannelAction();

        const auto channelSpec = channelAction.getSpec();

        if (channelAction.shouldDisplaySpec())
            channels[channelAction.getInternalName()] = channelSpec;
    }

    _spec["channels"] = channels;

    return _spec;
}

SettingsAction::Widget::Widget(QWidget* parent, SettingsAction* settingsAction) :
    WidgetActionWidget(parent, settingsAction, State::Standard)
{
    setAutoFillBackground(true);

    auto layout = new QHBoxLayout();

    setLayout(layout);

    layout->setContentsMargins(3, 3, 3, 3);
    layout->setSpacing(3);

    layout->addWidget(settingsAction->getLayersAction().createCollapsedWidget(this));
    layout->addWidget(settingsAction->getDimensionsAction().createCollapsedWidget(this));
    layout->addStretch(1);
}