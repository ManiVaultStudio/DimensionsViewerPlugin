#include "Channel.h"
#include "Channels.h"
#include "Configuration.h"
#include "ConfigurationsModel.h"

#include "PointData.h"

#include <QDebug>
#include <QVariantList>

const QMap<QString, Channel::Column> Channel::columns = {
    { "Name", Channel::Column::Name },
    { "Index", Channel::Column::Index },
    { "Internal name", Channel::Column::InternalName },
    { "Display name", Channel::Column::DisplayName },
    { "Enabled", Channel::Column::Enabled },
    { "Dataset names", Channel::Column::DatasetNames },
    { "Dataset name", Channel::Column::DatasetName },
    { "Number of dimensions", Channel::Column::NoDimensions },
    { "Number of points", Channel::Column::NoPoints },
    { "Styling", Channel::Column::Styling }
};

Channel::Channel(ModelItem* parent, const std::uint32_t& index, const QString& displayName, const bool& enabled, const QString& datasetName, const QString& dataName, const QColor& color, const float& opacity /*= 1.0f*/) :
    ModelItem("Channel", parent),
	_index(index),
	_internalName(QString("channel%1").arg(QString::number(index))),
	_displayName(displayName),
	_enabled(enabled),
    _datasetNames(),
	_datasetName(datasetName),
	_color(color),
	_opacity(opacity),
	_profile(this, Profile::ProfileType::Mean),
	_styling(this),
	_spec(),
    _points(nullptr)
{
    resolvePoints();
}

int Channel::columnCount() const 
{
    return to_ul(Column::Count);
}

