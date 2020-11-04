#include "DimensionsViewerPlugin.h"
#include "ModelItemWidget.h"
#include "DimensionsViewerWidget.h"
#include "ConfigurationWidget.h"
#include "ChannelsWidget.h"
#include "ChannelSettingsWidget.h"
#include "GlobalSettingsWidget.h"
#include "DifferentialProfileSettingsWidget.h"
#include "MiscellaneousSettingsWidget.h"
#include "Channel.h"

#include <QDebug>

Q_PLUGIN_METADATA(IID "nl.tudelft.DimensionsViewerPlugin")

DimensionsViewerPlugin::DimensionsViewerPlugin() : 
	ViewPlugin("Dimensions Viewer"),
	_configurationsModel(this),
	_dimensionsViewerWidget(),
	_configurationWidget()
{
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    ModelItem::dimensionsViewerPlugin = this;
    ModelItemWidget::dimensionsViewerPlugin = this;

	_dimensionsViewerWidget = new DimensionsViewerWidget(this);
	_configurationWidget = new ConfigurationWidget(this);
}

void DimensionsViewerPlugin::init()
{
    auto mainLayout = new QVBoxLayout();

    mainLayout->addWidget(_dimensionsViewerWidget, 1);
    mainLayout->addWidget(reinterpret_cast<QWidget*>(_configurationWidget));

    setMainLayout(mainLayout);
}

void DimensionsViewerPlugin::dataAdded(const QString dataset)
{
	_configurationsModel.addDataset(dataset);
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