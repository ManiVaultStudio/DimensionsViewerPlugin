#pragma once

#include "Item.h"
#include "Profile.h"

class Channel;
class Configuration;

#include <QObject>
#include <QVector>

/**
 * Channels tree item class
 *
 * @author T. Kroes
 */
class Channels : public tree::Item
{

public: // Enumerations

    /** Child enumeration (each child enum item maps to a child tree item) */
    enum class Child {
        Dataset,            /** Primary dataset channel */
        Subset1,            /** First subset */
        Subset2,            /** Second subset */
        Differential,       /** Compare channels */

        _Start  = Dataset,
        _End    = Differential,
        _Count  = _End + 1
    };

    /** Children set alias */
    using Children = QSet<Child>;

    /** Maps child name to child enum */
    static QMap<QString, Child> const children;

protected: // Construction

    /**
     * Constructor
     * @param parent Parent tree item
     * @param datasetName Name of the primary dataset
     * @param dataName Name of the primary data
     */
	Channels(tree::Item* parent, const QString& datasetName, const QString& dataName);

public: // TreeItem: model API

    /** Initialization after the model index has been set */
    void initialize() override;

public: // TreeItem: visitor API

    /** Accept visitor */
    void accept(tree::Visitor* visitor) const override;

public: // Getters

    /**
     * Get (filtered) channels
     * @param profileTypes Filter profile types
     * @param enabled Filter enabled/disabled channels
     * @return Filtered channels
     */
    QVector<Channel*> getFiltered(const Profile::ProfileTypes& profileTypes, bool* enabled = nullptr) const;

public: // Overloaded operators

    /**
     * Bracket operator
     * @param row Row enum
     */
    Channel* operator [](const Child& row) const {
        return reinterpret_cast<Channel*>(_children[static_cast<std::int32_t>(row)]);
    }

    friend class ConfigurationsModel;
    friend class Configuration;
    friend class Channel;
    friend class Differential;
};

inline uint qHash(Channels::Child key, uint seed) {
    return ::qHash(static_cast<uint>(key), seed);
}