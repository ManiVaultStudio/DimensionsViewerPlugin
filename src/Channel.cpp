#include "Channel.h"
#include "Channels.h"
#include "Configuration.h"
#include "ConfigurationsModel.h"
#include "Profile.h"
#include "Differential.h"
#include "Styling.h"
#include "StandardItems.h"
#include "DimensionsViewerPlugin.h"
#include "Visitor.h"

#include "PointData.h"

#include <QDebug>
#include <QVariantList>

const QMap<QString, Channel::Child> Channel::children = {
    { "Enabled", Channel::Child::Enabled },
    { "DatasetNames", Channel::Child::DatasetNames },
    { "DatasetName", Channel::Child::DatasetName },
    { "Linked", Channel::Child::Linked },
    { "NumberOfPoints", Channel::Child::NoPoints },
    { "NumberOfDimensions", Channel::Child::NoDimensions },
    { "Profile", Channel::Child::Profile },
    { "Differential", Channel::Child::Differential },
    { "Styling", Channel::Child::Styling }
};

DimensionsViewerPlugin* Channel::dimensionsViewerPlugin = nullptr;

Channel::Channel(Item* parent, const std::uint32_t& index, const QString& name, const bool& enabled, const bool& linked, const QString& datasetName) :
    Item(parent, "Channel", name),
    _points(nullptr)
{
    _flags.setFlag(Qt::ItemIsEditable);
    _flags.setFlag(Qt::ItemIsEnabled);

    _children << new tree::Boolean(this, "Enabled", enabled);
    _children << new tree::StringList(this, "DatasetNames");
    _children << new tree::String(this, "DatasetName");
    _children << new tree::Boolean(this, "Linked", linked);
    _children << new tree::Integral(this, "NumberOfPoints");
    _children << new tree::Integral(this, "NumberOfDimensions");
    _children << new Profile(this, "Profile");
    _children << new Differential(this, "Differential");
    _children << new Styling(this, "Styling");

    resolvePoints();
}

void Channel::initialize()
{
    const auto updateFlags = [this](const QModelIndex& modelIndex) {
        const auto isEnabled    = getChild("Enabled")->getData(Column::Value, Qt::EditRole).toBool();
        const auto datasetNames = getChild("DatasetNames")->getData(Column::Value, Qt::EditRole).toStringList();
        const auto isLinked     = getChild("Linked")->getData(Column::Value, Qt::EditRole).toBool();

        getChild("Enabled")->setFlag(Qt::ItemIsEnabled, false);// datasetNames.count() >= 1);
        getChild("DatasetName")->setFlag(Qt::ItemIsEnabled, isEnabled);
        getChild("Linked")->setFlag(Qt::ItemIsEnabled, isEnabled);
        getChild("Profile")->setFlag(Qt::ItemIsEnabled, isEnabled && !isLinked);
        getChild("Differential")->setFlag(Qt::ItemIsEnabled, isEnabled);
        getChild("Styling")->setFlag(Qt::ItemIsEnabled, isEnabled && !isLinked);
    };

    QObject::connect(getChild("Enabled"), &tree::Boolean::dataChanged, updateFlags);
    QObject::connect(getChild("Linked"), &tree::Boolean::dataChanged, updateFlags);

    updateFlags(_modelIndex);
}

void Channel::accept(tree::Visitor* visitor) const
{
    visitor->visitTreeItem(this);
}

void Channel::setDimensionsViewerPlugin(DimensionsViewerPlugin* dimensionsViewerPlugin)
{
    Q_ASSERT(dimensionsViewerPlugin != nullptr);

    Channel::dimensionsViewerPlugin = dimensionsViewerPlugin;
}

std::int32_t Channel::getNoDimensions() const
{
    if (_points == nullptr)
        return 0;

    return _points->getNumDimensions();
}

std::int32_t Channel::getNoPoints() const
{
    if (_points == nullptr)
        return 0;

    return _points->getNumPoints();
}

void Channel::resolvePoints()
{
    Q_ASSERT(Channel::dimensionsViewerPlugin != nullptr);

    auto core = Channel::dimensionsViewerPlugin->getCore();

    const auto datasetName = _children[to_ul(Channel::Child::DatasetName)]->getData(Column::Value, Qt::EditRole).toString();

    if (datasetName.isEmpty())
        return;

    _points = &dynamic_cast<Points&>(core->requestData(datasetName));

    model->setData(model->index(to_ul(Child::NoPoints), to_ul(Item::Column::Value), _modelIndex), _points->getNumPoints());
    model->setData(model->index(to_ul(Child::NoDimensions), to_ul(Item::Column::Value), _modelIndex), _points->getNumDimensions());
}

