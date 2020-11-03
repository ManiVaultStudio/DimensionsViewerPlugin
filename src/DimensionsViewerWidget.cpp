#include "DimensionsViewerWidget.h"
#include "DimensionsViewerPlugin.h"

#include <QFileInfo>
#include <QWebChannel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

DimensionsViewerWidget::DimensionsViewerWidget(DimensionsViewerPlugin* dimensionsViewerPlugin) :
	QWebEngineView(),
	_dimensionsViewerPlugin(dimensionsViewerPlugin),
    _webChannel(new QWebChannel(this)),
    _specSynchronizer(dimensionsViewerPlugin)
{
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	setAcceptDrops(true);

    auto& configurationsModel = _dimensionsViewerPlugin->getConfigurationsModel();

    page()->setWebChannel(_webChannel);

    _webChannel->registerObject("specSynchronizer", &_specSynchronizer);

    load(QUrl("qrc:DimensionsViewer.html"));
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
    //TODO
	//const auto items        = dropEvent->mimeData()->text().split("\n");
	//const auto datasetName  = items.at(0);

	//auto& configurationsModel = _dimensionsViewerPlugin->getConfigurationsModel();

	//for (int channelIndex = 0; channelIndex < Configuration::noChannels; channelIndex++) {
	//	const auto hits = configurationsModel.match(configurationsModel.index(0, Configuration::Column::ChannelDatasetNameStart + channelIndex), Qt::DisplayRole, datasetName, -1, Qt::MatchExactly);

	//	if (!hits.isEmpty()) {
	//		configurationsModel.selectRow(hits.first().row());
	//		break;
	//	}
	//}
}