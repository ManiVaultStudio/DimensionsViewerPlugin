#pragma once

#include "StandardItem.h"

namespace tree {

class StringList : public StandardItem
{
public:

    StringList(Item* parent, const QString& name, const QStringList& value = {});

public: // Model API

    QVariant getData(const QModelIndex& index, const int& role) const override;

    bool setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role = Qt::EditRole) override;

public: // Visitor API

    void accept(Visitor* visitor) const override;

protected:
    QStringList     _value;
};

}