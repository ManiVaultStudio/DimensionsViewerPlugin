#include "DimensionsViewerPlugin.h"
#include "TreeItemWidget.h"
#include "ViewerWidget.h"
#include "ConfigurationWidget.h"
#include "Channel.h"

#include <QDebug>
#include <QSplitter>

Q_PLUGIN_METADATA(IID "nl.tudelft.DimensionsViewerPlugin")

DimensionsViewerPlugin::DimensionsViewerPlugin() : 
	ViewPlugin("Dimensions Viewer"),
	_model(this),
	_viewerWidget(),
	_configurationWidget()
{
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    tree::Item::setModel(&_model);
    Channel::setDimensionsViewerPlugin(this);
    TreeItemWidget::setDimensionsViewerPlugin(this);

	_viewerWidget = new ViewerWidget(this);
	_configurationWidget = new ConfigurationWidget(this);
}

void DimensionsViewerPlugin::init()
{
    auto splitter = new QSplitter();

    auto mainLayout = new QVBoxLayout();

    splitter->addWidget(_viewerWidget);
    splitter->addWidget(reinterpret_cast<QWidget*>(_configurationWidget));

    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 0);

    splitter->setOrientation(Qt::Vertical);
    splitter->setCollapsible(1, true);

    addWidget(splitter);
}

void DimensionsViewerPlugin::dataAdded(const QString dataset)
{
	_model.addDataset(dataset);
}

void DimensionsViewerPlugin::dataChanged(const QString dataset)
{
}

void DimensionsViewerPlugin::dataRemoved(const QString dataset)
{
}

void DimensionsViewerPlugin::selectionChanged(const QString dataName)
{
    //TODO
    //const auto hits = _configurationsModel.match(_configurationsModel.index(0, Configuration::Column::ChannelDataNameStart), Qt::DisplayRole, dataName, -1, Qt::MatchExactly);

    //if (!hits.isEmpty()) {
    //    //const auto selectionStamp = _configurationsModel.data(_configurationsModel.index(hits.first().row(), Configuration::Column::SelectionStamp)).toInt();
    //    _configurationsModel.setData(_configurationsModel.index(hits.first().row(), Configuration::Column::SelectionStamp), 0);
    //}
}

hdps::DataTypes DimensionsViewerPlugin::supportedDataTypes() const
{
	hdps::DataTypes supportedTypes;

	supportedTypes.append(PointType);

	return supportedTypes;
}

DimensionsViewerPlugin* DimensionsViewerPluginFactory::produce()
{
    return new DimensionsViewerPlugin();
}