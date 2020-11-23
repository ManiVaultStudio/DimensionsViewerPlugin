#pragma once

#include "Item.h"

namespace tree {

class StandardItem : public Item
{
public:

    StandardItem(Item* parent, const QString& type, const QString& name);

public: // Convenience functions

    //QVariant getValue(const int& role = Qt::EditRole) const;
};

}