Qt::ItemFlags Channel::getFlags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = Qt::ItemIsEditable;

    const auto column           = static_cast<Column>(index.column());
    const auto configuration    = getChannels()->getConfiguration();
    const auto globalEnabled    = configuration->getGlobal()->_enabled;
    const auto noSubsets        = configuration->getSubsets().count();

    switch (column)
    {
        case Column::Index:
        case Column::InternalName:
        case Column::DisplayName:
        {
            flags |= Qt::ItemIsEnabled;

            break;
        }

        case Column::Enabled: {
            switch (_index)
            {
                case Channels::Row::Channel1: {
                    flags |= Qt::ItemIsEnabled;

                    break;
                }

                case Channels::Row::Channel2: {
                    if (noSubsets >= 1)
                        flags |= Qt::ItemIsEnabled;

                    break;
                }

                case Channels::Row::Channel3: {
                    if (noSubsets >= 2)
                        flags |= Qt::ItemIsEnabled;

                    break;
                }
                    
                default:
                    break;
            }

            break;
        }
        
        case Column::DatasetNames:
        case Column::DatasetName:
        {
            switch (_index)
            {
                case Channels::Row::Channel1: {
                    flags |= Qt::ItemIsEnabled;

                    break;
                }

                case Channels::Row::Channel2: {
                    if (_enabled && noSubsets >= 1)
                        flags |= Qt::ItemIsEnabled;

                    break;
                }

                case Channels::Row::Channel3: {
                    if (_enabled && noSubsets >= 2)
                        flags |= Qt::ItemIsEnabled;

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

    return flags;
}

QVariant Channel::getData(const std::int32_t& column, const std::int32_t& role) const
{
    switch (role)
    {
        case Qt::EditRole: {

            switch (static_cast<Column>(column))
            {
                case Channel::Column::Name:
                    return _name;

                case Channel::Column::Index:
                    return _index;

                case Channel::Column::InternalName:
                    return _internalName;

                case Channel::Column::DisplayName:
                    return _displayName;

                case Channel::Column::Enabled:
                    return _enabled;

                case Channel::Column::DatasetNames:
                {
                    if (_index == 0)
                        return getConfigurationsModel()->getDatasetNames();

                    return getChannels()->getConfiguration()->getSubsets();
                }

                case Channel::Column::DatasetName:
                    return _datasetName;

                case Channel::Column::NoDimensions:
                    return getNoDimensions();

                case Channel::Column::NoPoints:
                    return getNoPoints();

                default:
                    break;
            }

            break;
        }

        case Qt::DisplayRole: {

            switch (static_cast<Column>(column))
            {
                case Channel::Column::Name:
                    return getData(column, Qt::EditRole);

                case Channel::Column::Index:
                    return QString::number(getData(column, Qt::EditRole).toInt());

                case Channel::Column::InternalName:
                    return getData(column, Qt::EditRole);

                case Channel::Column::DisplayName:
                    return getData(column, Qt::EditRole);

                case Channel::Column::Enabled:
                    return getData(column, Qt::EditRole).toBool() ? "on" : "off";

                case Channel::Column::DatasetNames:
                    return getData(column, Qt::EditRole).toStringList().join(", ");

                case Channel::Column::DatasetName:
                    return getData(column, Qt::EditRole);

                case Channel::Column::NoDimensions:
                case Channel::Column::NoPoints:
                    return QString::number(getData(column, Qt::EditRole).toInt());

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

ModelItem::AffectedColumns Channel::setData(const std::int32_t& column, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    AffectedColumns affectedColunns{ column };

    switch (role)
    {
        case Qt::EditRole: {

            switch (static_cast<Column>(column))
            {
                case Channel::Column::Name:
                {
                    _displayName = value.toString();
                    break;
                }

                case Channel::Column::Enabled:
                {
                    _enabled = value.toBool();

                    /*QModelIndexList affectedIndices;

                    for (int column = to_ul(Channel::Column::Start); column <= to_ul(Channel::Column::End); column++)
                        affectedIndices << index.siblingAtColumn(column);

                    const auto differentialProfileIndex = channel->index(to_ul(Channel::Row::DifferentialProfile), 0, index.parent().parent());

                    for (int column = to_ul(DifferentialProfile::Column::Start); column <= to_ul(DifferentialProfile::Column::End); column++)
                        affectedIndices << differentialProfileIndex.siblingAtColumn(column);

                    affectedIndices << index.parent().parent().siblingAtColumn(to_ul(Miscellaneous::Column::ShowDimensionNames));

                    return affectedIndices;*/

                    break;
                }

                case Channel::Column::DatasetNames:
                {
                    _datasetNames = value.toStringList();

                    /*QModelIndexList affectedIndices;

                    for (int column = to_ul(Channel::Column::Start); column <= to_ul(Channel::Column::End); column++)
                        affectedIndices << index.siblingAtColumn(column);

                    return affectedIndices;*/

                    break;
                }

                case Channel::Column::DatasetName:
                {
                    _datasetName = value.toString();
                    
                    resolvePoints();

                    break;
                }

                default:
                    break;
            }

            break;
        }
    }

    return affectedColunns;
}

ModelItem* Channel::getChild(const int& index) const
{
    switch (static_cast<Row>(index))
    {
        case Row::Profile:
            return const_cast<Profile*>(&_profile);

        case Row::Styling:
            return const_cast<Styling*>(&_styling);

        default:
            break;
    }
    return nullptr;
}

int Channel::getChildCount() const
{
    return to_ul(Row::Count);
}

int Channel::getChildIndex(ModelItem* child) const
{
    if (dynamic_cast<Profile*>(child))
        return to_ul(Row::Profile);

    if (dynamic_cast<Styling*>(child))
        return to_ul(Row::Styling);

    return 0;
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
    if (!_datasetName.isEmpty())
        _points = &dynamic_cast<Points&>(getCore()->requestData(_datasetName));
}

const Channels* Channel::getChannels() const
{
    return dynamic_cast<Channels*>(_parent);
}

bool Channel::canDisplay() const
{
    return _enabled;
}

bool Channel::isSubset() const
{
	if (_points == nullptr)
		return false;

	return !_points->indices.empty();
}

void Channel::updateSpec()
{
	if (_points == nullptr)
		return;

	//qDebug() << "Updating dimensions for" << _points->getName();

	const auto& selection = dynamic_cast<Points&>(getCore()->requestSelection(_points->getDataName()));
	
	std::vector<std::uint32_t> pointIndices;
	
	if (isSubset()) {
		pointIndices.resize(_points->indices.size());
		std::iota(pointIndices.begin(), pointIndices.end(), 0);
	}
	else {
		if (selection.indices.size() > 0) {
			pointIndices = selection.indices;
		}
		else {
			pointIndices.resize(_points->getNumPoints());
			std::iota(pointIndices.begin(), pointIndices.end(), 0);
		}
	}

	std::vector<std::uint32_t> dimensionIndices;

	dimensionIndices.resize(_points->getNumDimensions());
	std::iota(dimensionIndices.begin(), dimensionIndices.end(), 0);
	
	QVariantList dimensions;

    std::vector<float> dimensionValues;

    dimensionValues.resize(pointIndices.size());

	if (_enabled && !pointIndices.empty()) {
		_points->visitSourceData([this, &pointIndices, &dimensionIndices, &dimensions, &dimensionValues](auto& pointData) {
			for (const auto& dimensionIndex : dimensionIndices) {
				auto localPointIndex = 0;

				for (const auto& pointIndex : pointIndices) {
					dimensionValues[localPointIndex] = pointData[pointIndex][dimensionIndex];
					localPointIndex++;
				}

				QVariantMap dimension;

				dimension["chn"]		= _index;
				dimension["dimId"]		= dimensionIndex;
				dimension["dimName"]	= _points->getDimensionNames().at(dimensionIndex);

                const float sum     = std::accumulate(dimensionValues.begin(), dimensionValues.end(), 0.0);
				const float mean    = sum / dimensionValues.size();

				std::vector<float> diff(dimensionValues.size());

				std::transform(dimensionValues.begin(), dimensionValues.end(), diff.begin(), [mean](double x) { return x - mean; });

				switch (_profile.getProfileType())
				{
					case Profile::ProfileType::Mean: {
						dimension["agg"] = mean;
						break;
					}

					case Profile::ProfileType::Median: {
						std::sort(dimensionValues.begin(), dimensionValues.end());
						dimension["agg"] = dimensionValues[static_cast<int>(floorf(dimensionValues.size() / 2))];
						break;
					}

					default:
						break;
				}

				switch (_profile.getRangeType())
				{
                    case Profile::RangeType::StandardDeviation1: {
						double sqSum	= std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
						double stdDev1	= std::sqrt(sqSum / dimensionValues.size());

						dimension["v1"] = mean - stdDev1;
						dimension["v2"] = mean + stdDev1;
						break;
					}

					case Profile::RangeType::StandardDeviation2: {
						double sqSum	= std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
						double stdDev2	= 2.0 * std::sqrt(sqSum / dimensionValues.size());

						dimension["v1"] = mean - stdDev2;
						dimension["v2"] = mean + stdDev2;
						break;
					}

                    case Profile::RangeType::MinMax: {
                        auto result = std::minmax_element(dimensionValues.begin(), dimensionValues.end());

                        dimension["min"] = *result.first;
                        dimension["max"] = *result.second;
                        break;
                    }

					default:
						break;
				}

				dimensions.append(dimension);
			}
		});
	}

	_spec["enabled"]		= _enabled;
	_spec["index"]			= _index;
	_spec["displayName"]    = _displayName;
	_spec["datasetName"]	= _datasetName;
	_spec["dimensions"]		= dimensions;
	_spec["color"]			= _color;
	_spec["opacity"]		= _opacity;
	_spec["profileType"]	= static_cast<int>(_profile.getProfileType());
	_spec["rangeType"]		= static_cast<int>(_profile.getRangeType());
	_spec["canDisplay"]		= canDisplay();

    emit specChanged(this);
}

QVariantMap Channel::getSpec()
{
    return _spec;
}