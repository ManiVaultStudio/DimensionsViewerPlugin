#pragma once

#include "ConfigurationsModel.h"

#include <memory>

#include <QWidget>
#include <QPersistentModelIndex>

class DimensionsViewerPlugin;

/**
 * Tree item widget base class
 *
 * @author T. Kroes
 */
class TreeItemWidget : public QWidget
{
public:

    class WidgetMapper {

    public: // Alias(es)

        /** Callback executed when data changes */
        using CallbackFunction = std::function<void(const QPersistentModelIndex& index, const bool& initialize)>;

    public: // Construction

        /** Default constructor */
        WidgetMapper() :
            _index(),
            _widget(nullptr),
            _callbackFunction()
        {
        }

        /**
         * Constructor
         * @param model Model to register to
         * @param index Model index to register to
         * @param widget Widget that responds to data change(s)
         * @param callbackFunction Method which is called when data for model index changes
         */
        WidgetMapper(QWidget* widget, CallbackFunction callbackFunction) :
            _index(QModelIndex()),
            _widget(widget),
            _callbackFunction(callbackFunction)
        {
            QObject::connect(model, &QAbstractItemModel::dataChanged, [this](const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles = QVector<int>()) {
                if (!_index.parent().isValid() || !topLeft.parent().isValid())
                    return;

                if (_index.parent() != topLeft.parent())
                    return;

                if (_index.row() != topLeft.row())
                    return;
                
                const auto column = _index.column();

                if (column >= topLeft.column() && column <= bottomRight.column()) {
                    QSignalBlocker signalBlocker(_widget);

                    _callbackFunction(_index, false);
                }
            });

            setModelIndex(QModelIndex());
        }

    public: // Model index

        /** Gets the model index */
        QPersistentModelIndex getModelIndex() const {
            return _index;
        }

        /**
         * Gets the model index
         * @param index Model index
         */
        void setModelIndex(const QPersistentModelIndex& index) {
            _index = index;

            QSignalBlocker signalBlocker(_widget);

            _callbackFunction(_index, _index == QModelIndex());
        }

    private:
        QPersistentModelIndex   _index;                 /** Model index to register to */
        QWidget*                _widget;                /** Widget that responds to data change(s) */
        CallbackFunction        _callbackFunction;      /** Method which is called when data for model index changes */

    protected:
        static QAbstractItemModel*  model;      /** Model to register to */

        friend class TreeItemWidget;
    };

public: // Construction

    /**
     * Constructor
     * @param parent Parent widget
     */
    TreeItemWidget(QWidget* parent);

protected:

    /**
     * Sets the model index for this model item widget
     * @param modelIndex Persistent model index
     */
    virtual void setModelIndex(const QPersistentModelIndex& modelIndex) = 0;

    /** TODO */
    ConfigurationsModel& getModel();

    /** TODO */
    QItemSelectionModel& getSelectionModel();

protected: // Model operations

    /**
     * Get child at \p row
     * @param row Child row index
     * @return Sibling model index at \p row
     */
    QPersistentModelIndex getChild(const std::int32_t& row) {
        return getModel().index(row, 0, _modelIndex);
    }

    /**
     * Get sibling at \p column
     * @param column Sibling column
     * @param index Model index
     * @return Sibling model index at column
     */
    QPersistentModelIndex getSiblingAtColumn(const std::int32_t& column, const QPersistentModelIndex& index) {
        return index.sibling(index.row(), column);
    }

    /**
     * Get sibling at \p column
     * @param column Sibling column
     * @return Sibling model index at column
     */
    QPersistentModelIndex getSiblingAtColumn(const std::int32_t& column) {
        return getSiblingAtColumn(column, _modelIndex);
    }

    /** Gets the model index assigned to the widget */
    QModelIndex getModelIndex() const;

    /**
     * Set model data
     * @param column Data column
     * @param value Data value
     * @param role Data role
     */
    void setData(const std::int32_t& column, const QVariant& value, const std::int32_t& role = Qt::EditRole);

protected: // Widget mappers

    /**
     * Add widget mapper
     * @param widgetMapper Widget mapper
     */
    void addWidgetMapper(const QString& name, const QSharedPointer<WidgetMapper>& widgetMapper);

    /**
     * Gets widget mapper at \p modelIndex
     * @return Widget mapper at \p modelIndex
     */
    QSharedPointer<WidgetMapper> getWidgetMapper(const QString& name);

protected:

    /**
     * Sets the pointer to the dimensions viewer plugin
     * @param Pointer to the dimensions viewer plugin
     */
    static void setDimensionsViewerPlugin(DimensionsViewerPlugin* dimensionsViewerPlugin);

protected:
    QPersistentModelIndex       _modelIndex;          /** Persistent model index */

    static DimensionsViewerPlugin*  dimensionsViewerPlugin;     /** Pointer to an instance of the dimensions viewer plugin */

private:
    QMap<QString, QSharedPointer<WidgetMapper>> _widgetMappers;   /** Widget mappers */

    friend class DimensionsViewerPlugin;
};