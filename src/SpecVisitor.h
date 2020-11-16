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

    /**
     * Visit a \p profile tree item
     * @param profile Profile tree item
     */
    void visitProfile(const Profile* erofile);

    /**
     * Visit a \p differential tree item
     * @param differential Differential tree item
     */
    void visitDifferential(const Differential* differential);

    /**
     * Visit a \p styling tree item
     * @param styling Styling tree item
     */
    void visitStyling(const Styling* styling);

    /** Gets the visualization spec */
    QVariantMap getSpec() const {
        return _spec;
    }

private:
    QMap<QUuid, QVariantMap>    _children;      /** Cached children */
    QVariantMap                 _spec;          /** Specification for use in JS visualization client (Vega) */
};