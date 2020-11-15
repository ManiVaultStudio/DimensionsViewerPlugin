#include "SpecVisitor.h"
#include "Configurations.h"
#include "Configuration.h"
#include "Channels.h"
#include "Channel.h"

#include <QDebug>

SpecVisitor::SpecVisitor() :
    _items(),
    _spec()
{
}

void SpecVisitor::visitConfigurations(const Configurations* configurations)
{
}

void SpecVisitor::visitConfiguration(const Configuration* configuration)
{
    const auto channels = configuration->getChild(to_ul(Configuration::Row::Channels));

    channels->accept(this);
    
    _spec["channels"] = _items[channels->getUuid()];
}

void SpecVisitor::visitChannels(const Channels* channels)
{
    /*auto& map = _items[channels->getUuid()];

    for (auto child : channels->getChildren()) {
        child->accept(this);

        map[child]
    }
        

    _spec["channels"] = _current;*/
}

void SpecVisitor::visitChannel(const Channel* channel)
{
    //const auto internalName = channel->getData(to_ul(Channel::Column::InternalName), Qt::EditRole).toString();

    //auto& map = _items[channel->getUuid()];

    //map["enabled"]       = channel->getData(to_ul(Channel::Column::Enabled), Qt::EditRole).toBool();
    //map["index"]         = channel->getData(to_ul(Channel::Column::Index), Qt::EditRole).toInt();
    //map["displayName"]   = channel->getData(to_ul(Channel::Column::DisplayName), Qt::EditRole).toString();
    //map["datasetName"]   = channel->getData(to_ul(Channel::Column::DatasetName), Qt::EditRole).toString();
    ////map["dimensions"]    = dimensions;
    //map["profileType"]   = channel->getData(to_ul(Channel::Column::ProfileType), Qt::EditRole).toInt();
    //map["rangeType"]     = channel->getData(to_ul(Channel::Column::RangeType), Qt::EditRole).toInt();
}