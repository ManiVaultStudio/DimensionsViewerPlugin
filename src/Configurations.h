#pragma once

#include "TreeItem.h"

class Configuration;

/**
 * Tree item interface class
 *
 * @author Thomas Kroes
 */
class Configurations : public TreeItem
{
protected: // Construction

    /** Default constructor */
    Configurations();

public: // TreeItem: visitor API

    /** Accept visitor */
    void accept(Visitor* visitor) const override;

public: // Miscellaneous

    /**
     * Add a configuration
     * @param datasetName Name of the points dataset
     * @param dataName Name of the points data
     */
    void add(const QString& datasetName, const QString& dataName);

    friend class ConfigurationsModel;
};