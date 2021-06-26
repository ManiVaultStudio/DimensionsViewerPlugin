#include "DimensionsViewerPlugin.h"
#include "DimensionsViewerWidget.h"
#include "ConfigurationAction.h"

#include <widgets/DropWidget.h>

#include <QDebug>
#include <QMimeData>

Q_PLUGIN_METADATA(IID "nl.tudelft.DimensionsViewerPlugin")

using namespace hdps;
using namespace hdps::gui;

DimensionsViewerPlugin::DimensionsViewerPlugin() : 
	ViewPlugin("Dimensions Viewer"),
	_dimensionsViewerWidget(nullptr),
    _configurationAction(nullptr),
    _dropWidget(nullptr)
{
}

void DimensionsViewerPlugin::init()
{
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    _dimensionsViewerWidget = new DimensionsViewerWidget(this);
    _configurationAction    = new ConfigurationAction(this);
    _dropWidget             = new DropWidget(_dimensionsViewerWidget);

    auto mainLayout = new QVBoxLayout();

    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    mainLayout->addWidget(_dimensionsViewerWidget, 1);
    mainLayout->addWidget(_configurationAction->createWidget(this));

    setLayout(mainLayout);
    
    _dropWidget->setDropIndicatorWidget(new DropWidget::DropIndicatorWidget(this, "No channels to display", "Drag an item from the data hierarchy and drop it here to visualize data..."));

    _dropWidget->initialize([this](const QMimeData* mimeData) -> DropWidget::DropRegions {
        DropWidget::DropRegions dropRegions;

        const auto mimeText             = mimeData->text();
        const auto tokens               = mimeText.split("\n");
        const auto datasetName          = tokens[0];
        const auto dataType             = DataType(tokens[1]);
        const auto dataTypes            = DataTypes({ PointType });
        const auto candidateDataset     = _core->requestData<Points>(datasetName);
        const auto candidateDatasetName = candidateDataset.getName();

        if (!dataTypes.contains(dataType))
            dropRegions << new DropWidget::DropRegion(this, "Incompatible data", "This type of data is not supported", false);

        if (datasetName == _configurationAction->getLoadedDataset()) {
            dropRegions << new DropWidget::DropRegion(this, "Warning", "Data already loaded", false);
        }
        else {
            if (dataType == PointType) {
                dropRegions << new DropWidget::DropRegion(this, "Points", QString("Visualize %1 dimensions").arg(candidateDatasetName), true, [this, candidateDatasetName]() {
                    if (candidateDatasetName.isEmpty())
                        return;

                    _configurationAction->loadDataset(candidateDatasetName);

                    _dropWidget->setShowDropIndicator(false);
                });
            }
        }

        return dropRegions;
    });

    connect(&_dimensionsViewerWidget->getSpecSynchronizer(), &SpecSynchronizer::dimensionSelectionChanged, [this](const std::uint32_t& dimensionIndex) {
        auto& channels = _configurationAction->getChannelsAction().getChannels();

        if (channels.isEmpty())
            return;
        
        auto points = channels.first()->getPoints1();

        if (points == nullptr)
            return;

        points->dimensionIndices = std::vector<std::uint32_t>({ dimensionIndex });

        _core->notifyDimensionSelectionChanged(points->getName());

        qDebug() << points->getName() << "dimension selection changed";
    });
}

DimensionsViewerPlugin* DimensionsViewerPluginFactory::produce()
{
    return new DimensionsViewerPlugin();
}