#pragma once

#include "TreeItemWidget.h"

#include <memory>

namespace Ui {
    class ProfileWidget;
}

/**
 * Profile tree item widget class
 *
 * @author T. Kroes
 */
class ProfileWidget : public TreeItemWidget
{
public: // Construction

    /**
     * Constructor
     * @param parent Parent widget
     */
    ProfileWidget(QWidget* parent);

public: // Inherited

    /**
     * Sets the model index for this tree item widget
     * @param modelIndex Persistent model index
     */
    void setModelIndex(const QPersistentModelIndex& modelIndex) override;

private:
    std::unique_ptr<Ui::ProfileWidget>      _ui;        /** UI from creator */
};