#include "Configurations.h"
#include "Configuration.h"
#include "DimensionsViewerPlugin.h"

#include <QMessageBox>

Qt::ItemFlags Configurations::getFlags(const QModelIndex& index) const
{
    return getChild(index.row())->getFlags(index);
}

QVariant Configurations::getData(const QModelIndex& index, const int& role) const
{
    return getChild(index.row())->getData(index, role);
}

QModelIndexList Configurations::setData(const QModelIndex& index, const QVariant& value, const int& role)
{
    return getChild(index.row())->setData(index, value, role);
}

ModelItem* Configurations::getChild(const int& index) const
{
    if (index < 0 || index >= _configurations.size())
        return nullptr;

    return _configurations[index];
}

int Configurations::getChildCount() const
{
    return _configurations.size();
}

int Configurations::getChildIndex(ModelItem* child) const
{
    const auto configuration = dynamic_cast<Configuration*>(child);

    if (configuration)
        return _configurations.indexOf(configuration);

    return 0;
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

        _configurations << new Configuration(this, datasetName, dataName);
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

QStringList Configurations::getNames() const
{
    QStringList names;

    for (auto configuration : _configurations)
        names << configuration->getName(Qt::EditRole).toString();

    return names;
}