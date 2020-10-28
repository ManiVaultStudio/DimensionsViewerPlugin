#include "DimensionsViewerWidget.h"
#include "DimensionsViewerPlugin.h"

#include <QFileInfo>
#include <QWebChannel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

DimensionsViewerWidget::DimensionsViewerWidget(DimensionsViewerPlugin* dimensionsViewerPlugin) :
	QWebEngineView(),
	_dimensionsViewerPlugin(dimensionsViewerPlugin)
{
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	setAcceptDrops(true);

	auto& configurationsModel = _dimensionsViewerPlugin->getConfigurationsModel();

	QObject::connect(&configurationsModel.getSelectionModel(), &QItemSelectionModel::selectionChanged, [this, &configurationsModel](const QItemSelection& selected, const QItemSelection& deselected) {
		const auto selectedRows = configurationsModel.getSelectionModel().selectedRows();

		if (!selectedRows.isEmpty()) {
			QWebChannel* webChannel = new QWebChannel(this);

			for (auto registeredObject : webChannel->registeredObjects())
				webChannel->deregisterObject(registeredObject);

			auto selectedConfiguration = configurationsModel.getSelectedConfiguration();

			webChannel->registerObject("configuration", selectedConfiguration);
			
			page()->setWebChannel(webChannel);

			load(QUrl("qrc:DimensionsViewer.html"));
		}
	});
}

void DimensionsViewerWidget::dragEnterEvent(QDragEnterEvent* dragEnterEvent)
{
	const auto items = dragEnterEvent->mimeData()->text().split("\n");
	const auto datasetName = items.at(0);
	const auto datasetType = items.at(1);

	if (datasetType == "Points")
		dragEnterEvent->acceptProposedAction();
}

void DimensionsViewerWidget::dropEvent(QDropEvent* dropEvent)
{
	const auto items        = dropEvent->mimeData()->text().split("\n");
	const auto datasetName  = items.at(0);

	auto& configurationsModel = _dimensionsViewerPlugin->getConfigurationsModel();

	for (int channelIndex = 0; channelIndex < Configuration::noChannels; channelIndex++) {
		const auto hits = configurationsModel.match(configurationsModel.index(0, Configuration::Column::ChannelDatasetNameStart + channelIndex), Qt::DisplayRole, datasetName, -1, Qt::MatchExactly);

		if (!hits.isEmpty()) {
			configurationsModel.selectRow(hits.first().row());
			break;
		}
	}
}