#pragma once

#include <memory>

#include <QWidget.h>

class DimensionsViewerPlugin;

namespace Ui {
	class ChannelsWidget;
}

/**
 * Channels container widget class
 *
 * @author T. Kroes
 */
class ChannelsWidget : public QWidget
{
	Q_OBJECT

public: // Construction

    /**
     * Constructor
     * @param parent Parent widget
     */
	ChannelsWidget(QWidget* parent);

private:
	std::unique_ptr<Ui::ChannelsWidget>		_ui;    /** UI from creator */

protected:
    static DimensionsViewerPlugin*  dimensionsViewerPlugin;         /** Pointer to an instance of the dimensions viewer plugin */

    friend class DimensionsViewerPlugin;
};