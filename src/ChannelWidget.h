#pragma once

#include "TreeItemWidget.h"

#include <memory>

namespace Ui {
	class ChannelWidget;
}

/**
 * Channel tree item widget class
 *
 * @author T. Kroes
 */
class ChannelWidget : public TreeItemWidget
{
public: // Construction

    /**
     * Constructor
     * @param parent Parent widget
     */
    ChannelWidget(QWidget* parent);

public: // Inherited

    /**
     * Sets the model index for this tree item widget
     * @param modelIndex Persistent model index
     */
    void setModelIndex(const QPersistentModelIndex& modelIndex) override;

private:
	std::unique_ptr<Ui::ChannelWidget>		_ui;        /** UI from creator */
};