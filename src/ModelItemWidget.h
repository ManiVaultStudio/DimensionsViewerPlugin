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

    /** TODO */
    QPersistentModelIndex getSiblingModelIndex(const std::int32_t& row, const std::int32_t& column = 0);

    /** TODO */
    QModelIndex getModelIndex() const;

    /** TODO */
    void setData(const std::int32_t& column, const QVariant& value);

    /** TODO */
    void reset();

public:

    /**
     * Add mapper
     * @param index Model index to respond to
     * @param mapper Callback function
     */
    void addMapper(const QModelIndex& index, std::function<void(const QModelIndex& index)> mapper);

protected:
    QPersistentModelIndex       _persistentModelIndex;    /** Persistent model index */

    QMap<QModelIndex, std::function<void(const QModelIndex& index)>> _mappers;

protected:
    static DimensionsViewerPlugin*  dimensionsViewerPlugin;     /** Pointer to an instance of the dimensions viewer plugin */

    friend class DimensionsViewerPlugin;
};