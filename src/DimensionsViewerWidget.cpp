#include "DimensionsViewerWidget.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>
#include <QFileInfo>
#include <QWebChannel>
#include <QDragEnterEvent>
#include <QDropEvent>

DimensionsViewerWidget::DimensionsViewerWidget(DimensionsViewerPlugin* dimensionsViewerPlugin) :
	QWebEngineView(),
	_dimensionsViewerPlugin(dimensionsViewerPlugin),
    _webChannel(new QWebChannel(this)),
    _specSynchronizer(dimensionsViewerPlugin)
{
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    page()->setWebChannel(_webChannel);

    _webChannel->registerObject("specSynchronizer", &_specSynchronizer);

    load(QUrl("qrc:DimensionsViewer.html"));
}

void DimensionsViewerWidget::dragEnterEvent(QDragEnterEvent* dragEnterEvent)
{
}

void DimensionsViewerWidget::dropEvent(QDropEvent* dropEvent)
{
}