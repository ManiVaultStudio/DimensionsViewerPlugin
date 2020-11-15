#include "Channel.h"
#include "Channels.h"
#include "Configuration.h"
#include "ConfigurationsModel.h"

#include "Application.h"
#include "PointData.h"

#include <QDebug>
#include <QVariantList>

const QMap<QString, Channel::Column> Channel::columns = {
    { "Type", Channel::Column::Type },
    { "Index", Channel::Column::Index },
    { "Internal name", Channel::Column::InternalName },
    { "Display name", Channel::Column::DisplayName },
    { "Enabled", Channel::Column::Enabled },
    { "Dataset names", Channel::Column::DatasetNames },
    { "Dataset name", Channel::Column::DatasetName },
    { "Dataset name", Channel::Column::DatasetName },
    { "ProfileTypes", Channel::Column::ProfileTypes },
    { "Profile type", Channel::Column::ProfileType },
    { "Range types", Channel::Column::RangeTypes },
    { "Range type", Channel::Column::RangeType },
    { "Render points", Channel::Column::RenderPoints },
    { "Differential", Channel::Column::Differential },
    { "Differential operand A names", Channel::Column::DifferentialOperandNamesA },
    { "Differential operand B names", Channel::Column::DifferentialOperandNamesB },
    { "Differential operand A name", Channel::Column::DifferentialOperandA },
    { "Differential operand B name", Channel::Column::DifferentialOperandB },
    { "Styling", Channel::Column::Styling },
    { "Line types", Channel::Column::LineTypes },
    { "Line type profile", Channel::Column::LineTypeProfile },
    { "Line type range", Channel::Column::LineTypeRange },
    { "Opacity", Channel::Column::Opacity },
    { "Color", Channel::Column::Color},
    { "Linked", Channel::Column::Linked },
    { "Number of dimensions", Channel::Column::NoDimensions },
    { "Number of points", Channel::Column::NoPoints }
};

const QMap<QString, Channel::Row> Channel::rows = {
    { "Profile", Channel::Row::Profile },
    { "Styling", Channel::Row::Styling }
};

Channel::Channel(ModelItem* parent, const std::uint32_t& index, const QString& displayName, const bool& enabled, const bool& linked, const QString& datasetName, const Profile::ProfileType& profileType, const QColor& color, const float& opacity /*= 1.0f*/) :
    ModelItem("Channel", parent),
	_index(index),
	_internalName(QString("channel%1").arg(QString::number(index))),
	_displayName(displayName),
	_enabled(enabled),
    _datasetNames(),
	_datasetName(datasetName),
	_profile(profileType),
    _differential(this),
    _linked(linked),
	_styling(),
	_spec(),
    _points(nullptr)
{
    resolvePoints();

    _styling.setColor(color);
    _styling.setOpacity(opacity);
}

int Channel::columnCount() const 
{
    return to_ul(Column::_Count);
}

