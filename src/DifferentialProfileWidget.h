#pragma once

#include "ModelItemWidget.h"

#include <memory>

#include <QWidget.h>
#include <QVector.h>

class DimensionsViewerPlugin;

namespace Ui {
	class DifferentialProfileWidget;
}

/**
 * Differential profile widget class
 *
 * @author T. Kroes
 */
class DifferentialProfileWidget : public ModelItemWidget
{
public: // Construction

    /**
     * Constructor
     * @param parent Parent widget
     */
    DifferentialProfileWidget(QWidget* parent);

public: // Inherited from ModelItemWidget

    /**
     * Sets the model index for this model item widget
     * @param modelIndex Persistent model index
     */
    void setModelIndex(const QPersistentModelIndex& modelIndex) override;

private:
	std::unique_ptr<Ui::DifferentialProfileWidget>  _ui;    /** UI from creator */
};