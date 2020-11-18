#include "Styling.h"
#include "Channel.h"
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
    TreeItem(getModel()->index(to_ul(Channel::Row::Styling), 0, parent->getModelIndex()), "Styling", "Styling", parent),
    _lineTypeProfile(LineType::Solid),
    _lineTypeRange(LineType::DashDot),
    _renderPoints(true),
    _opacity(1.0f),
    _color()
{
    setNumColumns(to_ul(Column::_Count));
}

Qt::ItemFlags Styling::getFlags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = TreeItem::getFlags(index);

    const auto column = static_cast<Column>(index.column());
    
    switch (column)
    {
        case Styling::Column::LineTypes:
        case Styling::Column::LineTypeProfile:
        case Styling::Column::LineTypeRange:
        case Styling::Column::RenderPoints:
        case Styling::Column::Opacity:
        case Styling::Column::Color:
        {
            flags |= Qt::ItemIsEditable;
            
            if (getChannel()->getData(Channel::Column::Enabled, Qt::EditRole).toBool())
                flags |= Qt::ItemIsEnabled;

            break;
        }

        default:
            break;
    }

    return flags;
}

QVariant Styling::getData(const std::int32_t& column, const std::int32_t& role) const
{
    auto data = TreeItem::getData(column, role);

    switch (role)
    {
        case Qt::EditRole:
        {
            switch (static_cast<Column>(column))
            {
                case Styling::Column::LineTypes:
                    return QStringList(lineTypes.keys());

                case Styling::Column::LineTypeProfile:
                    return static_cast<std::int32_t>(_lineTypeProfile);

                case Styling::Column::LineTypeRange:
                    return static_cast<std::int32_t>(_lineTypeRange);

                case Styling::Column::RenderPoints:
                    return _renderPoints;

                case Styling::Column::Opacity:
                    return _opacity;

                case Styling::Column::Color:
                    return QVariant::fromValue(_color);

                default:
                    break;
            }

            break;
        }

        case Qt::DisplayRole:
        {
            switch (static_cast<Column>(column))
            {
                case Styling::Column::LineTypes:
                    return getData(column, Qt::EditRole).toStringList().join(", ");

                case Styling::Column::LineTypeProfile:
                case Styling::Column::LineTypeRange:
                    return Styling::getLineTypeName(static_cast<Styling::LineType>(getData(column, Qt::EditRole).toInt()));

                case Styling::Column::RenderPoints:
                    return getData(column, Qt::EditRole).toBool() ? "on" : "off";

                case Styling::Column::Opacity:
                    return QString::number(getData(column, Qt::EditRole).toFloat(), 'f', 2);

                case Styling::Column::Color:
                    return getData(column, Qt::EditRole).value<QColor>().name();

                default:
                    break;
            }

            break;
        }

        case Qt::ToolTipRole:
        {
            const auto tooltip = [&column](const QString& value) {
                return QString("%1: %2").arg(getColumnTypeName(static_cast<Column>(column)), value);
            };

            switch (static_cast<Column>(column))
            {
                case Styling::Column::LineTypes:
                case Styling::Column::LineTypeProfile:
                case Styling::Column::LineTypeRange:
                case Styling::Column::RenderPoints:
                case Styling::Column::Opacity:
                case Styling::Column::Color:
                    return tooltip(getData(column, Qt::DisplayRole).toString());

                default:
                    break;
            }

            break;
        }

        default:
            break;
    }

    return data;
}

QVariant Styling::getData(const Column& column, const std::int32_t& role) const
{
    return getData(static_cast<std::int32_t>(column), role);
}

QModelIndexList Styling::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    QModelIndexList affectedIndices = TreeItem::setData(index, value, role);

    const auto column = static_cast<Column>(index.column());

    switch (role)
    {
        case Qt::EditRole:
        {
            switch (column)
            {
                case Styling::Column::LineTypes:
                    break;

                case Styling::Column::LineTypeProfile:
                {
                    _lineTypeProfile = static_cast<Styling::LineType>(value.toInt());

                    break;
                }

                case Styling::Column::LineTypeRange:
                {
                    _lineTypeRange = static_cast<Styling::LineType>(value.toInt());

                    break;
                }

                case Styling::Column::RenderPoints:
                {
                    _renderPoints = value.toBool();

                    break;
                }

                case Styling::Column::Opacity:
                {
                    _opacity = value.toFloat();

                    break;
                }

                case Styling::Column::Color:
                {
                    _color = value.value<QColor>();

                    break;
                }

                default:
                    break;
            }

            break;
        }

        case Qt::DisplayRole:
        {
            switch (column)
            {
                case Styling::Column::LineTypes:
                    break;

                case Styling::Column::LineTypeProfile:
                {
                    _lineTypeProfile = Styling::getLineTypeEnum(value.toString());

                    break;
                }

                case Styling::Column::LineTypeRange:
                {
                    _lineTypeRange = Styling::getLineTypeEnum(value.toString());

                    break;
                }

                case Styling::Column::RenderPoints:
                case Styling::Column::Opacity:
                case Styling::Column::Color:
                    break;

                default:
                    break;
            }

            break;
        }

        default:
            break;
    }

    return affectedIndices;
}

QModelIndexList Styling::getAffectedIndices(const QModelIndex& index) const
{
    QModelIndexList affectedIndices{ index };
    return affectedIndices;
}

void Styling::accept(Visitor* visitor) const
{
    visitor->visitStyling(this);
}

const Channel* Styling::getChannel() const
{
    return dynamic_cast<Channel*>(_parent);
}