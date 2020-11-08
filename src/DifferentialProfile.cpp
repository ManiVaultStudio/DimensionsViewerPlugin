#include "DifferentialProfile.h"
#include "Channel.h"
#include "Configuration.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>

const QMap<QString, DifferentialProfile::Column> DifferentialProfile::columns = {
    { "Name", DifferentialProfile::Column::Name },
    { "Enabled", DifferentialProfile::Column::Enabled },
    { "Dataset names 1", DifferentialProfile::Column::DatasetNames2 },
    { "Dataset names 2", DifferentialProfile::Column::DatasetNames2 },
    { "Dataset name 1", DifferentialProfile::Column::DatasetName1 },
    { "Dataset name 2", DifferentialProfile::Column::DatasetName1 }
};

const QMap<DifferentialProfile::Column, std::function<QVariant(DifferentialProfile* differentialProfile)>> DifferentialProfile::getEditRoles = {
    { DifferentialProfile::Column::Name, [](DifferentialProfile* differentialProfile) {
        return differentialProfile->_name;
    }},
    { DifferentialProfile::Column::Enabled, [](DifferentialProfile* differentialProfile) {
        return differentialProfile->_enabled;
    }},
    { DifferentialProfile::Column::DatasetNames1, [](DifferentialProfile* differentialProfile) {
        return differentialProfile->getConfigurationsModel()->getDatasetNames();
    }},
    { DifferentialProfile::Column::DatasetNames2, [](DifferentialProfile* differentialProfile) {
        return differentialProfile->getConfigurationsModel()->getDatasetNames();
    }},
    { DifferentialProfile::Column::DatasetName1, [](DifferentialProfile* differentialProfile) {
        return differentialProfile->_profileDatasetName[0];
    }},
    { DifferentialProfile::Column::DatasetName2, [](DifferentialProfile* differentialProfile) {
        return differentialProfile->_profileDatasetName[1];
    }}
};

const QMap<DifferentialProfile::Column, std::function<QVariant(DifferentialProfile* differentialProfile)>> DifferentialProfile::getDisplayRoles = {
    { DifferentialProfile::Column::Name, [](DifferentialProfile* differentialProfile) {
        return getEditRoles[DifferentialProfile::Column::Name](differentialProfile);
    }},
    { DifferentialProfile::Column::Enabled, [](DifferentialProfile* differentialProfile) {
        return getEditRoles[DifferentialProfile::Column::Enabled](differentialProfile).toBool() ? "on" : "off";
    }},
    { DifferentialProfile::Column::DatasetNames1, [](DifferentialProfile* differentialProfile) {
        return getEditRoles[DifferentialProfile::Column::DatasetNames1](differentialProfile);
    }},
    { DifferentialProfile::Column::DatasetNames2, [](DifferentialProfile* differentialProfile) {
        return getEditRoles[DifferentialProfile::Column::DatasetNames2](differentialProfile);
    }},
    { DifferentialProfile::Column::DatasetName1, [](DifferentialProfile* differentialProfile) {
        return getEditRoles[DifferentialProfile::Column::DatasetName1](differentialProfile);
    }},
    { DifferentialProfile::Column::DatasetName2, [](DifferentialProfile* differentialProfile) {
        return getEditRoles[DifferentialProfile::Column::DatasetName2](differentialProfile);
    }}
};

const QMap<DifferentialProfile::Column, std::function<QModelIndexList(DifferentialProfile* differentialProfile, const QVariant& value, const QModelIndex& index)>> DifferentialProfile::setEditRoles = {
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

    const auto column       = static_cast<Column>(index.column());
    const auto noSubsets    = getConfiguration()->getSubsets().count();

    switch (column)
    {
        case Column::Enabled:
        {
            if (_enabled && noSubsets >= 2)
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

QVariant DifferentialProfile::getData(const QModelIndex& index, const int& role) const
{
    const auto column = index.column();

    switch (role)
    {
        case Qt::EditRole:
        {
            if (getEditRoles.contains(static_cast<Column>(column)))
                return getEditRoles[static_cast<Column>(column)](const_cast<DifferentialProfile*>(this));

            break;
        }

        case Qt::DisplayRole:
        {
            if (getDisplayRoles.contains(static_cast<Column>(column)))
                return getDisplayRoles[static_cast<Column>(column)](const_cast<DifferentialProfile*>(this));

            break;
        }

        default:
            break;
    }

    return QVariant();
}

QModelIndexList DifferentialProfile::setData(const QModelIndex& index, const QVariant& value, const int& role)
{
    const auto column = static_cast<Column>(index.column());

    QModelIndexList affectedIndices{ index };

    switch (role)
    {
        case Qt::EditRole:
        {
            if (setEditRoles.contains(column))
                affectedIndices << setEditRoles[column](const_cast<DifferentialProfile*>(this), value, index);

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