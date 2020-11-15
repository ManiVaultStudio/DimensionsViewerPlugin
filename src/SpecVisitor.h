#pragma once

#include "Visitor.h"

#include <QVariantMap>

class Configurations;
class Configuration;
class Channels;
class Channel;

/**
 * Spec visitor class
 *
 * @author T. Kroes
 */
class SpecVisitor : public Visitor {
public:

    /** Default constructor */
    SpecVisitor();

    /**
     * Visit a \p configurations tree item
     * @param configurations Configurations tree item
     */
    void visitConfigurations(const Configurations* configurations) override;

    /**
     * Visit a \p configuration tree item
     * @param configuration Configuration tree item
     */
    void visitConfiguration(const Configuration* configuration) override;

    /**
     * Visit a \p channels tree item
     * @param channels Channels tree item
     */
    void visitChannels(const Channels* channels) override;

    /**
     * Visit a \p channel tree item
     * @param channel Channel tree item
     */
    void visitChannel(const Channel* channel) override;

    /** Gets the visualization spec */
    QVariantMap getSpec() const {
        return _spec;
    }

private:
    QMap<QUuid, QVariantMap>    _items;     /**  */
    QVariantMap                 _spec;      /** Specification for use in JS visualization client (Vega) */
};