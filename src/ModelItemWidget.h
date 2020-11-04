#pragma once

#include "ConfigurationsModel.h"

#include <memory>

#include <QWidget>
#include <QPersistentModelIndex>

class DimensionsViewerPlugin;

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
     * Sets the model index for this model item widget
     * @param modelIndex Persistent model index
     */
    virtual void setModelIndex(const QPersistentModelIndex& modelIndex) = 0;

    /** TODO */
    ConfigurationsModel& getConfigurationsModel();

    /** TODO */
    QItemSelectionModel& getSelectionModel();

protected:
    QPersistentModelIndex       _modelIndex;    /** TODO */

protected:
    static DimensionsViewerPlugin*  dimensionsViewerPlugin;     /** Pointer to an instance of the dimensions viewer plugin */

    friend class DimensionsViewerPlugin;
};