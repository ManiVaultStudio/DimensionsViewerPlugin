#include "Styling.h"
#include "Channel.h"
#include "StandardItems.h"
#include "Visitor.h"

#include <QDebug>

const QMap<QString, Styling::Child> Styling::children = {
    { "Line types", Styling::Child::LineTypes },
    { "Line type profile", Styling::Child::LineTypeProfile },
    { "Line type range", Styling::Child::LineTypeRange },
    { "Render points", Styling::Child::RenderPoints },
    { "Opacity", Styling::Child::Opacity },
    { "Color", Styling::Child::Color }
};

const QMap<QString, Styling::LineType> Styling::lineTypes = {
    { "Solid", Styling::LineType::Solid },
    { "Dash", Styling::LineType::Dash },
    { "Dot", Styling::LineType::Dot },
    { "DashDot", Styling::LineType::DashDot },
    { "DashDotDot", Styling::LineType::DashDotDot }
};

const QSize Styling::LineTypesModel::iconSize = QSize(24, 12);

Styling::Styling(Item* parent, const QString& name) :
    Item(parent, "Styling", name)
{
    _flags.setFlag(Qt::ItemIsEnabled);

    _children << new tree::StringList(this, "Line types", QStringList(lineTypes.keys()));
    _children << new tree::String(this, "Line type (profile)", "Solid");
    _children << new tree::String(this, "Line type (range)", "DashDot");
    _children << new tree::Boolean(this, "Render points");
    _children << new tree::Float(this, "Opacity");
    _children << new tree::Color(this, "Color");
}

void Styling::accept(tree::Visitor* visitor) const
{
    Q_ASSERT(visitor != nullptr);

    visitor->visitTreeItem(this);
}

const Channel* Styling::getChannel() const
{
    return dynamic_cast<Channel*>(_parent);
}