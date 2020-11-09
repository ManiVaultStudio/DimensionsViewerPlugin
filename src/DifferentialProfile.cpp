#include "DifferentialProfile.h"
#include "Channel.h"
#include "Configuration.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>

const QMap<QString, DifferentialProfile::Columns> DifferentialProfile::columns = {
    { "Name", DifferentialProfile::Columns::Name },
    { "Enabled", DifferentialProfile::Columns::Enabled },
    { "Dataset names 1", DifferentialProfile::Columns::DatasetNames2 },
    { "Dataset names 2", DifferentialProfile::Columns::DatasetNames2 },
    { "Dataset name 1", DifferentialProfile::Columns::DatasetName1 },
    { "Dataset name 2", DifferentialProfile::Columns::DatasetName1 }
};

const QMap<DifferentialProfile::Columns, std::function<QVariant(DifferentialProfile* differentialProfile)>> DifferentialProfile::getEditRoles = {
    { DifferentialProfile::Columns::Name, [](DifferentialProfile* differentialProfile) {
        return differentialProfile->_name;
    }},
    { DifferentialProfile::Columns::Enabled, [](DifferentialProfile* differentialProfile) {
        return differentialProfile->_enabled && differentialProfile->getConfiguration()->getChannels()->getNoChannelsEnabled() >= 2;
    }},
    { DifferentialProfile::Columns::DatasetNames1, [](DifferentialProfile* differentialProfile) {
        return differentialProfile->getConfigurationsModel()->getDatasetNames();
    }},
    { DifferentialProfile::Columns::DatasetNames2, [](DifferentialProfile* differentialProfile) {
        return differentialProfile->getConfigurationsModel()->getDatasetNames();
    }},
    { DifferentialProfile::Columns::DatasetName1, [](DifferentialProfile* differentialProfile) {
        return differentialProfile->_profileDatasetName[0];
    }},
    { DifferentialProfile::Columns::DatasetName2, [](DifferentialProfile* differentialProfile) {
        return differentialProfile->_profileDatasetName[1];
    }}
};

const QMap<DifferentialProfile::Columns, std::function<QVariant(DifferentialProfile* differentialProfile)>> DifferentialProfile::getDisplayRoles = {
    { DifferentialProfile::Columns::Name, [](DifferentialProfile* differentialProfile) {
        return getEditRoles[DifferentialProfile::Columns::Name](differentialProfile);
    }},
    { DifferentialProfile::Columns::Enabled, [](DifferentialProfile* differentialProfile) {
        return getEditRoles[DifferentialProfile::Columns::Enabled](differentialProfile).toBool() ? "on" : "off";
    }},
    { DifferentialProfile::Columns::DatasetNames1, [](DifferentialProfile* differentialProfile) {
        return getEditRoles[DifferentialProfile::Columns::DatasetNames1](differentialProfile);
    }},
    { DifferentialProfile::Columns::DatasetNames2, [](DifferentialProfile* differentialProfile) {
        return getEditRoles[DifferentialProfile::Columns::DatasetNames2](differentialProfile);
    }},
    { DifferentialProfile::Columns::DatasetName1, [](DifferentialProfile* differentialProfile) {
        return getEditRoles[DifferentialProfile::Columns::DatasetName1](differentialProfile);
    }},
    { DifferentialProfile::Columns::DatasetName2, [](DifferentialProfile* differentialProfile) {
        return getEditRoles[DifferentialProfile::Columns::DatasetName2](differentialProfile);
    }}
};

const QMap<DifferentialProfile::Columns, std::function<QModelIndexList(DifferentialProfile* differentialProfile, const QModelIndex& index, const QVariant& value)>> DifferentialProfile::setEditRoles = {
    { DifferentialProfile::Columns::Enabled, [](DifferentialProfile* differentialProfile, const QModelIndex& index, const QVariant& value) {
        differentialProfile->_enabled = value.toBool();
        return QModelIndexList();
    }},
    { DifferentialProfile::Columns::DatasetNames1, [](DifferentialProfile* differentialProfile, const QModelIndex& index, const QVariant& value) {
        differentialProfile->_profileDatasetNames[0] = value.toStringList();
        return QModelIndexList();
    }},
    { DifferentialProfile::Columns::DatasetNames2, [](DifferentialProfile* differentialProfile, const QModelIndex& index, const QVariant& value) {
        differentialProfile->_profileDatasetNames[1] = value.toStringList();
        return QModelIndexList();
    }},
    { DifferentialProfile::Columns::DatasetName1, [](DifferentialProfile* differentialProfile, const QModelIndex& index, const QVariant& value) {
        differentialProfile->_profileDatasetName[0] = value.toString();
        return QModelIndexList();
    }},
    { DifferentialProfile::Columns::DatasetName2, [](DifferentialProfile* differentialProfile, const QModelIndex& index, const QVariant& value) {
        differentialProfile->_profileDatasetName[1] = value.toString();
        return QModelIndexList();
    }}
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
    return ModelItem::maxNoColumns;
}

Qt::ItemFlags DifferentialProfile::getFlags(const QModelIndex& index) const
{
    Qt::ItemFlags flags;

    const auto column               = static_cast<Columns>(index.column());
    const auto noChannelsEnabled    = getConfiguration()->getChannels()->getNoChannelsEnabled();
    const auto noDatasets           = getConfiguration()->getSubsets().count() + 1;

    switch (column)
    {
        case Columns::Enabled:
        {
            if (noChannelsEnabled >= 2)
                flags |= Qt::ItemIsEnabled;

            break;
        }

        case Columns::DatasetNames1:
        case Columns::DatasetNames2:
        case Columns::DatasetName1:
        case Columns::DatasetName2:
        {
            if (noDatasets > 2)
                flags |= Qt::ItemIsEnabled;

            break;
        }

        default:
            break;
    }

    return flags;
}

QVariant DifferentialProfile::getData(const QModelIndex& index, const int& role) const
{
    const auto column = index.column();

    switch (role)
    {
        case Qt::EditRole:
        {
            if (getEditRoles.contains(static_cast<Columns>(column)))
                return getEditRoles[static_cast<Columns>(column)](const_cast<DifferentialProfile*>(this));

            break;
        }

        case Qt::DisplayRole:
        {
            if (getDisplayRoles.contains(static_cast<Columns>(column)))
                return getDisplayRoles[static_cast<Columns>(column)](const_cast<DifferentialProfile*>(this));

            break;
        }

        default:
            break;
    }

    return QVariant();
}

QModelIndexList DifferentialProfile::setData(const QModelIndex& index, const QVariant& value, const int& role)
{
    const auto column = static_cast<Columns>(index.column());

    QModelIndexList affectedIndices{ index };

    switch (role)
    {
        case Qt::EditRole:
        {
            if (setEditRoles.contains(column))
                affectedIndices << setEditRoles[column](const_cast<DifferentialProfile*>(this), index, value);

            break;
        }

        default:
            break;
    }

    return affectedIndices;
}

ModelItem* DifferentialProfile::getChild(const int& index) const
{
    return nullptr;
}

int DifferentialProfile::getChildCount() const
{
    return 0;
}

int DifferentialProfile::getChildIndex(ModelItem* child) const
{
    return 0;
}

Configuration* DifferentialProfile::getConfiguration() const
{
    return dynamic_cast<Configuration*>(parent());
}