#include "ChannelAction.h"
#include "ConfigurationAction.h"
#include "DimensionsViewerPlugin.h"

#include "Application.h"
#include "PointData.h"

#include "util/Timer.h"

#include <QDebug>
#include <QVariantList>

using namespace hdps;
using namespace hdps::gui;

const QMap<ChannelAction::ProfileType, QString> ChannelAction::profileTypes = QMap<ChannelAction::ProfileType, QString>({
    { ChannelAction::ProfileType::Mean, "Mean" },
    { ChannelAction::ProfileType::Median, "Median" },
    { ChannelAction::ProfileType::Differential, "Differential" }
});

const QMap<ChannelAction::MeanBandType, QString> ChannelAction::meanBandTypes = QMap<MeanBandType, QString>({
    { ChannelAction::MeanBandType::StandardDeviation1, "Std 1" },
    { ChannelAction::MeanBandType::StandardDeviation1, "Std 1" }
});

const QMap<ChannelAction::MedianBandType, QString> ChannelAction::medianBandTypes = QMap<ChannelAction::MedianBandType, QString>({
    { ChannelAction::MedianBandType::Percentile5, "5-95th perc." },
    { ChannelAction::MedianBandType::Percentile10, "10-90th perc." }
});

ChannelAction::ChannelAction(ConfigurationAction* configurationAction, const ProfileType& profileType /*= ProfileType::Mean*/, const bool& lock /*= false*/) :
    PluginAction(configurationAction->getDimensionsViewerPlugin(), QString("Channel %1").arg(configurationAction->getNumChannels())),
    hdps::EventListener(),
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
    _showRangeAction(this, ""),
    _lockedAction(this, ""),
    _spec(),
    _points1(nullptr),
    _points2(nullptr)
{
    setEventCore(_dimensionsViewerPlugin->getCore());

    _enabledAction.setCheckable(true);
    _enabledAction.setChecked(false);

    _profileTypeAction.setOptions(profileTypes.values());
    _profileTypeAction.setCurrentIndex(static_cast<std::int32_t>(profileType));
    
    _showRangeAction.setCheckable(true);
    _showRangeAction.setChecked(true);

    _lockedAction.setVisible(_index > 0);
    _lockedAction.setCheckable(true);
    _lockedAction.setChecked(lock);

    const auto updateUI = [this, configurationAction]() {
        const auto numDatasets      = _datasetName1Action.getOptions().count();
        const auto isEnabled        = _enabledAction.isChecked();
        const auto isDifferential   = _profileTypeAction.getCurrentText() == profileTypes.value(ProfileType::Differential);
        const auto isLocked         = _lockedAction.isChecked();
        const auto showRange        = _showRangeAction.isChecked();

        switch (_profileTypeAction.getCurrentIndex())
        {
            case static_cast<std::int32_t>(ProfileType::Mean):
                _bandTypeAction.setOptions(meanBandTypes.values());
                break;

            case static_cast<std::int32_t>(ProfileType::Median) :
                _bandTypeAction.setOptions(medianBandTypes.values());
                break;
                
            case static_cast<std::int32_t>(ProfileType::Differential) :
                _bandTypeAction.setOptions({});
                break;

            default:
                break;
        }

        _enabledAction.setEnabled(numDatasets >= 1);
        _datasetName1Action.setEnabled(isEnabled && numDatasets >= 1);
        _datasetName2Action.setEnabled(isEnabled && numDatasets >= 2);
        _colorAction.setEnabled(isEnabled && !isLocked);
        _opacityAction.setEnabled(isEnabled && !isLocked);
        _profileTypeAction.setEnabled(isEnabled && !isLocked);
        _bandTypeAction.setEnabled(isEnabled && !isDifferential && !isLocked && showRange);
        _showRangeAction.setEnabled(isEnabled && !isDifferential && !isLocked);
        _lockedAction.setEnabled(isEnabled);

        const auto visible = configurationAction->_showAdvancedSettingsAction.isChecked();

        _datasetName2Action.setVisible(_profileTypeAction.getCurrentText() == profileTypes.value(ProfileType::Differential));
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

        try
        {
            _points1 = &dynamic_cast<Points&>(_dimensionsViewerPlugin->getCore()->requestData(currentText));
        }
        catch (const std::exception&)
        {
        }
    });

    connect(&_datasetName2Action, &OptionAction::currentTextChanged, [this](const QString& currentText) {
        if (currentText.isEmpty())
            return;
        try
        {
            _points2 = &dynamic_cast<Points&>(_dimensionsViewerPlugin->getCore()->requestData(currentText));
        }
        catch (const std::exception&)
        {

        }
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

    connect(&configurationAction->_interactiveAction, &StandardAction::toggled, [this, updateUI](bool state) {
        updateUI();
    });

    connect(&configurationAction->_showAdvancedSettingsAction, &StandardAction::toggled, [this, updateUI](bool state) {
        updateUI();
    });

    updateUI();

    registerDataEventByType(PointType, [this](hdps::DataEvent* dataEvent) {
        if (dataEvent->getType() == EventType::SelectionChanged) {
            const auto datasetName = static_cast<SelectionChangedEvent*>(dataEvent)->dataSetName;

            if (datasetName == _datasetName1Action.getCurrentText() || datasetName == _datasetName2Action.getCurrentText())
                updateSpec();
        }
    });
}

std::int32_t ChannelAction::getNoDimensions() const
{
    return _points1->getNumDimensions();
}

std::int32_t ChannelAction::getNoPoints() const
{
    return _points1->getNumPoints();
}

void ChannelAction::updateSpec()
{
    const auto isInteractive = getConfiguration()->getInteractiveAction().isChecked();

	std::vector<std::uint32_t> pointIndices1, pointIndices2;

    const auto& selection1 = dynamic_cast<Points&>(_points1->getSelection());
    const auto& selection2 = dynamic_cast<Points&>(_points2->getSelection());

    if (_points1 != nullptr) {
        if (isInteractive) {
            pointIndices1 = selection1.indices;
        }
        else {
            if (_points1->indices.empty()) {
                pointIndices1.resize(_points1->getNumPoints());
                std::iota(pointIndices1.begin(), pointIndices1.end(), 0);
            }
            else {
                pointIndices1 = _points1->indices;
            }
        }
    }

    if (_profileTypeAction.getCurrentIndex() == static_cast<std::int32_t>(ProfileType::Differential) && _points2 != nullptr) {
        if (isInteractive) {
            pointIndices2 = selection2.indices;
        }
        else {
            if (_points2->indices.empty()) {
                pointIndices2.resize(_points2->getNumPoints());
                std::iota(pointIndices2.begin(), pointIndices2.end(), 0);
            }
            else {
                pointIndices2 = _points2->indices;
            }
        }
    }

    //qDebug() << pointIndices1;
    //qDebug() << pointIndices2;
    
	std::vector<std::uint32_t> dimensionIndices;

	dimensionIndices.resize(_points1->getNumDimensions());
	std::iota(dimensionIndices.begin(), dimensionIndices.end(), 0);
	
	QVariantList dimensions;

    std::vector<float> dimensionValues;

    dimensionValues.resize(pointIndices1.size());
    
	if (_enabledAction.isChecked() && !pointIndices1.empty()) {
		_points1->visitSourceData([this, &pointIndices1, &dimensionIndices, &dimensions, &dimensionValues](auto& pointData) {
			for (auto dimensionIndex : dimensionIndices) {
				auto localPointIndex = 0;

				for (const auto& pointIndex : pointIndices1) {
					dimensionValues[localPointIndex] = pointData[pointIndex][dimensionIndex];
					localPointIndex++;
				}

				QVariantMap dimension;

				dimension["chn"]		= _index;
				dimension["dimId"]		= dimensionIndex;
				dimension["dimName"]	= _points1->getDimensionNames().at(dimensionIndex);

                const float sum     = std::accumulate(dimensionValues.begin(), dimensionValues.end(), 0.0);
				const float mean    = sum / dimensionValues.size();

				std::vector<float> diff(dimensionValues.size());

				std::transform(dimensionValues.begin(), dimensionValues.end(), diff.begin(), [mean](double x) { return x - mean; });

				switch (static_cast<ProfileType>(_profileTypeAction.getCurrentIndex()))
				{
					case ProfileType::Mean: {
						dimension["agg"] = mean;

                        switch (static_cast<ProfileType>(_bandTypeAction.getCurrentIndex()))
                        {
                            case MeanBandType::StandardDeviation1: {
                                double sqSum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
                                double stdDev1 = std::sqrt(sqSum / dimensionValues.size());

                                dimension["v1"] = mean - stdDev1;
                                dimension["v2"] = mean + stdDev1;
                                break;
                            }

                            case MeanBandType::StandardDeviation2: {
                                double sqSum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
                                double stdDev2 = 2.0 * std::sqrt(sqSum / dimensionValues.size());

                                dimension["v1"] = mean - stdDev2;
                                dimension["v2"] = mean + stdDev2;
                                break;
                            }

                            default:
                                break;
                        }

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

				if (_showRangeAction.isChecked()) {
					auto result = std::minmax_element(dimensionValues.begin(), dimensionValues.end());

                    dimension["min"] = *result.first;
					dimension["max"] = *result.second;
				}

				dimensions.append(dimension);
			}
		});
	}

	_spec["enabled"]		= _enabledAction.isChecked();
	_spec["index"]			= _index;
	_spec["displayName"]    = _displayName;
	_spec["datasetName"]	= _datasetName1Action.getCurrentText();
	_spec["dimensions"]		= dimensions;
	_spec["color"]			= _colorAction.getColor();
	_spec["opacity"]		= _opacityAction.getValue();
	_spec["profileType"]	= _profileTypeAction.getCurrentIndex();
	_spec["bandType"]		= _bandTypeAction.getCurrentIndex();
	_spec["showRange"]		= _showRangeAction.isChecked() && pointIndices1.size() > 1;
    _spec["canDisplay"]     = false;// canDisplay();

    qDebug() << _spec;

    //emit specChanged(this);
}

void ChannelAction::computeMeanSpec()
{

}

void ChannelAction::computeMedianSpec()
{

}

void ChannelAction::computeDifferentialSpec()
{

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
    auto showRangeWidget        = new StandardAction::PushButton(this, &channelAction->_showRangeAction);
    auto lockedWidget           = new StandardAction::PushButton(this, &channelAction->_lockedAction);

    auto datasetNamesLayout = new QHBoxLayout();

    datasetNamesLayout->setMargin(0);
    datasetNamesLayout->addWidget(datasetName1Widget);
    datasetNamesLayout->addWidget(datasetName2Widget);

    datasetNamesWidget->setLayout(datasetNamesLayout);

    auto lockedWidgetSizePolicy = lockedWidget->sizePolicy();
    
    lockedWidgetSizePolicy.setRetainSizeWhenHidden(true);
    
    lockedWidget->setSizePolicy(lockedWidgetSizePolicy);

    profileTypeWidget->setFixedWidth(70);
    bandTypeWidget->setFixedWidth(90);

    _mainLayout.addWidget(enabledWidget);
    _mainLayout.addWidget(datasetNamesWidget, 1);
    _mainLayout.addWidget(profileTypeWidget);
    _mainLayout.addWidget(colorWidget);
    _mainLayout.addWidget(opacityWidget);
    _mainLayout.addWidget(bandTypeWidget);
    _mainLayout.addWidget(showRangeWidget);
    _mainLayout.addWidget(lockedWidget);

    auto& fontAwesome = Application::getIconFont("FontAwesome");

    const auto updateShowRangeIcon = [showRangeWidget, channelAction, &fontAwesome]() -> void {
        showRangeWidget->setIcon(channelAction->_showRangeAction.isChecked() ? fontAwesome.getIcon("equals") : fontAwesome.getIcon("equals"));
    };

    const auto updateLockedIcon = [lockedWidget, channelAction, &fontAwesome]() -> void {
        lockedWidget->setIcon(channelAction->_lockedAction.isChecked() ? fontAwesome.getIcon("lock") : fontAwesome.getIcon("unlock"));
    };

    connect(&channelAction->_showRangeAction, &StandardAction::changed, [this, updateShowRangeIcon]() {
        updateShowRangeIcon();
    });

    updateShowRangeIcon();

    connect(&channelAction->_lockedAction, &StandardAction::changed, [this, updateLockedIcon]() {
        updateLockedIcon();
    });

    updateLockedIcon();
}