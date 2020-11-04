#pragma once

#include <memory>

#include <QWidget>
#include <QPersistentModelIndex>

class DimensionsViewerPlugin;
class ConfigurationsModel;

/**
 * Model item widget class
 *
 * TODO
 *
 * @author T. Kroes
 */
class ModelItemWidget : public QWidget
{
public: // Construction

    /**
     * Constructor
     * @param parent Parent widget
     */
    ModelItemWidget(QWidget* parent);

protected:

	/**
	 * Updates the UI with model indices ranging from \p begin to \p end
	 * @param begin Start of model index range
	 * @param end End of model index range
	 * @param roles Data roles
	 */
	virtual void updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles = QVector<int>()) = 0;

    /**
     * TODO
     */
    void setModelIndex(const QPersistentModelIndex& modelIndex);

private:
    QPersistentModelIndex       _modelIndex;    /** TODO */

protected:
    static DimensionsViewerPlugin*  dimensionsViewerPlugin;     /** Pointer to an instance of the dimensions viewer plugin */

    friend class DimensionsViewerPlugin;
};