#pragma once

#include "TreeItemWidget.h"

#include <memory>

#include <QWidget.h>

class DimensionsViewerPlugin;

namespace Ui {
	class MiscellaneousWidget;
}

/**
 * Miscellaneous settings widget class
 *
 * @author T. Kroes
 */
class MiscellaneousWidget : public TreeItemWidget
{
public: // Construction

    /**
     * Constructor
     * @param parent Parent widget
     */
    MiscellaneousWidget(QWidget* parent);

public: // Inherited

    /**
     * Sets the model index for this model item widget
     * @param modelIndex Persistent model index
     */
    void setModelIndex(const QPersistentModelIndex& modelIndex) override;

private:
	std::unique_ptr<Ui::MiscellaneousWidget>	_ui;    /** UI from creator */
};