const Channels* Channel::getChannels() const
{
    return dynamic_cast<Channels*>(_parent);
}

/*
Qt::ItemFlags Channel::getFlags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = TreeItem::getFlags(index);

    const auto column               = static_cast<Column>(index.column());
    const auto noDatasets           = _datasetNames.count();
    const auto channel              = static_cast<Channels::Row>(_index);

    switch (column)
    {
        case Column::Enabled:
        {
            flags |= Qt::ItemIsEditable;

            switch (channel)
            {
                case Channels::Row::Dataset:
                {
                    if (noDatasets >= 1)
                        flags |= Qt::ItemIsEnabled;

                    break;
                }

                case Channels::Row::Subset1:
                {
                    if (noDatasets >= 1)
                        flags |= Qt::ItemIsEnabled;

                    break;
                }

                case Channels::Row::Subset2:
                {
                    if (noDatasets >= 2)
                        flags |= Qt::ItemIsEnabled;

                    break;
                }

                case Channels::Row::Differential:
                {
                    if (_differential->isPrimed())
                        flags |= Qt::ItemIsEnabled;

                    break;
                }

                default:
                    break;
            }

            break;
        }

        case Column::Index:
        {
            flags |= Qt::ItemIsEnabled;

            break;
        }

        case Column::DatasetNames:
        case Column::DatasetName:
        {
            switch (channel)
            {
                case Channels::Row::Dataset:
                {
                    flags |= Qt::ItemIsEditable;

                    if (noDatasets >= 2)
                        flags |= Qt::ItemIsEnabled;

                    break;
                }

                case Channels::Row::Subset1:
                {
                    flags |= Qt::ItemIsEditable;

                    if (_enabled && noDatasets >= 1)
                        flags |= Qt::ItemIsEnabled;

                    break;
                }

                case Channels::Row::Subset2:
                {
                    flags |= Qt::ItemIsEditable;

                    if (_enabled && noDatasets >= 2)
                        flags |= Qt::ItemIsEnabled;

                    break;
                }

                case Channels::Row::Differential:
                {
                    if (_enabled && noDatasets >= 2)
                        flags |= Qt::ItemIsEnabled;

                    break;
                }

                default:
                    break;
            }

            break;
        }

        case Column::Differential:
        {
            switch (channel)
            {
                case Channels::Row::Dataset:
                case Channels::Row::Subset1:
                case Channels::Row::Subset2:
                    break;

                case Channels::Row::Differential:
                {
                    flags |= Qt::ItemIsEditable;

                    if (_enabled)
                        flags |= Qt::ItemIsEnabled;

                    break;
                }

                default:
                    break;
            }

            break;
        }

        case Column::Profile:
        {
            flags |= Qt::ItemIsEditable;

            switch (channel)
            {
                case Channels::Row::Dataset:
                case Channels::Row::Subset1:
                case Channels::Row::Subset2:
                case Channels::Row::Differential:
                {
                    if (_enabled && !_linked)
                        flags |= Qt::ItemIsEnabled;

                    break;
                }

                default:
                    break;
            }

            break;
        }

        case Column::Styling:
        {
            flags |= Qt::ItemIsEditable;

            switch (channel)
            {
                case Channels::Row::Dataset:
                {
                    bool enabled = true;

                    const auto channelsEnabled = getChannels()->getFiltered(Profile::ProfileTypes(), &enabled);

                    if (!channelsEnabled.isEmpty())
                        flags |= Qt::ItemIsEnabled;

                    break;
                }

                case Channels::Row::Subset1:
                case Channels::Row::Subset2:
                case Channels::Row::Differential:
                {
                    if (_enabled && !_linked)
                        flags |= Qt::ItemIsEnabled;

                    break;
                }

                default:
                    break;
            }

            break;
        }

        case Column::Linked:
        {
            switch (channel)
            {
                case Channels::Row::Dataset:
                {
                    flags |= Qt::ItemIsEnabled;

                    break;
                }

                case Channels::Row::Subset1:
                case Channels::Row::Subset2:
                case Channels::Row::Differential:
                {
                    flags |= Qt::ItemIsEditable;

                    if (_enabled)
                        flags |= Qt::ItemIsEnabled;

                    break;
                }

                default:
                    break;
            }

            break;
        }

        case Column::NoDimensions:
        case Column::NoPoints:
        case Column::IsAggregate:
            break;

        default:
            break;
    }

    return flags;
}

*/

