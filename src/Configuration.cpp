#include "Configuration.h"
#include "ConfigurationsModel.h"
#include "StandardItems.h"
#include "Visitor.h"

#include <QDebug>

std::int32_t Configuration::maxNoDimensions = 100;
std::int32_t Configuration::noConfigurations = 0;

const QMap<QString, Configuration::Column> Configuration::columns = {
    { "Index", Configuration::Column::Index },
    { "Dataset name", Configuration::Column::DatasetName },
    { "Data name", Configuration::Column::DataName },
    { "Selection stamp", Configuration::Column::SelectionStamp }
};

Configuration::Configuration(Item* parent, const QString& datasetName, const QString& dataName) :
    Item(parent, "Configuration", datasetName)
{
    noConfigurations++;

    _flags.setFlag(Qt::ItemIsEditable);
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
    visitor->visitTreeItem(this);
}