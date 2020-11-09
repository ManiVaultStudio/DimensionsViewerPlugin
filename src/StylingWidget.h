#pragma once

#include "ModelItemWidget.h"

namespace Ui {
    class StylingWidget;
}

/**
 * Styling widget class
 *
 * @author T. Kroes
 */
class StylingWidget : public ModelItemWidget
{
public: // Construction

    /**
     * Constructor
     * @param parent Parent widget
     */
    StylingWidget(QWidget* parent);

public: // Inherited from ModelItemWidget

    /**
     * Updates the UI with model indices ranging from \p begin to \p end
     * @param begin Start of model index range
     * @param end End of model index range
     * @param roles Data roles
     */
    void updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles = QVector<int>()) override;

    /**
     * Sets the model index for this model item widget
     * @param modelIndex Persistent model index
     */
    void setModelIndex(const QPersistentModelIndex& modelIndex) override;

private:
    std::unique_ptr<Ui::StylingWidget>	_ui;        /** UI from creator */
};