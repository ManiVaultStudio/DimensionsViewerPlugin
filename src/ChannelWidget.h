#pragma once

#include "ModelItemWidget.h"

#include <memory>

#include <QWidget.h>

class DimensionsViewerPlugin;

namespace Ui {
	class ChannelWidget;
}

/**
 * Channel widget class
 *
 * @author T. Kroes
 */
class ChannelWidget : public ModelItemWidget
{
public: // Construction

    /**
     * Constructor
     * @param parent Parent widget
     */
    ChannelWidget(QWidget* parent);

public: // Inherited from ModelItemWidget

    /**
     * Sets the model index for this model item widget
     * @param modelIndex Persistent model index
     */
    void setModelIndex(const QPersistentModelIndex& modelIndex) override;

private:
	std::unique_ptr<Ui::ChannelWidget>		_ui;        /** UI from creator */
};