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
	_dimensionsViewerWidget(),
    _configurationAction(),
    _dropWidget(nullptr)
{
}

void DimensionsViewerPlugin::init()
{
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    _dimensionsViewerWidget = new QWidget(this);
    _configurationAction    = new ConfigurationAction(this);
    _dropWidget             = new DropWidget(_dimensionsViewerWidget);

    _dimensionsViewerWidget->setAcceptDrops(true);

    auto mainLayout = new QVBoxLayout();

    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    mainLayout->addWidget(_dimensionsViewerWidget, 1);
    mainLayout->addWidget(_configurationAction->createWidget(this));

    setLayout(mainLayout);

    _dropWidget->setDropIndicatorWidget(new DropWidget::DropIndicatorWidget(this, "No data loaded", "Drag an item from the data hierarchy and drop it here to visualize data..."));

    _dropWidget->initialize([this](const QMimeData* mimeData) -> DropWidget::DropRegions {
        DropWidget::DropRegions dropRegions;

        const auto mimeText = mimeData->text();
        const auto tokens = mimeText.split("\n");
        const auto datasetName = tokens[0];
        const auto dataType = DataType(tokens[1]);
        const auto dataTypes = DataTypes({ PointType });
        const auto candidateDataset = _core->requestData<Points>(datasetName);
        const auto candidateDatasetName = candidateDataset.getName();

        if (!dataTypes.contains(dataType))
            dropRegions << new DropWidget::DropRegion(this, "Incompatible data", "This type of data is not supported", false);
        
        if (dataType == PointType) {
            dropRegions << new DropWidget::DropRegion(this, "Points", QString("Visualize %1 dimensions in channel %2").arg(candidateDatasetName), true, [this, datasetName, candidateDatasetName]() {
                _dropWidget->setShowDropIndicator(false);
            });
        }

        return dropRegions;
    });

    registerDataEventByType(PointType, [this](hdps::DataEvent* dataEvent) {
        if (dataEvent->getType() == EventType::DataAdded) {
            //_dropWidget->setShowDropIndicator(false);
            //_configurationsModel.addDataset(dataEvent->dataSetName);
        }

        if (dataEvent->getType() == EventType::SelectionChanged)
        {
            /*
            const auto hits = _configurationsModel.match(_configurationsModel.index(0, Configuration::Column::ChannelDatasetNameStart), Qt::DisplayRole, dataEvent->dataSetName, -1, Qt::MatchExactly);

            if (!hits.isEmpty()) {
                _configurationsModel.setData(_configurationsModel.index(hits.first().row(), Configuration::Column::SelectionStamp), 0);
            }
            */
        }
    });
}

DimensionsViewerPlugin* DimensionsViewerPluginFactory::produce()
{
    return new DimensionsViewerPlugin();
}