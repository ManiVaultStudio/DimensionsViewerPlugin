#pragma once

#include "TreeItemWidget.h"

#include <memory>

namespace Ui {
	class ConfigurationWidget;
}

/**
 * Configuration tree item widget class
 *
 * @author T. Kroes
 */
class ConfigurationWidget : public TreeItemWidget
{
public: // Construction

    /**
     * Constructor
     * @param parent Parent widget
     */
	ConfigurationWidget(QWidget* parent);

public: // Inherited

    /**
     * Sets the model index for this tree item widget
     * @param modelIndex Persistent model index
     */
    void setModelIndex(const QPersistentModelIndex& modelIndex) override;

private:
	std::unique_ptr<Ui::ConfigurationWidget>    _ui;        /** UI from creator */
};