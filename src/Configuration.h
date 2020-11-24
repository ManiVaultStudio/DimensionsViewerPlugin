#pragma once

#include "Item.h"
#include "Channels.h"

#include <QStringList>
#include <QColor>
#include <QModelIndex>

class DimensionsViewerPlugin;

/**
 * Configuration tree item class
 *
 * @author T. Kroes
 */
class Configuration : public tree::Item
{

public: // Enumerations

    /** Child enumeration (each child enum item maps to a child tree item) */
    enum class Child {
        Channels,
        Index,
        DatasetName,
        DataName,
        Subsets,
        SelectionStamp,

        _Start  = Channels,
        _End    = Channels,
        _Count  = _End + 1
    };

    /** Children set alias */
    using Items = QSet<Child>;

    /** Maps child name to child enum */
    static QMap<QString, Child> const children;

protected: // Construction

	/**
	 * Constructor
	 * @param parent Parent tree item
	 * @param datasetName Name of the primary dataset
	 * @param dataName Name of the primary data
	 */
	Configuration(Item* parent, const QString& datasetName, const QString& dataName);

public: // TreeItem: visitor API

    /** Accept visitor */
    void accept(tree::Visitor* visitor) const override;

protected:
    static std::int32_t maxNoDimensions;
    static std::int32_t noConfigurations;

    friend class ConfigurationsModel;
    friend class Configurations;
    friend class Channels;
};

/** Get scoped enum in rows set to work */
inline uint qHash(Configuration::Child key, uint seed) {
    return ::qHash(static_cast<uint>(key), seed);
}