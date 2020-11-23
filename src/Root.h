#pragma once

#include "Item.h"

/**
 * Root tree item class
 *
 * @author Thomas Kroes
 */
class Root : public tree::Item
{
public: // Enumerations

    /** Child enumeration (each child enum item maps to a child tree item) */
    enum class Child {
        DatasetNames,
        Configurations,

        _Start  = DatasetNames,
        _End    = Configurations,
        _Count  = _End + 1
    };

    /** Children set alias */
    using Children = QSet<Child>;

    /** Maps child name to child enum */
    static QMap<QString, Child> const children;

public: // Construction
    
    /** Default constructor */
    Root();

public: // TreeItem: visitor API

    /** Accept visitor */
    void accept(tree::Visitor* visitor) const override;

    friend class ConfigurationsModel;
};