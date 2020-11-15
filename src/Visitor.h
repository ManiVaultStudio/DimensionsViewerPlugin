#pragma once

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
    virtual void visitConfigurations(const Configurations* configurations) = 0;
    virtual void visitConfiguration(const Configuration* configuration) = 0;
    virtual void visitChannels(const Channels* channels) = 0;
    virtual void visitChannel(const Channel* channel) = 0;
};