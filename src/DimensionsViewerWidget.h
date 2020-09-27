#pragma once

#include <QWebEngineView>

#include "Common.h"

class DimensionsViewerPlugin;

class DimensionsViewerWidget : public QWebEngineView
{
	Q_OBJECT

public:
	DimensionsViewerWidget(DimensionsViewerPlugin* dimensionsViewerPlugin);

private:
	DimensionsViewerPlugin*		_dimensionsViewerPlugin;
};