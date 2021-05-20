#include "DimensionsViewerWidget.h"
#include "DimensionsViewerPlugin.h"

#include <QFileInfo>
#include <QWebChannel>

DimensionsViewerWidget::DimensionsViewerWidget(DimensionsViewerPlugin* dimensionsViewerPlugin) :
	QWebEngineView(),
	_dimensionsViewerPlugin(dimensionsViewerPlugin),
    //_webChannel(new QWebChannel(this)),
    _specSynchronizer(dimensionsViewerPlugin)
{
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	setAcceptDrops(true);

    /*
    auto& configurationsModel = _dimensionsViewerPlugin->getConfigurationsModel();

    page()->setWebChannel(_webChannel);

    _webChannel->registerObject("specSynchronizer", &_specSynchronizer);

    load(QUrl("qrc:DimensionsViewer.html"));
    */
}