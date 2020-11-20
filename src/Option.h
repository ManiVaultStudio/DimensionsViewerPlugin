#pragma once

#include "Item.h"

namespace tree {

class Option : public Item
{

public: // Columns and rows

    /** Child enumeration */
    enum class Column {
        Options = static_cast<std::int32_t>(Item::Column::_Count),
    };

public:

    Option(Item* parent, const QString& name, const std::int32_t& value = 0, const QStringList& options = QStringList());

public: // Model API

    QVariant getData(const QModelIndex& index, const int& role) const override;

    void setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role = Qt::EditRole) override;

public: // Visitor API

    void accept(Visitor* visitor) const override;

protected:
    std::int32_t    _value;
    QStringList     _options;
};

}