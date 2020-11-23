#pragma once

#include "Item.h"

class Configuration;

/**
 * Configurations tree item class
 *
 * @author Thomas Kroes
 */
class Configurations : public tree::Item
{
protected: // Construction

    /**
     * Constructor
     * @param parent Parent tree item
     */
    Configurations(Item* parent);

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

    /**
     * Get configuration by \p dataName
     * @param dataName Name of the points data
     * @return Configuration
     */
    Configuration* getConfigurationByDataName(const QString& dataName);

    friend class Root;
};