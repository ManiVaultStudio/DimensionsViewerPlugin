#include "Configurations.h"
#include "Configuration.h"
#include "Visitor.h"

#include <QMessageBox>

Configurations::Configurations() :
    Item(nullptr, "Configurations", "Configurations")
{
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