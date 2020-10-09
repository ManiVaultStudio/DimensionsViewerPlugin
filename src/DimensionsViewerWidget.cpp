#include "DimensionsViewerWidget.h"
#include "DimensionsViewerPlugin.h"

#include <QFileInfo>
#include <QWebChannel>

DimensionsViewerWidget::DimensionsViewerWidget(DimensionsViewerPlugin* dimensionsViewerPlugin) :
	QWebEngineView(),
	_dimensionsViewerPlugin(dimensionsViewerPlugin)
{
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

	load(QUrl("qrc:DimensionsViewer.html"));

	QWebChannel* webChannel = new QWebChannel(this);

	for (auto channel : _dimensionsViewerPlugin->getChannels()) {
		webChannel->registerObject(channel->getName(), channel);
	}

	page()->setWebChannel(webChannel);
}