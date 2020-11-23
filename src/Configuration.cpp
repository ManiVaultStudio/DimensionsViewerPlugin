#include "Configuration.h"
#include "StandardItems.h"
#include "Visitor.h"

#include <QDebug>

std::int32_t Configuration::maxNoDimensions = 100;
std::int32_t Configuration::noConfigurations = 0;

const QMap<QString, Configuration::Child> Configuration::children = {
    { "Channels", Configuration::Child::Channels },
    { "Index", Configuration::Child::Index },
    { "Dataset name", Configuration::Child::DatasetName },
    { "Data name", Configuration::Child::DataName },
    { "Selection stamp", Configuration::Child::SelectionStamp }
};

Configuration::Configuration(Item* parent, const QString& datasetName, const QString& dataName) :
    Item(parent, "Configuration", "Configuration")
{
    noConfigurations++;

    _flags.setFlag(Qt::ItemIsEnabled);
    _flags.setFlag(Qt::ItemIsSelectable);

    _children << new Channels(this, datasetName, dataName);
    _children << new tree::Integral(this, "Index", noConfigurations);
    _children << new tree::String(this, "DatasetName", datasetName);
    _children << new tree::String(this, "DataName", dataName);
    _children << new tree::Integral(this, "SelectionStamp");
}

void Configuration::accept(tree::Visitor* visitor) const
{
    Q_ASSERT(visitor != nullptr);

    visitor->visitTreeItem(this);
}