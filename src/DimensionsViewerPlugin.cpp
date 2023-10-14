#include "DimensionsViewerPlugin.h"
#include "DimensionsViewerWidget.h"
#include "SettingsAction.h"

#include <widgets/DropWidget.h>

#include <actions/PluginTriggerAction.h>

#include <DatasetsMimeData.h>

#include <QDebug>
#include <QMimeData>

Q_PLUGIN_METADATA(IID "nl.BioVault.DimensionsViewerPlugin")

using namespace mv;
using namespace mv::gui;

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
    getWidget().setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    _dimensionsViewerWidget = new DimensionsViewerWidget(this);
    _settingsAction         = new SettingsAction(*this);
    _dropWidget             = new DropWidget(_dimensionsViewerWidget);

    auto mainLayout = new QVBoxLayout();

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    mainLayout->addWidget(_settingsAction->createWidget(&getWidget()));
    mainLayout->addWidget(_dimensionsViewerWidget, 1);

    getWidget().setLayout(mainLayout);

    _dropWidget->setDropIndicatorWidget(new DropWidget::DropIndicatorWidget(&getWidget(), "No channels to display", "Drag an item from the data hierarchy and drop it here to visualize data..."));

    _dropWidget->initialize([this](const QMimeData* mimeData) -> DropWidget::DropRegions {
        DropWidget::DropRegions dropRegions;

        const auto datasetsMimeData = dynamic_cast<const DatasetsMimeData*>(mimeData);

        if (datasetsMimeData == nullptr)
            return dropRegions;

        if (datasetsMimeData->getDatasets().count() > 1)
            return dropRegions;

        const auto dataset = datasetsMimeData->getDatasets().first();
        const auto datasetGuiName = dataset->text();
        const auto datasetId = dataset->getId();
        const auto dataType = dataset->getDataType();
        const auto dataTypes = DataTypes({ PointType });

        if (!dataTypes.contains(dataType))
            dropRegions << new DropWidget::DropRegion(this, "Incompatible data", "This type of data is not supported", "exclamation-circle", false);

        if (dataType == PointType) {
            const auto candidateDataset = _core->requestDataset(datasetId);

            dropRegions << new DropWidget::DropRegion(this, "Points", QString("Visualize %1 dimensions").arg(candidateDataset->getGuiName()), "map-marker-alt", true, [this, candidateDataset]() {
                loadData({ candidateDataset });
            });
        }

        return dropRegions;
    });
}

void DimensionsViewerPlugin::loadData(const mv::Datasets& datasets)
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

mv::DataTypes DimensionsViewerPluginFactory::supportedDataTypes() const
{
    DataTypes supportedTypes;
    supportedTypes.append(PointType);
    return supportedTypes;
}

mv::gui::PluginTriggerActions DimensionsViewerPluginFactory::getPluginTriggerActions(const mv::Datasets& datasets) const
{
    PluginTriggerActions pluginTriggerActions;

    const auto getPluginInstance = [this]() -> DimensionsViewerPlugin* {
        return dynamic_cast<DimensionsViewerPlugin*>(plugins().requestViewPlugin(getKind()));
    };

    const auto numberOfDatasets = datasets.count();

    if (PluginFactory::areAllDatasetsOfTheSameType(datasets, PointType)) {
        if (numberOfDatasets == 1) {
            auto pluginTriggerAction = new PluginTriggerAction(const_cast<DimensionsViewerPluginFactory*>(this), this, "Dimensions", "View dimensions", getIcon(), [this, getPluginInstance, datasets](PluginTriggerAction& pluginTriggerAction) -> void {
                for (auto dataset : datasets)
                    getPluginInstance()->loadData({ dataset });
            });

            pluginTriggerActions << pluginTriggerAction;
        }

        if (numberOfDatasets >= 2) {
            auto pluginTriggerAction = new PluginTriggerAction(const_cast<DimensionsViewerPluginFactory*>(this), this, "Compare dimensions", "View selected datasets together in a single dimensions viewer", getIcon(), [this, getPluginInstance, datasets](PluginTriggerAction& pluginTriggerAction) -> void {
                getPluginInstance()->loadData(datasets);
            });

            pluginTriggerActions << pluginTriggerAction;
        }
    }

    return pluginTriggerActions;
}
