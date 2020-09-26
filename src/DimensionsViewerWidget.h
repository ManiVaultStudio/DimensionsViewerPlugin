#pragma once

#include <QWebEngineView>

#include "Common.h"

class DimensionsViewerPlugin;

class DimensionsViewerWidget : public QWebEngineView
{
	Q_OBJECT

public:
	DimensionsViewerWidget(DimensionsViewerPlugin* histogramViewerPlugin);

private:
	//void onBinsChanged(const QString& bins);

private:
	DimensionsViewerPlugin*		_dimensionsViewerPlugin;
};