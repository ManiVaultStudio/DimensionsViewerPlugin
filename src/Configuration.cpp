#include "Configuration.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>

std::int32_t Configuration::maxNoDimensions = 100;
std::int32_t Configuration::noConfigurations = 0;

const QMap<QString, Configuration::Column> Configuration::columns = {
    { "Type", Configuration::Column::Type },
    { "Index", Configuration::Column::Index },
    { "Dataset name", Configuration::Column::DatasetName },
    { "Data name", Configuration::Column::DataName },
    { "Selection stamp", Configuration::Column::SelectionStamp }
};

Configuration::Configuration(ModelItem* parent, const QString& datasetName, const QString& dataName) :
    ModelItem("Configuration", parent),
	_index(noConfigurations),
    _datasetName(datasetName),
    _dataName(dataName),
	_channels(this, datasetName, dataName),
    _spec()
{
    noConfigurations++;

    _spec["modified"] = 0;


}

int Configuration::columnCount() const 
{
    return to_ul(Column::_Count);
}

Qt::ItemFlags Configuration::getFlags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    const auto column = static_cast<Column>(index.column());

    switch (column)
    {
        case Column::Type:
        case Column::Index:
        case Column::DatasetName:
        case Column::DataName:
        case Column::SelectionStamp:
        {
            flags |= Qt::ItemIsEnabled;

            break;
        }

        default:
            break;
    }

    return flags;
}

QVariant Configuration::getData(const std::int32_t& column, const std::int32_t& role) const
{
    switch (role)
    {
        case Qt::EditRole: {

            switch (static_cast<Column>(column))
            {
                case Configuration::Column::Type:
                    return _type;

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
                case Configuration::Column::Type:
                    return getData(column, Qt::EditRole);

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
    QModelIndexList affectedIndices{ index };

    switch (role)
    {
        case Qt::EditRole: {

            switch (static_cast<Column>(index.column()))
            {
                case Configuration::Column::Type:
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

ModelItem* Configuration::getChild(const int& index) const
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
    return static_cast<int>(Row::_End);
}

int Configuration::getChildIndex(ModelItem* child) const
{
    if (dynamic_cast<Channels*>(child))
        return static_cast<int>(Row::Channels);

    return 0;
}

const Channels* Configuration::getChannels() const
{
    return &_channels;
}

void Configuration::updateSpec()
{
    /*QVariantMap channels;

    for (auto channel : _channels) {
        if (!channel->isEnabled())
            continue;

        channels[channel->getInternalName()] = channel->getSpec();
    }

    _spec["channels"]               = channels;
    _spec["showDimensionNames"]     = _showDimensionNames;*/
}

QVariantMap Configuration::getSpec() const
{
	return _spec;
}

std::int32_t Configuration::getModified() const
{
    return _spec["modified"].toInt();
}


