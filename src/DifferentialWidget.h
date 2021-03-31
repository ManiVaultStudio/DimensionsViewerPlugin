#pragma once

#include "TreeItemWidget.h"

#include <memory>

namespace Ui {
    class DifferentialWidget;
}

/**
 * Differential tree item widget class
 *
 * @author T. Kroes
 */
class DifferentialWidget : public TreeItemWidget
{
public: // Construction

    /**
     * Constructor
     * @param parent Parent widget
     */
    DifferentialWidget(QWidget* parent);

public: // Inherited

    /**
     * Sets the model index for this tree item widget
     * @param modelIndex Persistent model index
     */
    void setModelIndex(const QPersistentModelIndex& modelIndex) override;

private:
    std::unique_ptr<Ui::DifferentialWidget>     _ui;        /** UI from creator */
};