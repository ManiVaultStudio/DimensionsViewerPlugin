#pragma once

#include "SpecSynchronizer.h"

#include <QWebEngineView>

class DimensionsViewerPlugin;

/**
 * Dimensions viewer widget class
 *
 * UI widget class for displaying dimensions with the Vega (JS visualization)
 *
 * @author T. Kroes
 */
class DimensionsViewerWidget : public QWebEngineView
{
	Q_OBJECT

public:
    DimensionsViewerWidget(DimensionsViewerPlugin* dimensionsViewerPlugin);

    /*
    void dragEnterEvent(QDragEnterEvent* dragEnterEvent);
    void dropEvent(QDropEvent* dropEvent) override;
    */


private:
	DimensionsViewerPlugin*		_dimensionsViewerPlugin;    /** Pointer to an instance of the dimensions viewer plugin */
    QWebChannel*                _webChannel;                /** Web channel (bridge between Qt and the Vega visualization JS client) */
    SpecSynchronizer            _specSynchronizer;          /** Spec synchronizer */
};