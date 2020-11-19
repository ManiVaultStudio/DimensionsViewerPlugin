#pragma once

#include "TreeItem.h"
#include "Profile.h"

class ChannelItem;
class Configuration;

#include <QObject>
#include <QVector>

/**
 * Channels tree item class
 *
 * @author T. Kroes
 */
class Channels : public TreeItem {

public: // Columns and rows

    /** Tree item rows */
    enum class Row {
        Dataset,                        /** Primary dataset channel */
        Subset1,                        /** First subset */
        Subset2,                        /** Second subset */
        Differential,                   /** Compare channels */

        _Start  = Dataset,
        _End    = Differential,
        _Count  = _End + 1
    };

    /** Rows set alias */
    using Rows = QSet<Row>;

    /** Maps row name to row enum */
    static QMap<QString, Row> const rows;

    /** Get row name from row enum */
    static QString getRowTypeName(const Row& row) {
        return rows.key(row);
    }

    /** Get row enum from row name */
    static Row getRowTypeEnum(const QString& rowName) {
        return rows[rowName];
    }

protected: // Construction

    /**
     * Constructor
     * @param parent Parent tree item
     * @param datasetName Name of the primary dataset
     * @param dataName Name of the primary data
     */
	Channels(TreeItem* parent, const QString& datasetName, const QString& dataName);

public: // TreeItem: visitor API

    /** Accept visitor */
    void accept(Visitor* visitor) const override;

public: // Getters

    /**
     * Get (filtered) channels
     * @param profileTypes Filter profile types
     * @param enabled Filter enabled/disabled channels
     * @return Filtered channels
     */
    QVector<ChannelItem*> getFiltered(const Profile::ProfileTypes& profileTypes, bool* enabled = nullptr) const;

public: // Overloaded operators

    /**
     * Bracket operator
     * @param row Row enum
     */
    ChannelItem* operator [](const Row& row) const {
        return reinterpret_cast<ChannelItem*>(_children[static_cast<std::int32_t>(row)]);
    }

    friend class ConfigurationsModel;
    friend class Configuration;
    friend class ChannelItem;
    friend class Differential;
};

inline uint qHash(Channels::Row key, uint seed) {
    return ::qHash(static_cast<uint>(key), seed);
}