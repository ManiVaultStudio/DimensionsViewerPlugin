#include "Configuration.h"
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
    TreeItem("Configuration", datasetName, parent),
	_index(noConfigurations),
    _datasetName(datasetName),
    _dataName(dataName),
	_channels(this, datasetName, dataName)
{
    setNumColumns(to_ul(Column::_Count));

    noConfigurations++;
}

Qt::ItemFlags Configuration::getFlags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = TreeItem::getFlags(index);

    flags |= Qt::ItemIsEnabled;
    flags |= Qt::ItemIsSelectable;

    const auto column = static_cast<Column>(index.column());

    switch (column)
    {
        case Column::Index:
        case Column::DatasetName:
        case Column::DataName:
        case Column::SelectionStamp:
            break;

        default:
            break;
    }

    return flags;
}

QVariant Configuration::getData(const std::int32_t& column, const std::int32_t& role) const
{
    if (static_cast<TreeItem::Column>(column) <= TreeItem::Column::_End)
        return TreeItem::getData(column, role);

    switch (role)
    {
        case Qt::EditRole: {

            switch (static_cast<Column>(column))
            {
                case Configuration::Column::Index:
                    return _index;

                case Configuration::Column::DatasetName:
                    return _datasetName;

                case Configuration::Column::DataName:
                    return _dataName;

                default:
                    break;
            }

            break;
        }

        case Qt::DisplayRole: {

            switch (static_cast<Column>(column))
            {
                case Configuration::Column::Index:
                    return QString::number(getData(column, Qt::EditRole).toInt());

                case Configuration::Column::DatasetName:
                    return getData(column, Qt::EditRole);

                case Configuration::Column::DataName:
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

QModelIndexList Configuration::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    if (static_cast<TreeItem::Column>(index.column()) <= TreeItem::Column::_End)
        return TreeItem::setData(index, value, role);

    QModelIndexList affectedIndices{ index };

    switch (role)
    {
        case Qt::EditRole: {

            switch (static_cast<Column>(index.column()))
            {
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

TreeItem* Configuration::getChild(const int& index) const
{
    switch (static_cast<Row>(index))
    {
        case Row::Channels:
            return const_cast<Channels*>(&_channels);

        case Row::Miscellaneous:
            return nullptr;

        default:
            break;
    }

    return nullptr;
}

int Configuration::getChildCount() const
{
    return static_cast<int>(Row::_Count);
}

int Configuration::getChildIndex(TreeItem* child) const
{
    if (dynamic_cast<Channels*>(child))
        return static_cast<int>(Row::Channels);

    return 0;
}

void Configuration::accept(Visitor* visitor) const
{
    visitor->visitConfiguration(this);
}

const Channels* Configuration::getChannels() const
{
    return &_channels;
}