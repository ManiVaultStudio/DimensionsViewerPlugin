#include "Configurations.h"
#include "Configuration.h"
#include "Visitor.h"

#include <QMessageBox>

Configurations::Configurations(Item* parent) :
    Item(parent, "Configurations", "Configurations")
{
}

void Configurations::accept(tree::Visitor* visitor) const
{
    Q_ASSERT(visitor != nullptr);

    visitor->visitTreeItem(this);
}

void Configurations::add(const QString& datasetName, const QString& dataName)
{
    Q_ASSERT(Item::model != nullptr);

    Q_ASSERT(!datasetName.isEmpty());
    Q_ASSERT(!dataName.isEmpty());

    const auto presentError = [](const QString& reason) {
        QMessageBox::critical(nullptr, "Unable to add configuration", reason);
    };

    try
    {
        _children << new Configuration(this, datasetName, dataName);

        _children.last()->setModelIndex(Item::model->index(getChildCount() - 1, 0, _modelIndex));
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
        const auto type = child->getData(Column::Type, Qt::EditRole).toString();

        if (type == "Configuration" && child->getChild("DataName")->getValue().toString() == dataName)
            return reinterpret_cast<Configuration*>(child);
    }

    return nullptr;
}