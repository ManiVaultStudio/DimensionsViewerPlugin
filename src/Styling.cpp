#include "Styling.h"
#include "Channel.h"

#include <QDebug>

const QMap<QString, Styling::Column> Styling::columns = {
    { "Name", Styling::Column::Name },
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
        case Column::Name:
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
                case Styling::Column::Name:
                    return _name;

                case Styling::Column::LineTypes:
                    return QStringList(Styling::lineTypes.keys());

                case Styling::Column::LineTypeProfile:
                    return Styling::getLineTypeName(_lineTypeProfile);

                case Styling::Column::LineTypeRange:
                    return Styling::getLineTypeName(_lineTypeRange);

                case Styling::Column::Opacity:
                    return _opacity;

                case Styling::Column::Color:
                    return QVariant::fromValue(_color);

                default:
                    break;
            }

            break;
        }

        case Qt::DisplayRole: {

            switch (static_cast<Column>(column))
            {
                case Styling::Column::Name:
                    return getData(column, Qt::EditRole);

                case Styling::Column::LineTypes:
                    return getData(column, Qt::EditRole).toStringList().join(", ");

                case Styling::Column::LineTypeProfile:
                    return getData(column, Qt::EditRole).toStringList().join(", ");

                case Styling::Column::LineTypeRange:
                    return getData(column, Qt::EditRole).toStringList().join(", ");

                case Styling::Column::Opacity:
                    return QString::number(getData(column, Qt::EditRole).toFloat(), 'f', 1);

                case Styling::Column::Color:
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
                case Styling::Column::LineTypeProfile: {
                    _lineTypeProfile = Styling::getLineTypeEnum(value.toString());
                    break;
                }
                
                case Styling::Column::LineTypeRange: {
                    _lineTypeRange = Styling::getLineTypeEnum(value.toString());
                    break;
                }

                case Styling::Column::Opacity: {
                    _opacity = value.toFloat();
                    break;
                }

                case Styling::Column::Color: {
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