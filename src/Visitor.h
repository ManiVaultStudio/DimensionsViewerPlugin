#pragma once

class Configurations;
class Configuration;
class Channels;
class Channel;
class Profile;
class Differential;
class Styling;

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
    virtual void visitProfile(const Profile* profile) = 0;
    virtual void visitDifferential(const Differential* differential) = 0;
    virtual void visitStyling(const Styling* styling) = 0;
};