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
public: // Columns and rows

    /** Tree item columns */
    enum class Column {

        /** Derived tree item columns */
        Type,                           /** Type of tree item */
        Name,                           /** Name of tree item */
        Enabled,                        /** Whether the tree item is enabled or not */
        Modified,                       /** Last modified integer stamp */
        UUID,                           /** Universal unique identifier */

        /** Configuration tree item columns */
        Index,                          /** I-th configuration */
        DatasetName,                    /** Name of the points dataset */
        DataName,                       /** Name of the points data */
        SelectionStamp,                 /** Selection stamp */

        _Start  = Index,
        _End    = SelectionStamp,
        _Count  = _End + 1
    };

    /** Columns set alias */
    using Columns = QSet<Column>;

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
        Index,
        DatasetName,
        DataName,

        _Start  = Channels,
        _End    = Channels,
        _Count  = _End + 1
    };

    /** Rows set alias */
    using Rows = QSet<Row>;

public: // Construction

	/**
	 * Constructor
	 * @param parent Parent tree item
	 * @param datasetName Name of the primary dataset
	 * @param dataName Name of the primary data
	 */
	Configuration(TreeItem* parent, const QString& datasetName, const QString& dataName);

public: // TreeItem: visitor API

    /** Accept visitor */
    void accept(Visitor* visitor) const override;

protected:
    static std::int32_t maxNoDimensions;
    static std::int32_t noConfigurations;

    friend class ConfigurationsModel;
    friend class Configurations;
    friend class Channels;
};

/** Get scoped enum in columns set to work */
inline uint qHash(Configuration::Column key, uint seed) {
    return ::qHash(static_cast<uint>(key), seed);
}

/** Get scoped enum in rows set to work */
inline uint qHash(Configuration::Row key, uint seed) {
    return ::qHash(static_cast<uint>(key), seed);
}