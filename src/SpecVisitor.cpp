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
    /*
    const auto channels = configuration->getChild(to_ul(Configuration::Row::Channels));

    channels->accept(this);
    
    QVariantMap configurationSpec;

    // Common
    configurationSpec["name"]       = configuration->getData(to_ul(TreeItem::Column::Name), Qt::EditRole).toString();
    configurationSpec["uuid"]       = configuration->getData(to_ul(TreeItem::Column::UUID), Qt::EditRole).toString();
    configurationSpec["modified"]   = configuration->getData(to_ul(TreeItem::Column::Modified), Qt::EditRole).toInt();

    // Channels
    configurationSpec["channels"]       = _children[channels->getUuid()];

    _spec = configurationSpec;
    
    _children.clear();
    */
}

void SpecVisitor::visitChannels(const Channels* channels)
{
    /*
    QVariantMap channelsSpec;

    // Common
    channelsSpec["name"]        = channels->getData(to_ul(TreeItem::Column::Name), Qt::EditRole).toString();
    channelsSpec["uuid"]        = channels->getData(to_ul(TreeItem::Column::UUID), Qt::EditRole).toString();
    channelsSpec["modified"]    = channels->getData(to_ul(TreeItem::Column::Modified), Qt::EditRole).toInt();

    for (auto child : channels->getChildren()) {
        child->accept(this);

        const auto name = child->getData(to_ul(TreeItem::Column::Name), Qt::EditRole).toString();

        channelsSpec[name] = _children[child->getUuid()];
    }

    _children[channels->getUuid()] = channelsSpec;
    */
}

void SpecVisitor::visitChannel(const ChannelItem* channel)
{
    /*
    QVariantMap channelSpec;

    channelSpec["name"]         = channel->getData(to_ul(TreeItem::Column::Name), Qt::EditRole).toString();
    channelSpec["uuid"]         = channel->getData(to_ul(TreeItem::Column::UUID), Qt::EditRole).toString();
    channelSpec["modified"]     = channel->getData(to_ul(TreeItem::Column::Modified), Qt::EditRole).toInt();

    channelSpec["enabled"]      = channel->getData(to_ul(TreeItem::Column::Enabled), Qt::EditRole).toBool();
    channelSpec["index"]        = channel->getData(to_ul(Channel::Column::Index), Qt::EditRole).toInt();
    channelSpec["datasetName"]  = channel->getData(to_ul(Channel::Column::DatasetName), Qt::EditRole).toString();

    _children[channel->getUuid()] = channelSpec;
    */
}

void SpecVisitor::visitProfile(const Profile* profile)
{
    /*
    QVariantMap profileSpec;

    profileSpec["name"]         = profile->getData(to_ul(TreeItem::Column::Name), Qt::EditRole).toString();
    profileSpec["uuid"]         = profile->getData(to_ul(TreeItem::Column::UUID), Qt::EditRole).toString();
    profileSpec["modified"]     = profile->getData(to_ul(TreeItem::Column::Modified), Qt::EditRole).toInt();

    profileSpec["profileType"]  = profile->getData(Profile::Column::ProfileType, Qt::DisplayRole).toString();
    profileSpec["rangeType"]    = profile->getData(Profile::Column::RangeType, Qt::DisplayRole).toString();
    */
}

void SpecVisitor::visitDifferential(const Differential* differential)
{
    QVariantMap differentialSpec;
}

void SpecVisitor::visitStyling(const Styling* styling)
{
    /*
    QVariantMap stylingSpec;

    stylingSpec["name"]             = styling->getData(to_ul(TreeItem::Column::Name), Qt::EditRole).toString();
    stylingSpec["uuid"]             = styling->getData(to_ul(TreeItem::Column::UUID), Qt::EditRole).toString();
    stylingSpec["modified"]         = styling->getData(to_ul(TreeItem::Column::Modified), Qt::EditRole).toInt();

    stylingSpec["lineTypeProfile"] = styling->getData(Styling::Column::LineTypeProfile, Qt::DisplayRole).toString();
    stylingSpec["lineTypeRange"]   = styling->getData(Styling::Column::LineTypeRange, Qt::DisplayRole).toString();
    stylingSpec["renderPoints"]    = styling->getData(Styling::Column::RenderPoints, Qt::EditRole).toBool();
    stylingSpec["opacity"]         = styling->getData(Styling::Column::Opacity, Qt::EditRole).toFloat();
    stylingSpec["color"]           = styling->getData(Styling::Column::Color, Qt::EditRole).value<QColor>();
    */
}

void SpecVisitor::visitBooleanItem(const BooleanItem* booleanItem)
{

}

void SpecVisitor::visitIntegralItem(const IntegralItem* integralItem)
{
}

void SpecVisitor::visitFloatItem(const FloatItem* floatItem)
{

}

void SpecVisitor::visitColorItem(const ColorItem* colorItem)
{

}

void SpecVisitor::visitStringItem(const StringItem* stringItem)
{

}

void SpecVisitor::visitStringListItem(const StringListItem* stringListItem)
{

}