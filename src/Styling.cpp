#include "Styling.h"
#include "Channel.h"
#include "StandardItems.h"
#include "Visitor.h"

#include <QDebug>

const QMap<QString, Styling::Column> Styling::columns = {
    { "Line types", Styling::Column::LineTypes },
    { "Line type profile", Styling::Column::LineTypeProfile },
    { "Line type range", Styling::Column::LineTypeRange },
    { "Render points", Styling::Column::RenderPoints },
    { "Opacity", Styling::Column::Opacity },
    { "Color", Styling::Column::Color }
};

const QMap<QString, Styling::LineType> Styling::lineTypes = {
    { "Solid", Styling::LineType::Solid },
    { "Dash", Styling::LineType::Dash },
    { "Dot", Styling::LineType::Dot },
    { "DashDot", Styling::LineType::DashDot },
    { "DashDotDot", Styling::LineType::DashDotDot }
};

const QSize Styling::LineTypesModel::iconSize = QSize(24, 12);

Styling::Styling(Item* parent) :
    Item(parent, "Styling", "Styling")
{
    _flags.setFlag(Qt::ItemIsEditable);
    _flags.setFlag(Qt::ItemIsEnabled);

    _children << new tree::String(this, "Line type (profile)", "Solid");
    _children << new tree::String(this, "Line type (range)", "DashDot");
    _children << new tree::Boolean(this, "Render points");
    _children << new tree::Float(this, "Opacity");
    _children << new tree::Color(this, "Color");
}

void Styling::accept(tree::Visitor* visitor) const
{
    visitor->visitTreeItem(this);
}

const Channel* Styling::getChannel() const
{
    return dynamic_cast<Channel*>(_parent);
}