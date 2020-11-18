#include "Configurations.h"
#include "Configuration.h"
#include "ConfigurationsModel.h"
#include "Visitor.h"

#include <QMessageBox>

Configurations::Configurations() :
    TreeItem(nullptr, "Configurations", "Configurations")
{
}

void Configurations::accept(Visitor* visitor) const
{
    visitor->visitConfigurations(this);
}

void Configurations::add(const QString& datasetName, const QString& dataName)
{
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

        _children.last()->setModelIndex(getModel()->index(getChildCount() - 1, 0));
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