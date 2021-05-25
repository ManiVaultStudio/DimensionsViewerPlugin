#include "ChannelAction.h"
#include "ConfigurationAction.h"
#include "DimensionsViewerPlugin.h"

#include "PointData.h"

#include "util/Timer.h"

#include <QDebug>
#include <QVariantList>

using namespace hdps::gui;

ChannelAction::ChannelAction(ConfigurationAction* configurationAction, const ProfileType& profileType /*= ProfileType::Mean*/, const bool& lock /*= false*/) :
    PluginAction(configurationAction->getDimensionsViewerPlugin(), QString("Channel %1").arg(configurationAction->getNumChannels())),
	_index(configurationAction->getNumChannels()),
	_internalName(QString("channel%1").arg(QString::number(configurationAction->getNumChannels()))),
	_displayName(QString("Channel %1").arg(configurationAction->getNumChannels() + 1)),
    _configuration(configurationAction),
    _enabledAction(this, _displayName),
    _datasetName1Action(this, "Dataset 1"),
    _datasetName2Action(this, "Dataset 2"),
    _profileTypeAction(this, "Profile type"),
    _colorAction(this, "Color"),
    _opacityAction(this, "Opacity"),
    _bandTypeAction(this, "Band type"),
    _showRangeAction(this, "Show range"),
    _lockedAction(this, "Locked"),
    _spec(),
    _points1(nullptr),
    _points2(nullptr)
{
    _enabledAction.setCheckable(true);
    _enabledAction.setChecked(false);

    _profileTypeAction.setOptions(getProfileTypeNames());
    _profileTypeAction.setCurrentIndex(static_cast<std::int32_t>(profileType));

    _bandTypeAction.setOptions(getBandTypeNames());
    
    _showRangeAction.setCheckable(true);
    _showRangeAction.setChecked(true);

    _lockedAction.setVisible(_index > 0);
    _lockedAction.setCheckable(true);
    _lockedAction.setChecked(lock);

    const auto updateUI = [this, configurationAction]() {
        const auto numDatasets      = _datasetName1Action.getOptions().count();
        const auto isEnabled        = _enabledAction.isChecked();
        const auto isDifferential   = _profileTypeAction.getCurrentText() == getProfileTypeName(ProfileType::Differential);
        const auto isLocked         = _lockedAction.isChecked();
        const auto showRange        = _showRangeAction.isChecked();

        _enabledAction.setEnabled(numDatasets >= 1);
        _datasetName1Action.setEnabled(isEnabled && numDatasets >= 1);
        _datasetName2Action.setEnabled(isEnabled && numDatasets >= 2);
        _colorAction.setEnabled(isEnabled && !isLocked);
        _opacityAction.setEnabled(isEnabled && !isLocked);
        _profileTypeAction.setEnabled(isEnabled && !isLocked);
        _bandTypeAction.setEnabled(isEnabled && !isDifferential && !isLocked && showRange);
        _showRangeAction.setEnabled(isEnabled && !isDifferential && !isLocked);
        _lockedAction.setEnabled(isEnabled);

        const auto visible = configurationAction->_showAdvancedSettings.isChecked();

        _datasetName2Action.setVisible(_profileTypeAction.getCurrentText() == getProfileTypeName(ProfileType::Differential));
        _opacityAction.setVisible(visible);
        _bandTypeAction.setVisible(visible);
        _showRangeAction.setVisible(visible);
    };

    connect(&_enabledAction, &StandardAction::toggled, [this, updateUI](bool state) {
        updateUI();
    });

    connect(&_datasetName1Action, &OptionAction::currentTextChanged, [this](const QString& currentText) {
        if (currentText.isEmpty())
            return;

        _points1 = &dynamic_cast<Points&>(_dimensionsViewerPlugin->getCore()->requestData(currentText));
    });

    connect(&_datasetName2Action, &OptionAction::currentTextChanged, [this](const QString& currentText) {
        if (currentText.isEmpty())
            return;

        _points2 = &dynamic_cast<Points&>(_dimensionsViewerPlugin->getCore()->requestData(currentText));
    });

    connect(&_datasetName1Action, &OptionAction::optionsChanged, [this, updateUI](const QStringList& options) {
        updateUI();
    });

    connect(&_datasetName2Action, &OptionAction::optionsChanged, [this, updateUI](const QStringList& options) {
        updateUI();
    });

    if (_index >= 1) {
        auto& channel0 = configurationAction->getChannels()[0];

        connect(&channel0->getColorAction(), &ColorAction::colorChanged, [this, channel0](const QColor& color) {
            if (_lockedAction.isChecked())
                _colorAction.setColor(channel0->getColorAction().getColor());
        });

        connect(&channel0->getOpacityAction(), &DecimalAction::valueChanged, [this, channel0](const double& value) {
            if (_lockedAction.isChecked())
                _opacityAction.setValue(channel0->getOpacityAction().getValue());
        });

        connect(&channel0->getProfileTypeAction(), &OptionAction::currentIndexChanged, [this, channel0](const std::int32_t& currentIndex) {
            if (channel0->getProfileTypeAction().getCurrentIndex() == static_cast<std::int32_t>(ProfileType::Differential))
                return;

            if (_profileTypeAction.getCurrentIndex() == static_cast<std::int32_t>(ProfileType::Differential))
                return;

            if (_lockedAction.isChecked())
                _profileTypeAction.setCurrentIndex(channel0->getProfileTypeAction().getCurrentIndex());
        });

        connect(&channel0->getBandTypeAction(), &OptionAction::currentIndexChanged, [this, channel0](const std::int32_t& currentIndex) {
            if (_lockedAction.isChecked())
                _bandTypeAction.setCurrentIndex(channel0->getBandTypeAction().getCurrentIndex());
        });

        connect(&channel0->getShowRangeAction(), &StandardAction::toggled, [this, channel0](bool state) {
            if (_lockedAction.isChecked())
                _showRangeAction.setChecked(channel0->getShowRangeAction().isChecked());
        });
    }

    connect(&_profileTypeAction, &OptionAction::currentIndexChanged, [this, updateUI](const std::int32_t& currentIndex) {
        updateUI();
    });

    connect(&_showRangeAction, &StandardAction::toggled, [this, updateUI](bool state) {
        updateUI();
    });

    connect(&_lockedAction, &StandardAction::toggled, [this, updateUI, configurationAction](bool state) {
        updateUI();

        if (state) {
            auto& channel0 = configurationAction->getChannels()[0];

            _opacityAction.setValue(channel0->getOpacityAction().getValue());
            _profileTypeAction.setCurrentIndex(channel0->getProfileTypeAction().getCurrentIndex());
            _bandTypeAction.setCurrentIndex(channel0->getBandTypeAction().getCurrentIndex());
            _showRangeAction.setChecked(channel0->getShowRangeAction().isChecked());
        }
    });

    connect(&configurationAction->_showAdvancedSettings, &StandardAction::toggled, [this, updateUI](bool state) {
        updateUI();
    });

    updateUI();
}

