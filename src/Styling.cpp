#include "Styling.h"
#include "Channel.h"
#include "DataItems.h"
#include "ConfigurationsModel.h"
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

Styling::Styling(TreeItem* parent) :
    TreeItem(parent, "Styling", "Styling")
{
    _children << new StringItem(this, "Line type (profile)", "Solid");
    _children << new StringItem(this, "Line type (range)", "DashDot");
    _children << new BooleanItem(this, "Render points");
    _children << new FloatItem(this, "Opacity");
    _children << new ColorItem(this, "Color");
}

void Styling::accept(Visitor* visitor) const
{
    visitor->visitStyling(this);
}

const Channel* Styling::getChannel() const
{
    return dynamic_cast<Channel*>(_parent);
}