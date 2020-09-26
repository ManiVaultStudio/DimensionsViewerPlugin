#include "DimensionsViewerWidget.h"
#include "DimensionsViewerPlugin.h"

#include <QFileInfo>
#include <QWebChannel>

DimensionsViewerWidget::DimensionsViewerWidget(DimensionsViewerPlugin* histogramViewerPlugin) :
	QWebEngineView(),
	_dimensionsViewerPlugin(histogramViewerPlugin)
{
	/*
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

	//connect(&_histogramViewerPlugin->histogram(), &Histogram::binsChanged, this, &HistogramViewerWidget::onBinsChanged);

	auto url = QUrl::fromLocalFile(QFileInfo("HistogramViewer.html").absoluteFilePath());

	load(url);

	QWebChannel* webChannel = new QWebChannel(this);

	webChannel->registerObject("histogramViewer", &_histogramViewerPlugin->histogram());

	page()->setWebChannel(webChannel);

	//_histogramViewerPlugin->histogram().setData();
	*/
}

/*
void HistogramViewerWidget::onBinsChanged(const QString& bins)
{
}
*/