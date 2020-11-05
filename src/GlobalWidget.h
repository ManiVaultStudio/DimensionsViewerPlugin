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
    void setPersistentModelIndex(const QPersistentModelIndex& modelIndex) override;

private:
	std::unique_ptr<Ui::GlobalWidget>	_ui;            /** UI from creator */
};