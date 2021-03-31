#include "DimensionsViewerPlugin.h"
#include "TreeItemWidget.h"
#include "ViewerWidget.h"
#include "ConfigurationWidget.h"
#include "Channel.h"

#include <widgets/DropWidget.h>

#include <QDebug>
#include <QSplitter>

using namespace hdps::gui;

Q_PLUGIN_METADATA(IID "nl.tudelft.DimensionsViewerPlugin")

DimensionsViewerPlugin::DimensionsViewerPlugin() : 
    ViewPlugin("Dimensions Viewer"),
    _model(this),
    _viewerWidget(),
    _configurationWidget(),
    _dropWidget(nullptr)
{
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    Channel::setDimensionsViewerPlugin(this);
    TreeItemWidget::setDimensionsViewerPlugin(this);

    _viewerWidget           = new ViewerWidget(this);
    _configurationWidget    = new ConfigurationWidget(this);
    _dropWidget             = new DropWidget(_viewerWidget);

    setDockingLocation(hdps::gui::DockableWidget::DockingLocation::Bottom);

    _dropWidget->setDropIndicatorWidget(new DropWidget::DropIndicatorWidget(this, "No data loaded", "Drag an item from the data hierarchy and drop it on this view to visualize data..."));

    _dropWidget->initialize([this](const QMimeData* mimeData) -> DropWidget::DropRegions {
        DropWidget::DropRegions dropRegions;

        const auto mimeText             = mimeData->text();
        const auto tokens               = mimeText.split("\n");
        const auto datasetName          = tokens[0];
        const auto dataType             = DataType(tokens[1]);
        const auto dataTypes            = DataTypes({ PointType });
        const auto candidateDataset     = parentPlugin->getCore()->requestData<Points>(datasetName);
        const auto candidateDatasetName = candidateDataset.getName();

        if (!dataTypes.contains(dataType))
            dropRegions << new DropWidget::DropRegion(this, "Incompatible data", "This type of data is not supported", false);

        if (dataType == PointType) {
            const auto currentDatasetName = parentPlugin->getCurrentDataSetName();

            if (currentDatasetName.isEmpty()) {
                dropRegions << new DropWidget::DropRegion(this, "Points", "Visualize points as parallel coordinates", true, [this, parentPlugin, candidateDatasetName]() {
                    parentPlugin->onDataInput(candidateDatasetName);
                    _dropWidget->setShowDropIndicator(false);
                });
            }
            else {
                if (candidateDatasetName == currentDatasetName) {
                    dropRegions << new DropWidget::DropRegion(this, "Warning", "Data already loaded", false);
                }
                else {
                    dropRegions << new DropWidget::DropRegion(this, "Points", "Visualize points as parallel coordinates", true, [this, parentPlugin, candidateDatasetName]() {
                        parentPlugin->onDataInput(candidateDatasetName);
                        _dropWidget->setShowDropIndicator(false);
                    });
                }
            }
        }

        return dropRegions;
    });
}

void DimensionsViewerPlugin::init()
{
    auto splitter = new QSplitter();

    auto mainLayout = new QVBoxLayout();

    setLayout(mainLayout);

    splitter->addWidget(_viewerWidget);
    splitter->addWidget(reinterpret_cast<QWidget*>(_configurationWidget));

    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 0);

    splitter->setOrientation(Qt::Vertical);
    splitter->setCollapsible(1, true);

    mainLayout->addWidget(splitter);
}

DimensionsViewerPlugin* DimensionsViewerPluginFactory::produce()
{
    return new DimensionsViewerPlugin();
}