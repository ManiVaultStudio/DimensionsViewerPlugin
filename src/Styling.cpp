#include "Styling.h"
#include "Channel.h"

#include <QDebug>

const QMap<QString, Styling::Column> Styling::columns = {
    { "Name", Styling::Column::Type },
    { "Line type (profile)", Styling::Column::LineTypeProfile },
    { "Line type (range)", Styling::Column::LineTypeRange },
    { "Opacity", Styling::Column::Opacity },
    { "Color", Styling::Column::Color }
};

const QMap<QString, Styling::LineType> Styling::lineTypes = {
    { "Solid", Styling::LineType::Solid },
    { "Dot", Styling::LineType::Dot },
    { "DashDot", Styling::LineType::DashDot }
};

Styling::Styling(ModelItem* parent) :
    ModelItem("Styling", parent),
    _lineTypeProfile(LineType::Solid),
    _lineTypeRange(LineType::DashDot),
    _opacity(1.0f),
    _color()
{
}

int Styling::columnCount() const
{
    return to_ul(Column::Count);
}

Qt::ItemFlags Styling::getFlags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = Qt::ItemIsEditable;

    const auto column = static_cast<Column>(index.column());

    switch (column)
    {
        case Column::Type:
        case Column::LineTypeProfile:
        case Column::LineTypeRange:
        case Column::Opacity:
        case Column::Color:
        {
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
    switch (role)
    {
        case Qt::EditRole: {

            switch (static_cast<Column>(column))
            {
                case Column::Type:
                    return _type;

                case Column::LineTypes:
                    return QStringList(lineTypes.keys());

                case Column::LineTypeProfile:
                    return getLineTypeName(_lineTypeProfile);

                case Column::LineTypeRange:
                    return getLineTypeName(_lineTypeRange);

                case Column::Opacity:
                    return _opacity;

                case Column::Color:
                    return QVariant::fromValue(_color);

                default:
                    break;
            }

            break;
        }

        case Qt::DisplayRole: {

            switch (static_cast<Column>(column))
            {
                case Column::Type:
                    return getData(column, Qt::EditRole);

                case Column::LineTypes:
                    return getData(column, Qt::EditRole).toStringList().join(", ");

                case Column::LineTypeProfile:
                    return getData(column, Qt::EditRole).toStringList().join(", ");

                case Column::LineTypeRange:
                    return getData(column, Qt::EditRole).toStringList().join(", ");

                case Column::Opacity:
                    return QString::number(getData(column, Qt::EditRole).toFloat(), 'f', 1);

                case Column::Color:
                    return getData(column, Qt::EditRole).value<QColor>().name();

                default:
                    break;
            }

            break;
        }

        default:
            break;
    }

    return QVariant();
}

QModelIndexList Styling::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    QModelIndexList affectedIndices{ index };

    switch (role)
    {
        case Qt::EditRole: {

            switch (static_cast<Column>(index.column()))
            {
                case Column::Type:
                    break;

                case Column::LineTypeProfile: {
                    _lineTypeProfile = getLineTypeEnum(value.toString());
                    break;
                }
                
                case Column::LineTypeRange: {
                    _lineTypeRange = getLineTypeEnum(value.toString());
                    break;
                }

                case Column::Opacity: {
                    _opacity = value.toFloat();
                    break;
                }

                case Column::Color: {
                    _color = value.value<QColor>();

                    affectedIndices << index.parent().siblingAtColumn(to_ul(Channel::Column::Color));

                    break;
                }

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