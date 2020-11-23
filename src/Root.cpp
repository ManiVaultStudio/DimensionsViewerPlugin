#include "Root.h"
#include "StandardItems.h"
#include "Configurations.h"
#include "Visitor.h"

const QMap<QString, Root::Child> Root::children = {
    { "DatasetNames", Root::Child::DatasetNames },
    { "Configurations", Root::Child::Configurations }
};

Root::Root() :
    Item(nullptr, "Root", "Root")
{
    _flags.setFlag(Qt::ItemIsEnabled);

    _children << new tree::StringList(this, "DatasetNames");
    _children << new Configurations(this);
}

void Root::accept(tree::Visitor* visitor) const
{
    Q_ASSERT(visitor != nullptr);

    visitor->visitTreeItem(this);
}