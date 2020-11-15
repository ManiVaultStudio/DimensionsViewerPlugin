#pragma once

#include "SpecSynchronizer.h"

#include <QWebEngineView>

class DimensionsViewerPlugin;

class QDragEnterEvent;
class QDropEvent;

/**
 * Viewer widget class
 *
 * UI widget class for displaying dimensions with the Vega (JS visualization)
 *
 * @author T. Kroes
 */
class ViewerWidget : public QWebEngineView
{
	Q_OBJECT

public: // Construction

    /**
     * Constructor
     * @param dimensionsViewerPlugin Pointer to an instance of the dimensions viewer plugin
     */
    ViewerWidget(DimensionsViewerPlugin* dimensionsViewerPlugin);

private: // Drag and drop

    /**
     * Invoked when an item is dragged into the widget
     * @param dragEnterEvent Drag enter event
     */
    void dragEnterEvent(QDragEnterEvent* dragEnterEvent) override;

    /**
     * Invoked when an item is dropped onto the widget
     * @param dropEvent Drop event
     */
    void dropEvent(QDropEvent* dropEvent) override;

private:
	DimensionsViewerPlugin*		_dimensionsViewerPlugin;    /** Pointer to an instance of the dimensions viewer plugin */
    QWebChannel*                _webChannel;                /** Web channel (bridge between Qt and the Vega visualization JS client) */
    SpecSynchronizer            _specSynchronizer;          /** Spec synchronizer */
};