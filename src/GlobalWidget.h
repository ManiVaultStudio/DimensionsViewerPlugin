#pragma once

#include "ModelItemWidget.h"

#include <memory>

#include <QWidget>

class DimensionsViewerPlugin;

namespace Ui {
	class GlobalWidget;
}

/**
 * Global settings widget class
 *
 * @author T. Kroes
 */
class GlobalWidget : public ModelItemWidget
{
public: // Construction

    /**
     * Constructor
     * @param parent Parent widget
     */
    GlobalWidget(QWidget* parent);

public: // Inherited from ModelItemWidget

    /**
     * Sets the model index for this model item widget
     * @param modelIndex Persistent model index
     */
    void setModelIndex(const QPersistentModelIndex& modelIndex) override;

private:
	std::unique_ptr<Ui::GlobalWidget>	_ui;            /** UI from creator */
};