#include "StandardItem.h"
#include "Visitor.h"

namespace tree {

StandardItem::StandardItem(Item* parent, const QString& type, const QString& name) :
    Item(parent, type, name)
{
}

//QVariant StandardItem::getValue(const int& role /*= Qt::EditRole*/) const
//{
//    return getData(Column::Value, role);
//}

}