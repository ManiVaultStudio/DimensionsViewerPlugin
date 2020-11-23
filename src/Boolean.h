#pragma once

#include "StandardItem.h"

namespace tree {

class Boolean : public StandardItem
{
public:

    Boolean(Item* parent, const QString& name, const bool& value = true);

public: // Model API

    QVariant getData(const QModelIndex& index, const int& role) const override;

    bool setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role = Qt::EditRole) override;

public: // Visitor API

    void accept(Visitor* visitor) const override;

protected:
    bool     _value;
};

}