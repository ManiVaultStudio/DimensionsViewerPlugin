#include "Configuration.h"
#include "ConfigurationsModel.h"
#include "DataItems.h"
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

Configuration::Configuration(TreeItem* parent, const QString& datasetName, const QString& dataName) :
    TreeItem(parent, "Configuration", datasetName)
{
    noConfigurations++;

    _children << new Channels(this, datasetName, dataName);
    _children << new IntegralItem(this, "Index", noConfigurations);
    _children << new StringItem(this, "DatasetName", datasetName);
    _children << new StringItem(this, "DataName", dataName);
    _children << new IntegralItem(this, "SelectionStamp");
}

void Configuration::accept(Visitor* visitor) const
{
    visitor->visitConfiguration(this);
}