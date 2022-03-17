#pragma once

#include <QObject>

#include "ChannelAction.h"

class Layer : public QObject
{
public:
    Layer(QObject* parent, const QString& displayName, const ChannelAction::ProfileType& profileType = ChannelAction::ProfileType::Mean);

public:
    ChannelAction& getChannelAction() { return _channelAction; }

protected:
    ChannelAction     _channelAction;
};
