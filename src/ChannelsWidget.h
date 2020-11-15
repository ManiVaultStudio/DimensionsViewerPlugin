#pragma once

#include "TreeItemWidget.h"

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
class ChannelsWidget : public TreeItemWidget
{
public: // Construction

    /**
     * Constructor
     * @param parent Parent widget
     */
	ChannelsWidget(QWidget* parent);

public: // Inherited

    /**
     * Sets the model index for this model item widget
     * @param modelIndex Persistent model index
     */
    void setModelIndex(const QPersistentModelIndex& modelIndex) override;

private:
	std::unique_ptr<Ui::ChannelsWidget>		_ui;    /** UI from creator */
};