#pragma once

#include "Item.h"

namespace tree {

class Boolean : public Item
{
public:

    Boolean(Item* parent, const QString& name, const bool& value = true);

public: // Model API

    Qt::ItemFlags getFlags(const QModelIndex& index) const override;

    QVariant getData(const QModelIndex& index, const int& role) const override;

    void setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role = Qt::EditRole) override;

public: // Visitor API

    void accept(Visitor* visitor) const override;

protected:
    bool     _value;
};

}