#pragma once

#include "Visitor.h"

#include <QVariantMap>

class Configurations;
class Configuration;
class Channels;
class ChannelItem;

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
    void visitChannel(const ChannelItem* channel) override;

    /**
     * Visit a \p profile tree item
     * @param profile Profile tree item
     */
    void visitProfile(const Profile* profile) override;

    /**
     * Visit a \p differential tree item
     * @param differential Differential tree item
     */
    void visitDifferential(const Differential* differential) override;

    /**
     * Visit a \p styling tree item
     * @param styling Styling tree item
     */
    void visitStyling(const Styling* styling) override;

    /**
     * Visit a \p booleanItem
     * @param booleanItem Boolean tree item
     */
    void visitBooleanItem(const BooleanItem* booleanItem) override;

    /**
     * Visit an \p integralItem
     * @param integralItem Integral item
     */
    void visitIntegralItem(const IntegralItem* integralItem) override;

    /**
     * Visit a \p floatItem
     * @param floatItem Float item
     */
    void visitFloatItem(const FloatItem* floatItem) override;

    /**
     * Visit a \p colorItem
     * @param colorItem Color item
     */
    void visitColorItem(const ColorItem* colorItem) override;

    /**
     * Visit a \p stringItem
     * @param stringItem String item
     */
    void visitStringItem(const StringItem* stringItem) override;

    /**
     * Visit an \p stringListItem
     * @param stringListItem String list item
     */
    void visitStringListItem(const StringListItem* stringListItem) override;

    /** Gets the visualization spec */
    QVariantMap getSpec() const {
        return _spec;
    }

private:
    QMap<QUuid, QVariantMap>    _children;      /** Cached children */
    QVariantMap                 _spec;          /** Specification for use in JS visualization client (Vega) */
};