std::int32_t ChannelAction::getNoDimensions() const
{
    return _points1->getNumDimensions();
}

std::int32_t ChannelAction::getNoPoints() const
{
    return _points1->getNumPoints();
}

bool ChannelAction::isSubset() const
{
	if (_points1 == nullptr)
		return false;

	return !_points1->indices.empty();
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
    
    _mainLayout.setMargin(0);

    auto enabledWidget          = new StandardAction::CheckBox(this, &channelAction->_enabledAction);
    auto datasetName1Widget     = new OptionAction::Widget(this, &channelAction->_datasetName1Action, false);
    auto datasetName2Widget     = new OptionAction::Widget(this, &channelAction->_datasetName2Action, false);
    auto datasetNamesWidget     = new QWidget(this);
    auto profileTypeWidget      = new OptionAction::Widget(this, &channelAction->_profileTypeAction, false);
    auto colorWidget            = new ColorAction::Widget(this, &channelAction->_colorAction, false);
    auto opacityWidget          = new DecimalAction::Widget(this, &channelAction->_opacityAction, DecimalAction::Widget::Configuration::Slider);
    auto bandTypeWidget         = new OptionAction::Widget(this, &channelAction->_bandTypeAction, false);
    auto showRangeWidget        = new StandardAction::CheckBox(this, &channelAction->_showRangeAction);
    auto lockedWidget           = new StandardAction::CheckBox(this, &channelAction->_lockedAction);

    auto datasetNamesLayout = new QHBoxLayout();

    datasetNamesLayout->setMargin(0);
    datasetNamesLayout->addWidget(datasetName1Widget);
    datasetNamesLayout->addWidget(datasetName2Widget);

    datasetNamesWidget->setLayout(datasetNamesLayout);

    auto lockedWidgetSizePolicy = lockedWidget->sizePolicy();
    
    lockedWidgetSizePolicy.setRetainSizeWhenHidden(true);
    
    lockedWidget->setSizePolicy(lockedWidgetSizePolicy);

    _mainLayout.addWidget(enabledWidget);
    _mainLayout.addWidget(datasetNamesWidget, 1);
    _mainLayout.addWidget(profileTypeWidget);
    _mainLayout.addWidget(colorWidget);
    _mainLayout.addWidget(opacityWidget);
    _mainLayout.addWidget(bandTypeWidget);
    _mainLayout.addWidget(showRangeWidget);
    _mainLayout.addWidget(lockedWidget);
}