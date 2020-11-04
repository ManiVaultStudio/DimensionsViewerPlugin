#include "Configurations.h"
#include "Configuration.h"
#include "DimensionsViewerPlugin.h"

#include <QMessageBox>

int Configurations::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
    return parent.isValid() ? static_cast<ModelItem*>(parent.internalPointer())->getChildCount() : getChildCount();
}

int Configurations::getColumnCount() const
{
    return 5;
}

Qt::ItemFlags Configurations::getFlags(const QModelIndex& index) const
{
    return getChild(index.row())->getFlags(index);
}

QVariant Configurations::getData(const QModelIndex& index, const int& role) const
{
    return "CONFIGURATIONS_MODEL_ITEM";
}

QModelIndexList Configurations::setData(const QModelIndex& index, const QVariant& value, const int& role)
{
    return QModelIndexList();
}

QModelIndex Configurations::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    auto childLayer = getChild(row);

    if (childLayer)
        return dimensionsViewerPlugin->getConfigurationsModel().createIndex(row, column, childLayer);

    return QModelIndex();
}

QModelIndex Configurations::parent(const QModelIndex& index) const
{
    return QModelIndex();

    /*
    if (!index.isValid())
        return QModelIndex();

    auto childLayer     = reinterpret_cast<Configuration*>(index.row());
    auto parentLayer    = childLayer ? childLayer->getParent() : nullptr;

    if (parentLayer == _root || !parentLayer)
        return QModelIndex();
    
    return createIndex(parentLayer->getChildIndex(), 0, parentLayer);*/
}

ModelItem* Configurations::getChild(const int& index) const
{
    if (index >= _configurations.size())
        return nullptr;

    return _configurations[index];
}

int Configurations::getChildCount() const
{
    return _configurations.size();
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