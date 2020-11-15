#include "Configurations.h"
#include "Configuration.h"
#include "Visitor.h"

#include <QMessageBox>

const QMap<QString, Configurations::Column> Configurations::columns = {
    { "Type", Configurations::Column::Type }
};

Configurations::Configurations() :
    TreeItem("Configurations"),
    _configurations()
{
}

int Configurations::columnCount() const 
{
    return to_ul(Column::Count);
}

Qt::ItemFlags Configurations::getFlags(const QModelIndex& index) const
{
    return getChild(index.row())->getFlags(index);
}

QVariant Configurations::getData(const std::int32_t& column, const std::int32_t& role) const
{
    switch (role)
    {
        case Qt::EditRole: {

            switch (static_cast<Column>(column))
            {
                case Configurations::Column::Type:
                    return _type;

                default:
                    break;
            }

            break;
        }

        case Qt::DisplayRole: {

            switch (static_cast<Column>(column))
            {
                case Configurations::Column::Type:
                    return getData(column, Qt::EditRole);

                default:
                    break;
            }

            break;
        }

        default:
            break;
    }

    return QVariant();
}

QModelIndexList Configurations::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    QModelIndexList affectedIndices{ index };

    switch (role)
    {
        case Qt::EditRole: {

            switch (static_cast<Column>(index.column()))
            {
                case Configurations::Column::Type:
                    break;

                default:
                    break;
            }

            break;
        }

        default:
            break;
    }

    return affectedIndices;
}

TreeItem* Configurations::getChild(const int& index) const
{
    if (index < 0 || index >= _configurations.size())
        return nullptr;

    return _configurations.at(index);
}

int Configurations::getChildCount() const
{
    return _configurations.size();
}

int Configurations::getChildIndex(TreeItem* child) const
{
    const auto configuration = dynamic_cast<Configuration*>(child);

    if (configuration)
        return _configurations.indexOf(configuration);

    return 0;
}

void Configurations::accept(Visitor* visitor) const
{
    TreeItem::accept(visitor);

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