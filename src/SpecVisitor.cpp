#include "SpecVisitor.h"
#include "Configurations.h"
#include "Configuration.h"
#include "Channels.h"
#include "Channel.h"

#include <QDebug>

SpecVisitor::SpecVisitor() :
    _children(),
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
    
    QVariantMap configurationSpec;

    // Common
    configurationSpec["internalName"]   = configuration->getData(to_ul(TreeItem::Column::InternalName), Qt::EditRole).toString();
    configurationSpec["displayName"]    = configuration->getData(to_ul(TreeItem::Column::DisplayName), Qt::EditRole).toString();
    configurationSpec["uuid"]           = configuration->getData(to_ul(TreeItem::Column::UUID), Qt::EditRole).toString();
    configurationSpec["modified"]       = configuration->getData(to_ul(TreeItem::Column::Modified), Qt::EditRole).toInt();

    // Channels
    configurationSpec["channels"]       = _children[channels->getUuid()];

    _spec = configurationSpec;
    
    _children.clear();
}

void SpecVisitor::visitChannels(const Channels* channels)
{
    QVariantMap channelsSpec;

    // Common
    channelsSpec["internalName"]    = channels->getData(to_ul(TreeItem::Column::InternalName), Qt::EditRole).toString();
    channelsSpec["displayName"]     = channels->getData(to_ul(TreeItem::Column::DisplayName), Qt::EditRole).toString();
    channelsSpec["uuid"]            = channels->getData(to_ul(TreeItem::Column::UUID), Qt::EditRole).toString();
    channelsSpec["modified"]        = channels->getData(to_ul(TreeItem::Column::Modified), Qt::EditRole).toInt();

    for (auto child : channels->getChildren()) {
        child->accept(this);

        const auto internalName = child->getData(to_ul(TreeItem::Column::InternalName), Qt::EditRole).toString();

        channelsSpec[internalName] = _children[child->getUuid()];
    }

    _children[channels->getUuid()] = channelsSpec;
}

void SpecVisitor::visitChannel(const Channel* channel)
{
    QVariantMap channelSpec;

    // Common
    channelSpec["internalName"] = channel->getData(to_ul(TreeItem::Column::InternalName), Qt::EditRole).toString();
    channelSpec["displayName"]  = channel->getData(to_ul(TreeItem::Column::DisplayName), Qt::EditRole).toString();
    channelSpec["uuid"]         = channel->getData(to_ul(TreeItem::Column::UUID), Qt::EditRole).toString();
    channelSpec["modified"]     = channel->getData(to_ul(TreeItem::Column::Modified), Qt::EditRole).toInt();

    // General
    channelSpec["enabled"]      = channel->getData(to_ul(Channel::Column::Enabled), Qt::EditRole).toBool();
    channelSpec["index"]        = channel->getData(to_ul(Channel::Column::Index), Qt::EditRole).toInt();
    channelSpec["datasetName"]  = channel->getData(to_ul(Channel::Column::DatasetName), Qt::EditRole).toString();

    // Profile properties
    channelSpec["profileType"]  = channel->getData(to_ul(Channel::Column::ProfileType), Qt::DisplayRole).toString();
    channelSpec["rangeType"]    = channel->getData(to_ul(Channel::Column::RangeType), Qt::DisplayRole).toString();

    // Styling properties
    channelSpec["lineTypeProfile"]  = channel->getData(to_ul(Channel::Column::LineTypeProfile), Qt::DisplayRole).toString();
    channelSpec["lineTypeRange"]    = channel->getData(to_ul(Channel::Column::LineTypeRange), Qt::DisplayRole).toString();
    channelSpec["renderPoints"]     = channel->getData(to_ul(Channel::Column::RenderPoints), Qt::EditRole).toBool();
    channelSpec["opacity"]          = channel->getData(to_ul(Channel::Column::Opacity), Qt::EditRole).toFloat();
    channelSpec["color"]            = channel->getData(to_ul(Channel::Column::Color), Qt::EditRole).value<QColor>();

    _children[channel->getUuid()] = channelSpec;
}

void SpecVisitor::visitProfile(const Profile* erofile)
{

}

void SpecVisitor::visitDifferential(const Differential* differential)
{

}

void SpecVisitor::visitStyling(const Styling* styling)
{

}