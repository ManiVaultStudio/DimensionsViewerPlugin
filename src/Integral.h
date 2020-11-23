#pragma once

#include "StandardItem.h"

namespace tree {

class Integral : public StandardItem
{
public:

    Integral(Item* parent, const QString& name, const std::int32_t& value = 0);

public: // Model API

    QVariant getData(const QModelIndex& index, const int& role) const override;

    bool setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role = Qt::EditRole) override;

public: // Visitor API

    void accept(Visitor* visitor) const override;

protected:
    std::int32_t     _value;
};

}