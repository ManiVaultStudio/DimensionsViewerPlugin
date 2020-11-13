#include "Styling.h"
#include "Channel.h"

#include <QDebug>

const QMap<QString, Styling::LineType> Styling::lineTypes = {
    { "Solid", Styling::LineType::Solid },
    { "Dash", Styling::LineType::Dash },
    { "Dot", Styling::LineType::Dot },
    { "DashDot", Styling::LineType::DashDot },
    { "DashDotDot", Styling::LineType::DashDotDot }
};

const QSize Styling::LineTypesModel::iconSize = QSize(24, 12);

Styling::Styling() :
    _lineTypeProfile(LineType::Solid),
    _lineTypeRange(LineType::DashDot),
    _renderPoints(true),
    _opacity(1.0f),
    _color()
{
}

QStringList Styling::getLineTypeNames() const
{
    return QStringList(lineTypes.keys());
}

Styling::LineType Styling::getLineTypeProfile() const
{
    return _lineTypeProfile;
}

void Styling::setLineTypeProfile(const LineType& lineTypeProfile)
{
    _lineTypeProfile = lineTypeProfile;
}

Styling::LineType Styling::getLineTypeRange() const
{
    return _lineTypeRange;
}

void Styling::setLineTypeRange(const LineType& lineTypeRange)
{
    _lineTypeRange = lineTypeRange;
}

bool Styling::getRenderPoints() const
{
    return _renderPoints;
}

void Styling::setRenderPoints(const bool& renderPoints)
{
    _renderPoints = renderPoints;
}

float Styling::getOpacity() const
{
    return _opacity;
}

void Styling::setOpacity(const float& opacity)
{
    _opacity = opacity;
}

QColor Styling::getColor() const
{
    return _color;
}

void Styling::setColor(const QColor& color)
{
    _color = color;
}