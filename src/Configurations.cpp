#include "Configurations.h"
#include "Configuration.h"
#include "DimensionsViewerPlugin.h"

#include <QMessageBox>

const QMap<QString, Configurations::Column> Configurations::columns = {
    { "Name", Configurations::Column::Name }
};

const QMap<Configurations::Column, std::function<QVariant(Configurations* configurations)>> Configurations::getEditRoles = {
    { Configurations::Column::Name, [](Configurations* configurations) {
        return configurations->_name;
    }}
};

const QMap<Configurations::Column, std::function<QVariant(Configurations* configurations)>> Configurations::getDisplayRoles = {
    { Configurations::Column::Name, [](Configurations* configurations) {
        return getEditRoles[Configurations::Column::Name](configurations);
    }}
};

const QMap<Configurations::Column, std::function<QModelIndexList(Configurations* configurations, const QVariant& value, const QModelIndex& index)>> Configurations::setEditRoles = {
};

Configurations::Configurations() :
    ModelItem("Configurations"),
    _configurations()
{
}

int Configurations::columnCount() const 
{
    return ModelItem::maxNoColumns;
}

Qt::ItemFlags Configurations::getFlags(const QModelIndex& index) const
{
    return getChild(index.row())->getFlags(index);
}

QVariant Configurations::getData(const QModelIndex& index, const int& role) const
{
    const auto column = index.column();

    switch (role)
    {
        case Qt::EditRole:
        {
            if (getEditRoles.contains(static_cast<Column>(column)))
                return getEditRoles[static_cast<Column>(column)](const_cast<Configurations*>(this));

            break;
        }

        case Qt::DisplayRole:
        {
            if (getDisplayRoles.contains(static_cast<Column>(column)))
                return getDisplayRoles[static_cast<Column>(column)](const_cast<Configurations*>(this));

            break;
        }

        default:
            break;
    }

    return QVariant();
}

QModelIndexList Configurations::setData(const QModelIndex& index, const QVariant& value, const int& role)
{
    //if (index.parent().isValid())
    //    return getChild(index.row())->setData(index, value, role);

    const auto column = static_cast<Column>(index.column());

    QModelIndexList affectedIndices{ index };

    switch (role)
    {
        case Qt::EditRole:
        {
            if (setEditRoles.contains(column))
                affectedIndices << setEditRoles[column](const_cast<Configurations*>(this), value, index);

            break;
        }

        default:
            break;
    }

    return affectedIndices;
}

ModelItem* Configurations::getChild(const int& index) const
{
    if (index < 0 || index >= _configurations.size())
        return nullptr;

    return _configurations.at(index);
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