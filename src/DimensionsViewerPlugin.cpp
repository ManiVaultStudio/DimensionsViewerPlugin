#include "DimensionsViewerPlugin.h"
#include "DimensionsViewerWidget.h"
#include "SettingsAction.h"

#include <widgets/DropWidget.h>

#include <actions/PluginTriggerAction.h>

#include <QDebug>
#include <QMimeData>

Q_PLUGIN_METADATA(IID "nl.BioVault.DimensionsViewerPlugin")

using namespace hdps;
using namespace hdps::gui;

DimensionsViewerPlugin::DimensionsViewerPlugin(const PluginFactory* factory) :
    ViewPlugin(factory),
    _dimensionsViewerWidget(nullptr),
    _layersModel(this),
    _settingsAction(nullptr),
    _dropWidget(nullptr)
{
}

void DimensionsViewerPlugin::init()
{
    _widget.setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    _dimensionsViewerWidget = new DimensionsViewerWidget(this);
    _settingsAction         = new SettingsAction(*this);
    _dropWidget             = new DropWidget(_dimensionsViewerWidget);

    auto mainLayout = new QVBoxLayout();

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    mainLayout->addWidget(_settingsAction->createWidget(&_widget));
    mainLayout->addWidget(_dimensionsViewerWidget, 1);

    _widget.setLayout(mainLayout);

    _dropWidget->setDropIndicatorWidget(new DropWidget::DropIndicatorWidget(&_widget, "No channels to display", "Drag an item from the data hierarchy and drop it here to visualize data..."));

    _dropWidget->initialize([this](const QMimeData* mimeData) -> DropWidget::DropRegions {
        DropWidget::DropRegions dropRegions;

        const auto mimeText             = mimeData->text();
        const auto tokens               = mimeText.split("\n");
        const auto datasetName          = tokens[0];
        const auto datasetId            = tokens[1];
        const auto dataType             = DataType(tokens[2]);
        const auto dataTypes            = DataTypes({ PointType });
        const auto candidateDataset     = _core->requestDataset(datasetId);

        if (!dataTypes.contains(dataType))
            dropRegions << new DropWidget::DropRegion(this, "Incompatible data", "This type of data is not supported", "exclamation-circle", false);

        if (dataType == PointType) {
            dropRegions << new DropWidget::DropRegion(this, "Points", QString("Visualize %1 dimensions").arg(candidateDataset->getGuiName()), "map-marker-alt", true, [this, candidateDataset]() {
                loadData({ candidateDataset });
            });
        }

        return dropRegions;
    });
}

void DimensionsViewerPlugin::loadData(const hdps::Datasets& datasets)
{
    for (const auto& dataset : datasets)
        _layersModel.addLayer(new Layer(*_settingsAction, dataset));

    if (_layersModel.rowCount() >= 1)
        _dropWidget->setShowDropIndicator(false);
}

LayersModel& DimensionsViewerPlugin::getLayersModel()
{
    return _layersModel;
}

QIcon DimensionsViewerPluginFactory::getIcon(const QColor& color /*= Qt::black*/) const
{
    return Application::getIconFont("FontAwesome").getIcon("chart-area", color);
}

DimensionsViewerPlugin* DimensionsViewerPluginFactory::produce()
{
    return new DimensionsViewerPlugin(this);
}

hdps::DataTypes DimensionsViewerPluginFactory::supportedDataTypes() const
{
    DataTypes supportedTypes;
    supportedTypes.append(PointType);
    return supportedTypes;
}

hdps::gui::PluginTriggerActions DimensionsViewerPluginFactory::getPluginTriggerActions(const hdps::Datasets& datasets) const
{
    PluginTriggerActions pluginTriggerActions;

    const auto getPluginInstance = [this]() -> DimensionsViewerPlugin* {
        return dynamic_cast<DimensionsViewerPlugin*>(Application::core()->requestPlugin(getKind()));
    };

    const auto numberOfDatasets = datasets.count();

    if (PluginFactory::areAllDatasetsOfTheSameType(datasets, PointType)) {
        if (numberOfDatasets == 1) {
            auto pluginTriggerAction = createPluginTriggerAction("Dimensions", "View dimensions", datasets, "chart-area");

            connect(pluginTriggerAction, &QAction::triggered, [this, getPluginInstance, datasets]() -> void {
                for (auto dataset : datasets)
                    getPluginInstance()->loadData({ dataset });
            });

            pluginTriggerActions << pluginTriggerAction;
        }

        if (numberOfDatasets >= 2) {
            auto pluginTriggerAction = createPluginTriggerAction("Compare dimensions", "View selected datasets together in a single dimensions viewer", datasets, "chart-area");

            connect(pluginTriggerAction, &QAction::triggered, [this, getPluginInstance, datasets]() -> void {
                getPluginInstance()->loadData(datasets);
            });

            pluginTriggerActions << pluginTriggerAction;
        }
    }

    return pluginTriggerActions;
}
