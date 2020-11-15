#pragma once

#include "TreeItem.h"
#include "Channels.h"

#include <QStringList>
#include <QColor>
#include <QModelIndex>

class DimensionsViewerPlugin;

/**
 * Configuration class
 *
 * @author T. Kroes
 */
class Configuration : public TreeItem
{
public: // Constants

    /** The number of channels in the configuration */
    static const std::int32_t noChannels = 3;

public: // Columns and rows

    /** Tree item columns */
    enum class Column {
        Index = static_cast<std::int32_t>(TreeItem::Column::_Count),
        DatasetName,
        DataName,
        SelectionStamp,

        _Start  = Index,
        _End    = SelectionStamp,
        _Count  = _End + 1
    };

    /** Maps column name to column enum and vice versa */
    static QMap<QString, Column> const columns;

    /** Get string representation of column enum */
    static QString getColumnTypeName(const Column& column) {
        return columns.key(column);
    }

    /** Get enum representation from column type name */
    static Column getColumnTypeEnum(const QString& columnName) {
        return columns[columnName];
    }

    /** Tree item rows */
    enum class Row {
        Channels,
        Miscellaneous,

        _Start  = Channels,
        _End    = Miscellaneous,
        _Count  = _End + 1
    };

public: // Construction

	/**
	 * Constructor
	 * @param parent Parent tree item
	 * @param datasetName Name of the primary dataset
	 * @param dataName Name of the primary data
	 */
	Configuration(TreeItem* parent, const QString& datasetName, const QString& dataName);

public: // TreeItem: model API

    /** Returns the number of columns in the item */
    int columnCount() const override;

    /**
     * Returns the item flags for the given model index
     * @param index Model index
     * @return Item flags for the index
     */
    Qt::ItemFlags getFlags(const QModelIndex& index) const override;

    /**
     * Get data role
     * @param column Column to fetch data from
     * @param role Data role
     * @return Data in variant form
     */
    QVariant getData(const std::int32_t& column, const std::int32_t& role) const override;

    /**
     * Sets data
     * @param index Model index
     * @param value Data value in variant form
     * @param role Data role
     * @return Model indices that are affected by the operation
     */
    QModelIndexList setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role = Qt::EditRole) override;

public: // TreeItem: hierarchy API

    /**
     * Returns a tree item node by index
     * @param index Index of the child tree item
     * @return Tree item at index
     */
    TreeItem* getChild(const int& index) const override;

    /** Returns the number of children */
    int getChildCount() const override;

    /**
     * Returns the child index
     * @param child Pointer to child tree item
     */
    int getChildIndex(TreeItem* child) const override;

public: // TreeItem: visitor API

    /** Accept visitor */
    void accept(Visitor* visitor) const override;

public: // Miscellaneous

    /** Get channels tree item */
    const Channels* getChannels() const;

protected:
    std::int32_t    _index;             /** TODO */
    QString         _datasetName;       /** TODO */
    QString         _dataName;          /** Name of the points data */
    Channels        _channels;          /** TODO */

protected:
    static std::int32_t maxNoDimensions;
    static std::int32_t noConfigurations;

    friend class ConfigurationsModel;
    friend class Configurations;
    friend class Channels;
};