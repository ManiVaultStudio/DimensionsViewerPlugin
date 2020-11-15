#pragma once

#include "TreeItemWidget.h"

namespace Ui {
    class StylingWidget;
}

/**
 * Styling widget class
 *
 * @author T. Kroes
 */
class StylingWidget : public TreeItemWidget
{
public: // Construction

    /**
     * Constructor
     * @param parent Parent widget
     */
    StylingWidget(QWidget* parent);

public: // Inherited

    /**
     * Sets the model index for this tree item widget
     * @param modelIndex Persistent model index
     */
    void setModelIndex(const QPersistentModelIndex& modelIndex) override;

private:
    std::unique_ptr<Ui::StylingWidget>	_ui;        /** UI from creator */
};