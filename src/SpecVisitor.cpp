#include "SpecVisitor.h"
#include "StandardItems.h"

#include <QDebug>

SpecVisitor::SpecVisitor() :
    _children(),
    _spec()
{
}

void SpecVisitor::visitTreeItem(const tree::Item* treeItem)
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

    /*
    QVariantMap profileSpec;

    profileSpec["name"]         = profile->getData(to_ul(TreeItem::Column::Name), Qt::EditRole).toString();
    profileSpec["uuid"]         = profile->getData(to_ul(TreeItem::Column::UUID), Qt::EditRole).toString();
    profileSpec["modified"]     = profile->getData(to_ul(TreeItem::Column::Modified), Qt::EditRole).toInt();

    profileSpec["profileType"]  = profile->getData(Profile::Column::ProfileType, Qt::DisplayRole).toString();
    profileSpec["rangeType"]    = profile->getData(Profile::Column::RangeType, Qt::DisplayRole).toString();
    */

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