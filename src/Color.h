#pragma once

#include "Item.h"

#include <QColor>

namespace tree {

class Color : public Item
{
public:

    Color(Item* parent, const QString& name, const QColor& value = Qt::black);

public: // Model API

    QVariant getData(const QModelIndex& index, const int& role) const override;

    void setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role = Qt::EditRole) override;

public: // Visitor API

    void accept(Visitor* visitor) const override;

protected:
    QColor     _value;
};

}