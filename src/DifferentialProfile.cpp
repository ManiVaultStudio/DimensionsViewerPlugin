#include "DifferentialProfile.h"
#include "Channel.h"
#include "Configuration.h"

#include <QDebug>

const QMap<QString, DifferentialProfile::Column> DifferentialProfile::columns = {
    { "Type", DifferentialProfile::Column::Type },
    { "Enabled", DifferentialProfile::Column::Enabled },
    { "Dataset names 1", DifferentialProfile::Column::DatasetNames2 },
    { "Dataset names 2", DifferentialProfile::Column::DatasetNames2 },
    { "Dataset name 1", DifferentialProfile::Column::DatasetName1 },
    { "Dataset name 2", DifferentialProfile::Column::DatasetName1 }
};

DifferentialProfile::DifferentialProfile(ModelItem* parent) :
    ModelItem("DifferentialProfile", parent),
    _enabled(false),
    _profileDatasetNames(),
    _profileDatasetName()
{
}

int DifferentialProfile::columnCount() const
{
    return to_ul(Column::Count);
}

Qt::ItemFlags DifferentialProfile::getFlags(const QModelIndex& index) const
{
    Qt::ItemFlags flags;

    const auto column               = static_cast<Column>(index.column());
    const auto noChannelsEnabled    = getConfiguration()->getChannels()->getNoChannelsEnabled();

    switch (column)
    {
        case Column::Type:
            break;

        case Column::Enabled:
        {
            if (noChannelsEnabled >= 2)
                flags |= Qt::ItemIsEnabled;

            break;
        }

        case Column::DatasetNames1:
        case Column::DatasetNames2:
        case Column::DatasetName1:
        case Column::DatasetName2:
            break;

        default:
            break;
    }

    return flags;
}

QVariant DifferentialProfile::getData(const std::int32_t& column, const std::int32_t& role) const
{
    switch (role)
    {
        case Qt::EditRole: {

            switch (static_cast<Column>(column))
            {
                case Column::Type:
                    return _type;

                case Column::Enabled:
                    return _enabled && getConfiguration()->getChannels()->getNoChannelsEnabled() >= 2;

                default:
                    break;
            }

            break;
        }

        case Qt::DisplayRole: {

            switch (static_cast<Column>(column))
            {
                case Column::Type:
                    return getData(column, Qt::EditRole);

                case Column::Enabled:
                    return getData(column, Qt::EditRole).toBool() ? "on" : "off";

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

QModelIndexList DifferentialProfile::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    QModelIndexList affectedIndices{ index };

    switch (role)
    {
        case Qt::EditRole: {

            switch (static_cast<Column>(index.column()))
            {
                case Column::Type:
                    break;

                case Column::Enabled: {
                    _enabled = value.toBool();
                    break;
                }

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

Configuration* DifferentialProfile::getConfiguration() const
{
    return dynamic_cast<Configuration*>(parent());
}