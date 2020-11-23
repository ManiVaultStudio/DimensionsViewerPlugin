#include "Configurations.h"
#include "Configuration.h"
#include "StandardItems.h"
#include "Visitor.h"

#include <QMessageBox>

const QMap<QString, Configurations::Child> Configurations::children = {
    { "DatasetNames", Configurations::Child::DatasetNames }
};

Configurations::Configurations() :
    Item(nullptr, "Configurations", "Configurations")
{
    _flags.setFlag(Qt::ItemIsEditable);
    _flags.setFlag(Qt::ItemIsEnabled);

    _children << new tree::StringList(this, "DatasetNames");
}

void Configurations::accept(tree::Visitor* visitor) const
{
    visitor->visitTreeItem(this);
}

void Configurations::add(const QString& datasetName, const QString& dataName)
{
    Q_ASSERT(Item::model != nullptr);

    const auto presentError = [](const QString& reason) {
        QMessageBox::critical(nullptr, "Unable to add configuration", reason);
    };

    try
    {
        if (datasetName.isEmpty())
            throw std::runtime_error("Dataset name is not set");

        if (dataName.isEmpty())
            throw std::runtime_error("Data name is not set");

        _children << new Configuration(this, datasetName, dataName);

        _children.last()->setModelIndex(Item::model->index(getChildCount() - 1, 0));
    }
    catch (std::exception exception)
    {
        presentError(exception.what());
    }
    catch (...)
    {
        presentError("Unhandled exception");
    }
}

Configuration* Configurations::getConfigurationByDataName(const QString& dataName)
{
    Q_ASSERT(!dataName.isEmpty());

    for (auto child : _children) {
        const auto type = child->getValue(Column::Type).toString();

        if (type == "Configuration" && child->getChild("DataName")->getValue().toString() == dataName)
            return reinterpret_cast<Configuration*>(child);
    }

    return nullptr;
}