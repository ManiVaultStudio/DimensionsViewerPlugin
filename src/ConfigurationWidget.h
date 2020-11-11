#pragma once

#include "ModelItemWidget.h"

#include <memory>

#include <QWidget>

class DimensionsViewerPlugin;

namespace Ui {
	class ConfigurationWidget;
}

/**
 * Configuration widget class
 *
 * @author T. Kroes
 */
class ConfigurationWidget : public ModelItemWidget
{
public: // Construction

    /**
     * Constructor
     * @param parent Parent widget
     */
	ConfigurationWidget(QWidget* parent);

public: // Inherited from ModelItemWidget

    /**
     * Sets the model index for this model item widget
     * @param modelIndex Persistent model index
     */
    void setModelIndex(const QPersistentModelIndex& modelIndex) override;

private:
	std::unique_ptr<Ui::ConfigurationWidget>    _ui;        /** UI from creator */
};