Qt::ItemFlags Channel::getFlags(const QModelIndex& index) const
{
    Qt::ItemFlags flags;

    const auto column               = static_cast<Column>(index.column());
    const auto noDatasets           = _datasetNames.count();
    const auto channel              = static_cast<Channels::Row>(_index);
    const auto enabled              = getData(Column::Enabled, Qt::EditRole).toBool();
    const auto meanOrMedianChannel  = _profile.getProfileType() == Profile::ProfileType::Mean || _profile.getProfileType() == Profile::ProfileType::Median;

    switch (column)
    {
        case Column::Type:
        case Column::Index:
        case Column::InternalName:
        case Column::DisplayName:
        {
            flags |= Qt::ItemIsEnabled;

            break;
        }

        case Column::Enabled: {
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
                    if (_differential.isPrimed())
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

                    if (enabled && noDatasets >= 1)
                        flags |= Qt::ItemIsEnabled;

                    break;
                }

                case Channels::Row::Subset2:
                {
                    flags |= Qt::ItemIsEditable;

                    if (enabled && noDatasets >= 2)
                        flags |= Qt::ItemIsEnabled;

                    break;
                }

                case Channels::Row::Differential:
                {
                    if (enabled && noDatasets >= 2)
                        flags |= Qt::ItemIsEnabled;

                    break;
                }

                default:
                    break;
            }

            break;
        }

        case Channel::Column::ProfileTypes:
        case Channel::Column::ProfileType:
        case Channel::Column::RangeTypes:
        case Channel::Column::RangeType:
        {
            //if (meanOrMedianChannel)
            //    flags |= Qt::ItemIsEditable;

            if (enabled) {
                switch (channel)
                {
                    case Channels::Row::Dataset:
                    {
                        flags |= Qt::ItemIsEnabled;

                        break;
                    }

                    case Channels::Row::Subset1:
                    case Channels::Row::Subset2:
                    {
                        if (!_linked && meanOrMedianChannel)
                            flags |= Qt::ItemIsEnabled;

                        break;
                    }

                    case Channels::Row::Differential:
                        break;

                    default:
                        break;
                }
            }
            
            break;
        }

        case Channel::Column::RenderPoints:
        {
            flags |= Qt::ItemIsEditable | Qt::ItemIsEnabled;

            break;
        }

        case Channel::Column::Differential:
        {
            if (_profile.getProfileType() == Profile::ProfileType::Differential) {
                flags |= Qt::ItemIsEditable;

                if (enabled)
                    flags |= Qt::ItemIsEnabled;
            }

            break;
        }

        case Channel::Column::DifferentialOperandNamesA:
            break;

        case Channel::Column::DifferentialOperandA:
        case Channel::Column::DifferentialOperandB:
        {
            if (_profile.getProfileType() == Profile::ProfileType::Differential) {
                flags |= Qt::ItemIsEditable;

                if (enabled && _differential.isPrimed() && _differential.getNumCombinations() >= 2)
                    flags |= Qt::ItemIsEnabled;
            }

            break;
        }

        case Channel::Column::Styling:
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
                    if (enabled && !_linked)
                        flags |= Qt::ItemIsEnabled;

                    break;
                }

                default:
                    break;
            }

            break;
        }

        case Channel::Column::LineTypes:
        case Channel::Column::LineTypeProfile:
        case Channel::Column::LineTypeRange:
        case Channel::Column::Opacity:
        case Channel::Column::Color:
        {
            flags |= Qt::ItemIsEditable;

            if (enabled)
                flags |= Qt::ItemIsEnabled;

            break;
        }

        case Channel::Column::Linked:
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

                    if (enabled)
                        flags |= Qt::ItemIsEnabled;

                    break;
                }

                default:
                    break;
            }

            break;
        }

        case Channel::Column::NoDimensions:
        case Channel::Column::NoPoints:
            break;

        default:
            break;
    }

    return flags;
}

