#pragma once

#include <QWebEngineView>

#include "Common.h"

class DimensionsViewerPlugin;

class QDragEnterEvent;
class QDropEvent;

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

public: // Construction

    /**
     * Constructor
     * @param dimensionsViewerPlugin Pointer to an instance of the dimensions viewer plugin
     */
    DimensionsViewerWidget(DimensionsViewerPlugin* dimensionsViewerPlugin);

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
};