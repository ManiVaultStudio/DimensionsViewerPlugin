#pragma once

class Channel;

#include <QObject>
#include <QVector>

/**
 * Channels class
 *
 * @author T. Kroes
 */
class Channels : public QObject {

protected: // Construction

	Channels(QObject* parent, const QString& datasetName, const QString& dataName);

public: // Overloaded operators

    Channel* operator [](int i) const { return _channels[i]; }

public:
    Channel* getChannelByDatasetName(const QString& datasetName);
    QVector<std::uint32_t> getChannelsEnabled() const;
    std::int32_t getNoChannelsEnabled() const;
    std::int32_t getNoDisplayChannels() const;

private:
    QVector<Channel*>       _channels;

    friend class Configuration;
};