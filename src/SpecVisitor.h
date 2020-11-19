#pragma once

#include "Visitor.h"

#include <QVariantMap>

class Configurations;
class Configuration;
class Channels;
class Channel;

// Tree namespace spoofing
namespace tree {
    class Boolean;
    class Integral;
    class Float;
    class Color;
    class String;
    class StringList;
}

/**
 * Spec visitor class
 *
 * @author T. Kroes
 */
class SpecVisitor : public tree::Visitor{
public:

    /** Default constructor */
    SpecVisitor();

    /**
     * Visit a \p tree item
     * @param treeItem Tree item to visit
     */
    void visitTreeItem(const tree::Item* treeItem) override;

    /** Gets the visualization spec */
    QVariantMap getSpec() const {
        return _spec;
    }

private:
    QMap<QUuid, QVariantMap>    _children;      /** Cached children */
    QVariantMap                 _spec;          /** Specification for use in JS visualization client (Vega) */
};