QVariant Channel::getData(const std::int32_t& column, const std::int32_t& role) const
{
    const auto row = static_cast<Channels::Row>(_index);

    switch (role)
    {
        case Qt::EditRole:
        {
            switch (static_cast<Column>(column))
            {
                case Channel::Column::Type:
                    return _type;

                case Channel::Column::Index:
                    return _index;

                case Channel::Column::InternalName:
                    return _internalName;

                case Channel::Column::DisplayName:
                    return _displayName;

                case Channel::Column::Enabled:
                {
                    switch (row)
                    {
                        case Channels::Row::Dataset:
                        case Channels::Row::Subset1:
                        case Channels::Row::Subset2:
                            return _enabled;

                        case Channels::Row::Differential:
                            return _enabled && _differential.isPrimed();

                        default:
                            break;
                    }

                    break;
                }

                case Channel::Column::DatasetNames:
                    return _datasetNames;

                case Channel::Column::DatasetName:
                    return _datasetName;

                case Channel::Column::ProfileTypes:
                    return _profile.getProfileTypeNames();

                case Channel::Column::ProfileType:
                    return static_cast<std::int32_t>(_profile.getProfileType());

                case Channel::Column::RangeTypes:
                    return _profile.getRangeTypeNames();

                case Channel::Column::RangeType:
                    return static_cast<std::int32_t>(_profile.getRangeType());

                case Channel::Column::RenderPoints:
                    return _styling.getRenderPoints();

                case Channel::Column::DifferentialOperandNamesA:
                    return (_enabled && _differential.isPrimed()) ? _differential.getOperandChannelNames(Differential::Operand::ChannelA) : QStringList();

                case Channel::Column::DifferentialOperandNamesB:
                    return (_enabled && _differential.isPrimed()) ? _differential.getOperandChannelNames(Differential::Operand::ChannelB) : QStringList();

                case Channel::Column::DifferentialOperandA:
                    return (_enabled && _differential.isPrimed()) ? _differential.getOperandChannelName(Differential::Operand::ChannelA) : "";
                
                case Channel::Column::DifferentialOperandB:
                    return (_enabled && _differential.isPrimed()) ? _differential.getOperandChannelName(Differential::Operand::ChannelB) : "";

                case Channel::Column::Styling:
                    return "Styling";

                case Channel::Column::LineTypes:
                    return _styling.getLineTypeNames();

                case Channel::Column::LineTypeProfile:
                    return static_cast<std::int32_t>(_styling.getLineTypeProfile());

                case Channel::Column::LineTypeRange:
                    return static_cast<std::int32_t>(_styling.getLineTypeRange());

                case Channel::Column::Opacity:
                    return _styling.getOpacity();

                case Channel::Column::Color:
                    return QVariant::fromValue(_styling.getColor());

                case Channel::Column::Linked:
                    return _linked;

                case Channel::Column::NoDimensions:
                    return getNoDimensions();

                case Channel::Column::NoPoints:
                    return getNoPoints();

                default:
                    break;
            }

            break;
        }

        case Qt::DisplayRole:
        {
            switch (static_cast<Column>(column))
            {
                case Channel::Column::Type:
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

                case Channel::Column::ProfileTypes:
                    return getData(column, Qt::EditRole).toStringList().join(", ");

                case Channel::Column::ProfileType:
                    return Profile::getProfileTypeName(static_cast<Profile::ProfileType>(getData(column, Qt::EditRole).toInt()));

                case Channel::Column::RangeTypes:
                    return getData(column, Qt::EditRole).toStringList().join(", ");

                case Channel::Column::RangeType:
                    return Profile::getRangeTypeName(static_cast<Profile::RangeType>(getData(column, Qt::EditRole).toInt()));

                case Channel::Column::RenderPoints:
                    return getData(column, Qt::EditRole).toBool() ? "on" : "off";

                case Channel::Column::DifferentialOperandNamesA:
                    return getData(column, Qt::EditRole).toStringList().join(", ");

                case Channel::Column::DifferentialOperandNamesB:
                    return getData(column, Qt::EditRole).toStringList().join(", ");

                case Channel::Column::DifferentialOperandA:
                case Channel::Column::DifferentialOperandB:
                    return getData(column, Qt::EditRole);

                case Channel::Column::Styling:
                    return getData(column, Qt::EditRole);

                case Channel::Column::LineTypes:
                    return getData(column, Qt::EditRole).toStringList().join(", ");

                case Channel::Column::LineTypeProfile:
                case Channel::Column::LineTypeRange:
                    return Styling::getLineTypeName(static_cast<Styling::LineType>(getData(column, Qt::EditRole).toInt()));

                case Channel::Column::Opacity:
                    return QString::number(getData(column, Qt::EditRole).toFloat(), 'f', 2);

                case Channel::Column::Color:
                    return getData(column, Qt::EditRole).value<QColor>().name();

                case Channel::Column::Linked:
                    return hdps::Application::getIconFont("FontAwesome").getIconCharacter(_linked ? "link" : "unlink");

                case Channel::Column::NoDimensions:
                case Channel::Column::NoPoints:
                    return QString::number(getData(column, Qt::EditRole).toInt());

                default:
                    break;
            }

            break;
        }
        
        case Qt::ToolTipRole:
        {
            const auto tooltip = [&column](const QString& value) {
                return QString("%1: %2").arg(getColumnTypeName(static_cast<Column>(column)), value);
            };

            switch (static_cast<Column>(column))
            {
                case Channel::Column::Type:
                case Channel::Column::Index:
                case Channel::Column::InternalName:
                case Channel::Column::DisplayName:
                case Channel::Column::Enabled:
                case Channel::Column::DatasetNames:
                case Channel::Column::DatasetName:
                case Channel::Column::ProfileTypes:
                case Channel::Column::ProfileType:
                case Channel::Column::RangeTypes:
                case Channel::Column::RangeType:
                case Channel::Column::DifferentialOperandNamesA:
                case Channel::Column::DifferentialOperandNamesB:
                case Channel::Column::DifferentialOperandA:
                case Channel::Column::DifferentialOperandB:
                    return tooltip(getData(column, Qt::DisplayRole).toString());

                case Channel::Column::Styling:
                    return getData(column, Qt::DisplayRole).toString();

                case Channel::Column::LineTypes:
                case Channel::Column::LineTypeProfile:
                case Channel::Column::LineTypeRange:
                case Channel::Column::Opacity:
                case Channel::Column::Color:
                    return tooltip(getData(column, Qt::DisplayRole).toString());

                case Channel::Column::Linked:
                    return QString("%1: %2").arg("Linked to dataset", getData(column, Qt::EditRole).toBool() ? "true" : "false");

                case Channel::Column::NoDimensions:
                case Channel::Column::NoPoints:
                    return tooltip(getData(column, Qt::DisplayRole).toString());

                default:
                    break;
            }

            break;
        }

        case Qt::FontRole:
        {
            switch (static_cast<Column>(column))
            {
                case Channel::Column::Type:
                case Channel::Column::Index:
                case Channel::Column::InternalName:
                case Channel::Column::DisplayName:
                case Channel::Column::Enabled:
                case Channel::Column::DatasetNames:
                case Channel::Column::DatasetName:
                case Channel::Column::ProfileTypes:
                case Channel::Column::ProfileType:
                case Channel::Column::RangeTypes:
                case Channel::Column::RangeType:
                case Channel::Column::DifferentialOperandNamesA:
                case Channel::Column::DifferentialOperandNamesB:
                case Channel::Column::DifferentialOperandA:
                case Channel::Column::DifferentialOperandB:
                case Channel::Column::Styling:
                case Channel::Column::LineTypes:
                case Channel::Column::LineTypeProfile:
                case Channel::Column::LineTypeRange:
                case Channel::Column::Opacity:
                case Channel::Column::Color:
                    break;

                case Channel::Column::Linked:
                    return hdps::Application::getIconFont("FontAwesome").getFont(9);

                case Channel::Column::NoDimensions:
                case Channel::Column::NoPoints:
                    break;

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

QVariant Channel::getData(const Column& column, const std::int32_t& role) const
{
    return getData(static_cast<std::int32_t>(column), role);
}

QModelIndexList Channel::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    QModelIndexList affectedIndices{ index };

    const auto row      = static_cast<Channels::Row>(_index);
    const auto column   = static_cast<Column>(index.column());

    const auto updateChannel = [this, &affectedIndices, &index](const Channels::Row& channel) {
        for (int column = to_ul(Channel::Column::_Start); column <= to_ul(Channel::Column::_End); column++)
            affectedIndices << index.sibling(static_cast<int>(channel), column);
    };

    const auto updateDifferentialChannels = [this, &affectedIndices, &index, updateChannel]() {
        const auto differentialChannels = getChannels()->getFiltered(Profile::ProfileTypes({ Profile::ProfileType::Differential }));

        for (auto differentialChannel : differentialChannels) {
            differentialChannel->getDifferential().update();
            updateChannel(static_cast<Channels::Row>(differentialChannel->getData(to_ul(Channel::Column::Index), Qt::EditRole).toInt()));
        }
    };

    const auto synchronizeProfile = [this, &affectedIndices, &index]() {
        const auto channels = getChannels()->getFiltered(Profile::ProfileTypes({ Profile::ProfileType::Mean, Profile::ProfileType::Median }));

        for (auto channel : channels) {
            if (channel->getData(Column::Linked, Qt::EditRole).toBool())
                channel->getProfile() = getSibling(to_ul(Channels::Row::Dataset))->getProfile();

            for (int column = to_ul(Channel::Column::_ProfileStart); column <= to_ul(Channel::Column::_ProfileEnd); column++)
                affectedIndices << index.sibling(channel->getData(Column::Index, Qt::EditRole).toInt(), column);
        }
    };

    const auto synchronizeStyling = [this, &affectedIndices, &index]() {
        const auto channels = getChannels()->getFiltered(Profile::ProfileTypes({ Profile::ProfileType::Mean, Profile::ProfileType::Median, Profile::ProfileType::Differential }));

        for (auto channel : channels) {
            if (channel->getData(Column::Linked, Qt::EditRole).toBool())
                channel->getStyling() = getSibling(to_ul(Channels::Row::Dataset))->getStyling();

            for (int column = to_ul(Channel::Column::_StylingStart); column <= to_ul(Channel::Column::_StylingEnd); column++)
                affectedIndices << index.sibling(channel->getData(Column::Index, Qt::EditRole).toInt(), column);
        }
    };

    switch (role)
    {
        case Qt::EditRole:
        {
            switch (column)
            {
                case Channel::Column::Type:
                case Channel::Column::Index:
                case Channel::Column::InternalName:
                case Channel::Column::DisplayName:
                    break;

                case Channel::Column::Enabled:
                {
                    _enabled = value.toBool();

                    for (int row = to_ul(Channels::Row::_Start); row <= to_ul(Channels::Row::_End); row++)
                        affectedIndices << index.sibling(row, to_ul(Channel::Column::Styling));
                    
                    updateChannel(row);
                    updateDifferentialChannels();

                    break;
                }

                case Channel::Column::DatasetNames:
                {
                    _datasetNames = value.toStringList();

                    const auto noDatasets = _datasetNames.count();

                    switch (row)
                    {
                        case Channels::Row::Dataset:
                            break;

                        case Channels::Row::Subset1:
                        {
                            if (noDatasets == 1) {
                                _enabled        = true;
                                _datasetName    = _datasetNames[0];
                            }

                            break;
                        }

                        case Channels::Row::Subset2:
                        {
                            if (noDatasets == 2) {
                                _enabled        = true;
                                _datasetName    = _datasetNames[1];
                            }
                                

                            break;
                        }

                        case Channels::Row::Differential:
                        {
                            if (noDatasets >= 1)
                                _enabled = true;

                            break;
                        }

                        default:
                            break;
                    }

                    updateChannel(row);
                    updateDifferentialChannels();

                    break;
                }

                case Channel::Column::DatasetName:
                {
                    _datasetName = value.toString();
                    
                    resolvePoints();

                    break;
                }

                case Channel::Column::ProfileTypes:
                    break;

                case Channel::Column::ProfileType:
                {
                    _profile.setProfileType(static_cast<Profile::ProfileType>(value.toInt()));

                    affectedIndices << index.siblingAtColumn(to_ul(Channel::Column::RangeTypes));
                    affectedIndices << index.siblingAtColumn(to_ul(Channel::Column::RangeType));

                    break;
                }

                case Channel::Column::RangeTypes:
                    break;

                case Channel::Column::RangeType:
                {
                    _profile.setRangeType(static_cast<Profile::RangeType>(value.toInt()));

                    break;
                }

                case Channel::Column::RenderPoints:
                {
                    _styling.setRenderPoints(value.toBool());

                    break;
                }

                case Channel::Column::DifferentialOperandNamesA:
                case Channel::Column::DifferentialOperandNamesB:
                    break;

                case Channel::Column::DifferentialOperandA:
                {
                    _differential.setOperandChannelName(Differential::Operand::ChannelA, value.toString());

                    for (int column = to_ul(Channel::Column::_DifferentialStart); column <= to_ul(Channel::Column::_DifferentialEnd); column++)
                        affectedIndices << index.siblingAtColumn(column);

                    break;
                }

                case Channel::Column::DifferentialOperandB:
                {
                    _differential.setOperandChannelName(Differential::Operand::ChannelB, value.toString());

                    for (int column = to_ul(Channel::Column::_DifferentialStart); column <= to_ul(Channel::Column::_DifferentialEnd); column++)
                        affectedIndices << index.siblingAtColumn(column);

                    break;
                }

                case Channel::Column::Styling:
                case Channel::Column::LineTypes:
                    break;

                case Channel::Column::LineTypeProfile:
                case Channel::Column::LineTypeRange:
                {
                    _styling.setLineTypeProfile(static_cast<Styling::LineType>(value.toInt()));

                    break;
                }

                case Channel::Column::Opacity:
                {
                    _styling.setOpacity(value.toFloat());

                    break;
                }

                case Channel::Column::Color:
                {
                    _styling.setColor(value.value<QColor>());

                    break;
                }

                case Channel::Column::Linked:
                {
                    _linked = value.toBool();

                    updateChannel(row);
                    synchronizeProfile();
                    synchronizeStyling();

                    break;
                }

                case Channel::Column::NoDimensions:
                case Channel::Column::NoPoints:
                    break;

                default:
                    break;
            }

            break;
        }

        case Qt::DisplayRole:
        {
            switch (column)
            {
                case Channel::Column::Type:
                case Channel::Column::Index:
                case Channel::Column::InternalName:
                case Channel::Column::DisplayName:
                case Channel::Column::Enabled:
                case Channel::Column::DatasetNames:
                case Channel::Column::DatasetName:
                case Channel::Column::ProfileTypes:
                    break;

                case Channel::Column::ProfileType:
                {
                    _profile.setProfileType(Profile::getProfileTypeEnum(value.toString()));

                    affectedIndices << index.siblingAtColumn(to_ul(Channel::Column::RangeTypes));
                    affectedIndices << index.siblingAtColumn(to_ul(Channel::Column::RangeType));

                    break;
                }

                case Channel::Column::RangeTypes:
                    break;

                case Channel::Column::RangeType:
                {
                    _profile.setRangeType(Profile::getRangeTypeEnum(value.toString()));

                    synchronizeProfile();

                    break;
                }

                case Channel::Column::Styling:
                case Channel::Column::LineTypes:
                    break;

                case Channel::Column::LineTypeProfile:
                {
                    _styling.setLineTypeProfile(Styling::getLineTypeEnum(value.toString()));

                    break;
                }

                case Channel::Column::LineTypeRange:
                {
                    _styling.setLineTypeRange(Styling::getLineTypeEnum(value.toString()));

                    break;
                }

                case Channel::Column::Opacity:
                case Channel::Column::Color:
                case Channel::Column::Linked:
                case Channel::Column::NoDimensions:
                case Channel::Column::NoPoints:
                    break;

                default:
                    break;
            }

            break;
        }

        default:
            break;
    }

    if (column >= Column::_ProfileStart && column <= Column::_ProfileEnd)
        synchronizeProfile();

    if (column >= Column::_StylingStart && column <= Column::_StylingEnd)
        synchronizeStyling();

    return affectedIndices;
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

const Channel* Channel::getSibling(const std::int32_t& row) const
{
    return (*getChannels())[row];
}

Channel* Channel::getSibling(const std::int32_t& row)
{
    const auto constThis = const_cast<const Channel*>(this);

    return const_cast<Channel*>(constThis->getSibling(row));
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
	_spec["profileType"]	= static_cast<int>(_profile.getProfileType());
	_spec["rangeType"]		= static_cast<int>(_profile.getRangeType());
	_spec["canDisplay"]		= canDisplay();

    emit specChanged(this);
}

QVariantMap Channel::getSpec()
{
    return _spec;
}