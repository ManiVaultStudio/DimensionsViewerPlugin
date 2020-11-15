#pragma once

#include <QObject>
#include <QMap>
#include <QSet>

class Configurations;
class Configuration;
class Channels;
class Channel;

/**
 * Visitor abstract base class
 *
 * @author T. Kroes
 */
class Visitor {
public:
    virtual void visitConfigurations(const Configurations* configurations) const = 0;
    virtual void visitConfiguration(const Configuration* configuration) const = 0;
    virtual void visitChannels(const Channels* channels) const = 0;
    virtual void visitChannel(const Channel* channel) const = 0;
};