//void Channel::updateSpec()
//{
//    if (_points == nullptr)
//        return;
//
//    //qDebug() << "Updating dimensions for" << _points->getName();
//
//    const auto& selection = dynamic_cast<Points&>(getCore()->requestSelection(_points->getDataName()));
//
//    std::vector<std::uint32_t> pointIndices;
//
//    if (isSubset()) {
//        pointIndices.resize(_points->indices.size());
//        std::iota(pointIndices.begin(), pointIndices.end(), 0);
//    }
//    else {
//        if (selection.indices.size() > 0) {
//            pointIndices = selection.indices;
//        }
//        else {
//            pointIndices.resize(_points->getNumPoints());
//            std::iota(pointIndices.begin(), pointIndices.end(), 0);
//        }
//    }
//
//    std::vector<std::uint32_t> dimensionIndices;
//
//    dimensionIndices.resize(_points->getNumDimensions());
//    std::iota(dimensionIndices.begin(), dimensionIndices.end(), 0);
//
//    QVariantList dimensions;
//
//    std::vector<float> dimensionValues;
//
//    dimensionValues.resize(pointIndices.size());
//
//    if (_enabled && !pointIndices.empty()) {
//        _points->visitSourceData([this, &pointIndices, &dimensionIndices, &dimensions, &dimensionValues](auto& pointData) {
//            for (const auto& dimensionIndex : dimensionIndices) {
//                auto localPointIndex = 0;
//
//                for (const auto& pointIndex : pointIndices) {
//                    dimensionValues[localPointIndex] = pointData[pointIndex][dimensionIndex];
//                    localPointIndex++;
//                }
//
//                QVariantMap dimension;
//
//                dimension["chn"] = _index;
//                dimension["dimId"] = dimensionIndex;
//                dimension["dimName"] = _points->getDimensionNames().at(dimensionIndex);
//
//                const float sum = std::accumulate(dimensionValues.begin(), dimensionValues.end(), 0.0);
//                const float mean = sum / dimensionValues.size();
//
//                std::vector<float> diff(dimensionValues.size());
//
//                std::transform(dimensionValues.begin(), dimensionValues.end(), diff.begin(), [mean](double x) { return x - mean; });
//
//                switch (_profile.getProfileType())
//                {
//                    case Profile::ProfileType::Mean: {
//                        dimension["agg"] = mean;
//                        break;
//                    }
//
//                    case Profile::ProfileType::Median: {
//                        std::sort(dimensionValues.begin(), dimensionValues.end());
//                        dimension["agg"] = dimensionValues[static_cast<int>(floorf(dimensionValues.size() / 2))];
//                        break;
//                    }
//
//                    default:
//                        break;
//                }
//
//                switch (_profile.getRangeType())
//                {
//                    case Profile::RangeType::StandardDeviation1: {
//                        double sqSum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
//                        double stdDev1 = std::sqrt(sqSum / dimensionValues.size());
//
//                        dimension["v1"] = mean - stdDev1;
//                        dimension["v2"] = mean + stdDev1;
//                        break;
//                    }
//
//                    case Profile::RangeType::StandardDeviation2: {
//                        double sqSum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
//                        double stdDev2 = 2.0 * std::sqrt(sqSum / dimensionValues.size());
//
//                        dimension["v1"] = mean - stdDev2;
//                        dimension["v2"] = mean + stdDev2;
//                        break;
//                    }
//
//                    case Profile::RangeType::MinMax: {
//                        auto result = std::minmax_element(dimensionValues.begin(), dimensionValues.end());
//
//                        dimension["min"] = *result.first;
//                        dimension["max"] = *result.second;
//                        break;
//                    }
//
//                    default:
//                        break;
//                }
//
//                dimensions.append(dimension);
//            }
//        });
//    }
//
//    _spec["enabled"] = _enabled;
//    _spec["index"] = _index;
//    _spec["displayName"] = _displayName;
//    _spec["datasetName"] = _datasetName;
//    _spec["dimensions"] = dimensions;
//    _spec["profileType"] = static_cast<int>(_profile.getProfileType());
//    _spec["rangeType"] = static_cast<int>(_profile.getRangeType());
//    _spec["canDisplay"] = canDisplay();
//
//    emit specChanged(this);
//}