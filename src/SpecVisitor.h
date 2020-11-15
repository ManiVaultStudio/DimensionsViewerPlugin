#pragma once

#include "Visitor.h"

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

    /**
     * Visit a \p configurations tree item
     * @param configurations Configurations tree item
     */
    void visitConfigurations(const Configurations* configurations) const override;

    /**
     * Visit a \p configuration tree item
     * @param configuration Configuration tree item
     */
    void visitConfiguration(const Configuration* configuration) const override;

    /**
     * Visit a \p channels tree item
     * @param channels Channels tree item
     */
    void visitChannels(const Channels* channels) const override;

    /**
     * Visit a \p channel tree item
     * @param channel Channel tree item
     */
    void visitChannel(const Channel* channel) const override;
};