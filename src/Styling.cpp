#include "Styling.h"

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

const QMap<Styling::Column, std::function<QVariant(Styling* styling)>> Styling::getEditRoles = {
    { Styling::Column::Name, [](Styling* styling) {
        return styling->_name;
    }},
    { Styling::Column::LineTypes, [](Styling* styling) {
        return QStringList(Styling::lineTypes.keys());
    }},
    { Styling::Column::LineTypeProfile, [](Styling* styling) {
        return Styling::getLineTypeName(styling->_lineTypeProfile);
    }},
    { Styling::Column::LineTypeRange, [](Styling* styling) {
        return Styling::getLineTypeName(styling->_lineTypeRange);
    }},
    { Styling::Column::Opacity, [](Styling* styling) {
        return styling->_opacity;
    }},
    { Styling::Column::Color, [](Styling* styling) {
        return QVariant::fromValue(styling->_color);
    }}
};

const QMap<Styling::Column, std::function<QVariant(Styling* styling)>> Styling::getDisplayRoles = {
    { Styling::Column::Name, [](Styling* styling) {
        return getEditRoles[Styling::Column::Name](styling);
    }},
    { Styling::Column::LineTypes, [](Styling* styling) {
        return getEditRoles[Styling::Column::LineTypeProfile](styling).toStringList().join(", ");
    }},
    { Styling::Column::LineTypeProfile, [](Styling* styling) {
        return getEditRoles[Styling::Column::LineTypeProfile](styling).toStringList().join(", ");
    }},
    { Styling::Column::LineTypeRange, [](Styling* styling) {
        return getEditRoles[Styling::Column::LineTypeRange](styling).toStringList().join(", ");
    }},
    { Styling::Column::Opacity, [](Styling* styling) {
        return QString::number(getEditRoles[Styling::Column::Opacity](styling).toFloat(), 'f', 1);
    }},
    { Styling::Column::Color, [](Styling* styling) {
        return getEditRoles[Styling::Column::Color](styling).value<QColor>().name();
    }}
};

const QMap<Styling::Column, std::function<QModelIndexList(Styling* styling, const QModelIndex& index, const QVariant& value)>> Styling::setEditRoles = {
    { Styling::Column::LineTypeProfile, [](Styling* styling, const QModelIndex& index, const QVariant& value) {
        styling->_lineTypeProfile = Styling::getLineTypeEnum(value.toString());
        return QModelIndexList();
    }},
    { Styling::Column::LineTypeRange, [](Styling* styling, const QModelIndex& index, const QVariant& value) {
        styling->_lineTypeRange = Styling::getLineTypeEnum(value.toString());
        return QModelIndexList();
    }},
    { Styling::Column::Opacity, [](Styling* styling, const QModelIndex& index, const QVariant& value) {
        styling->_opacity = value.toFloat();
        return QModelIndexList();
    }},
    { Styling::Column::Color, [](Styling* styling, const QModelIndex& index, const QVariant& value) {
        styling->_color = value.value<QColor>();
        return QModelIndexList();
    }}
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

QVariant Styling::getData(const QModelIndex& index, const int& role) const
{
    const auto column = index.column();

    switch (role)
    {
        case Qt::EditRole:
        {
            if (getEditRoles.contains(static_cast<Column>(column)))
                return getEditRoles[static_cast<Column>(column)](const_cast<Styling*>(this));

            break;
        }

        case Qt::DisplayRole:
        {
            if (getDisplayRoles.contains(static_cast<Column>(column)))
                return getDisplayRoles[static_cast<Column>(column)](const_cast<Styling*>(this));

            break;
        }

        default:
            break;
    }

    return QVariant();
}

QModelIndexList Styling::setData(const QModelIndex& index, const QVariant& value, const int& role)
{
    const auto column = static_cast<Column>(index.column());

    QModelIndexList affectedIndices{ index };

    switch (role)
    {
        case Qt::EditRole:
        {
            if (setEditRoles.contains(column))
                affectedIndices << setEditRoles[column](const_cast<Styling*>(this), index, value);

            break;
        }

        default:
            break;
    }

    return affectedIndices;
}