#pragma once

#include "TreeItem.h"

#include <QColor>

class ColorItem : public TreeItem
{
public:

    ColorItem(TreeItem* parent, const QString& name, const QColor& value = Qt::black);

public: // Model API

    Qt::ItemFlags getFlags(const QModelIndex& index) const override;

    QVariant getData(const QModelIndex& index, const int& role) const override;

    void setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role = Qt::EditRole) override;

public: // Visitor API

    void accept(Visitor* visitor) const override;

protected:
    QColor     _value;
};