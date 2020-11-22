#pragma once

#include "Item.h"

class Configuration;

/**
 * Tree item interface class
 *
 * @author Thomas Kroes
 */
class Configurations : public tree::Item
{
public: // Enumerations

    /** Child enumeration (each child enum item maps to a child tree item) */
    enum class Child {
        DatasetNames,

        _Start  = DatasetNames,
        _End    = DatasetNames,
        _Count  = _End + 1
    };

    /** Children set alias */
    using Children = QSet<Child>;

    /** Maps child name to child enum */
    static QMap<QString, Child> const children;

protected: // Construction

    /** Default constructor */
    Configurations();

public: // TreeItem: visitor API

    /** Accept visitor */
    void accept(tree::Visitor* visitor) const override;

public: // Miscellaneous

    /**
     * Add a configuration
     * @param datasetName Name of the points dataset
     * @param dataName Name of the points data
     */
    void add(const QString& datasetName, const QString& dataName);

    friend class ConfigurationsModel;
};