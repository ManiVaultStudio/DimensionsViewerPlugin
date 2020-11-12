#include "Styling.h"
#include "Channel.h"

#include <QDebug>

const QMap<QString, Styling::LineType> Styling::lineTypes = {
    { "Solid", Styling::LineType::Solid },
    { "Dot", Styling::LineType::Dot },
    { "DashDot", Styling::LineType::DashDot }
};

Styling::Styling() :
    _lineTypeProfile(LineType::Solid),
    _lineTypeRange(LineType::DashDot),
    _opacity(1.0f),
    _color()
{
}

QStringList Styling::getLineTypeNames() const
{
    return QStringList(lineTypes.keys());
}