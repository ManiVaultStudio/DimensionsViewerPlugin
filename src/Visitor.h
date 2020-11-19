#pragma once

class Configurations;
class Configuration;
class Channels;
class ChannelItem;
class Profile;
class Differential;
class Styling;
class BooleanItem;
class IntegralItem;
class FloatItem;
class ColorItem;
class StringItem;
class StringListItem;

/**
 * Visitor abstract base class
 *
 * @author T. Kroes
 */
class Visitor {
public:
    virtual void visitConfigurations(const Configurations* configurations) = 0;
    virtual void visitConfiguration(const Configuration* configuration) = 0;
    virtual void visitChannels(const Channels* channels) = 0;
    virtual void visitChannel(const ChannelItem* channel) = 0;
    virtual void visitProfile(const Profile* profile) = 0;
    virtual void visitDifferential(const Differential* differential) = 0;
    virtual void visitStyling(const Styling* styling) = 0;
    virtual void visitBooleanItem(const BooleanItem* booleanItem) = 0;
    virtual void visitIntegralItem(const IntegralItem* integralItem) = 0;
    virtual void visitFloatItem(const FloatItem* floatItem) = 0;
    virtual void visitColorItem(const ColorItem* colorItem) = 0;
    virtual void visitStringItem(const StringItem* stringItem) = 0;
    virtual void visitStringListItem(const StringListItem* stringListItem) = 0;
};