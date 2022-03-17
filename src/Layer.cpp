#include "Layer.h"

Layer::Layer(QObject* parent, const QString& displayName, const ChannelAction::ProfileType& profileType /*= ChannelConfigurationAction::ProfileType::Mean*/) :
    QObject(parent),
    _channelAction(*this, displayName, profileType)
{
}
