#include "DimensionsViewerPlugin.h"
#include "DimensionsViewerWidget.h"
#include "ConfigurationAction.h"

#include <widgets/DropWidget.h>

#include <QDebug>
#include <QMimeData>

Q_PLUGIN_METADATA(IID "nl.tudelft.DimensionsViewerPlugin")

using namespace hdps;
using namespace hdps::gui;

DimensionsViewerPlugin::DimensionsViewerPlugin(const PluginFactory* factory) :
    ViewPlugin(factory),
    _dimensionsViewerWidget(nullptr),
    _configurationAction(nullptr),
    _dropWidget(nullptr)
{
}

void DimensionsViewerPlugin::init()
{
    _widget.setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    _dimensionsViewerWidget = new DimensionsViewerWidget(this);
    _configurationAction    = new ConfigurationAction(this);
    _dropWidget             = new DropWidget(_dimensionsViewerWidget);

    auto mainLayout = new QVBoxLayout();

    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    mainLayout->addWidget(_dimensionsViewerWidget, 1);
    mainLayout->addWidget(_configurationAction->createWidget(&_widget));

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

        if (candidateDataset == _configurationAction->getLoadedDataset()) {
            dropRegions << new DropWidget::DropRegion(this, "Warning", "Data already loaded", false);
        }
        else {
            if (dataType == PointType) {
                dropRegions << new DropWidget::DropRegion(this, "Points", QString("Visualize %1 dimensions").arg(candidateDataset->getGuiName()), "map-marker-alt", true, [this, candidateDataset]() {
                    _configurationAction->loadDataset(candidateDataset->getGuiName());
                    _dropWidget->setShowDropIndicator(false);
                });
            }
        }

        return dropRegions;
    });
}

QIcon DimensionsViewerPluginFactory::getIcon() const
{
    return Application::getIconFont("FontAwesome").getIcon("chart-area");
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
