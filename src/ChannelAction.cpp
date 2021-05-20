#include "ChannelAction.h"
#include "ConfigurationAction.h"
#include "DimensionsViewerPlugin.h"

#include "PointData.h"

#include "util/Timer.h"

#include <QDebug>
#include <QVariantList>

using namespace hdps::gui;

ChannelAction::ChannelAction(ConfigurationAction* configurationAction, const std::uint32_t& index, const bool& enabled, const QString& datasetName, const QColor& color, const float& opacity /*= 1.0f*/, const bool& lock /*= false*/) :
    PluginAction(configurationAction->getDimensionsViewerPlugin(), QString("Channel %1").arg(index)),
	_index(index),
	_internalName(QString("channel%1").arg(QString::number(index))),
	_displayName(QString("Channel %1").arg(index)),
	_spec(),
    _points(nullptr),
    _configuration(configurationAction),
    _enabledAction(this, QString("Channel %1").arg(index)),
    _datasetNameAction(this, ""),
    _colorAction(this, "Color"),
    _opacityAction(this, "Opacity"),
    _profileTypeAction(this, "Profile type"),
    _bandTypeAction(this, "Band type"),
    _showRangeAction(this, "Show range"),
    _lockedAction(this, "Locked")
{
    _enabledAction.setEnabled(enabled);
    _datasetNameAction.setEnabled(enabled);
    _datasetNameAction.setEnabled(enabled);
    _colorAction.setEnabled(enabled);
    _opacityAction.setEnabled(enabled);
    _profileTypeAction.setEnabled(enabled);
    _bandTypeAction.setEnabled(enabled);
    _showRangeAction.setEnabled(enabled);
    _lockedAction.setEnabled(enabled);

    _profileTypeAction.setOptions(getProfileTypeNames());
    _profileTypeAction.setOptions(getBandTypeNames());
}

std::int32_t ChannelAction::getNoDimensions() const
{
    return _points->getNumDimensions();
}

std::int32_t ChannelAction::getNoPoints() const
{
    return _points->getNumPoints();
}

bool ChannelAction::isSubset() const
{
	if (_points == nullptr)
		return false;

	return !_points->indices.empty();
}

void ChannelAction::updateSpec()
{
    /*
	if (_points == nullptr)
		return;

	//qDebug() << "Updating dimensions for" << _points->getName();

	//const auto& selection = dynamic_cast<Points&>(dimensionsViewerPlugin->getCore()->requestSelection(_points->getDataName()));
	const auto& selection = dynamic_cast<Points&>(_points->getSelection());
	
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

				switch (_profileType)
				{
					case ProfileType::Mean: {
						dimension["agg"] = mean;
						break;
					}

					case ProfileType::Median: {
						std::sort(dimensionValues.begin(), dimensionValues.end());
						dimension["agg"] = dimensionValues[static_cast<int>(floorf(dimensionValues.size() / 2))];
						break;
					}

					default:
						break;
				}

				switch (_bandType)
				{
					case BandType::StandardDeviation1: {
						double sqSum	= std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
						double stdDev1	= std::sqrt(sqSum / dimensionValues.size());

						dimension["v1"] = mean - stdDev1;
						dimension["v2"] = mean + stdDev1;
						break;
					}

					case BandType::StandardDeviation2: {
						double sqSum	= std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
						double stdDev2	= 2.0 * std::sqrt(sqSum / dimensionValues.size());

						dimension["v1"] = mean - stdDev2;
						dimension["v2"] = mean + stdDev2;
						break;
					}

					default:
						break;
				}

				if (_showRange) {
					auto result = std::minmax_element(dimensionValues.begin(), dimensionValues.end());

                    dimension["min"] = *result.first;
					dimension["max"] = *result.second;
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
	_spec["profileType"]	= static_cast<int>(_profileType);
	_spec["bandType"]		= static_cast<int>(_bandType);
	_spec["showRange"]		= _showRange && pointIndices.size() > 1;
	_spec["canDisplay"]		= canDisplay();

    //emit specChanged(this);
    */
}

ChannelAction::Widget::Widget(QWidget* parent, ChannelAction* channelAction) :
    WidgetAction::Widget(parent, channelAction),
    _mainLayout()
{
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    setLayout(&_mainLayout);
    
    _mainLayout.addWidget(new StandardAction::CheckBox(this, &channelAction->_enabledAction));
    _mainLayout.addWidget(new OptionAction::Widget(this, &channelAction->_datasetNameAction, false), 1);
    _mainLayout.addWidget(new ColorAction::Widget(this, &channelAction->_colorAction, false));
    _mainLayout.addWidget(new DecimalAction::Widget(this, &channelAction->_opacityAction, DecimalAction::Widget::Configuration::SpinBoxSlider));
    _mainLayout.addWidget(new OptionAction::Widget(this, &channelAction->_profileTypeAction, false));
    _mainLayout.addWidget(new OptionAction::Widget(this, &channelAction->_bandTypeAction, false));
    _mainLayout.addWidget(new StandardAction::CheckBox(this, &channelAction->_showRangeAction));
    _mainLayout.addWidget(new StandardAction::CheckBox(this, &channelAction->_lockedAction));
}