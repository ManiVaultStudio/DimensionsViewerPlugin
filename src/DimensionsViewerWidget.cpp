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

	webChannel->registerObject("dimensionsViewer", &_dimensionsViewerPlugin->getDimensions());

	page()->setWebChannel(webChannel);
}