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

const QMap<ChannelAction::MeanProfileConfig, QString> ChannelAction::meanProfileConfigs = QMap<MeanProfileConfig, QString>({
    { ChannelAction::MeanProfileConfig::None, "None" },
    { ChannelAction::MeanProfileConfig::StandardDeviation1, "Std 1" },
    { ChannelAction::MeanProfileConfig::StandardDeviation2, "Std 2" }
});

const QMap<ChannelAction::MedianProfileConfig, QString> ChannelAction::medianProfileConfigs = QMap<ChannelAction::MedianProfileConfig, QString>({
    { ChannelAction::MedianProfileConfig::None, "None" },
    { ChannelAction::MedianProfileConfig::Percentile5, "5,95th perc." },
    { ChannelAction::MedianProfileConfig::Percentile10, "10,90th perc." },
    { ChannelAction::MedianProfileConfig::Percentile15, "15,85th perc." },
    { ChannelAction::MedianProfileConfig::Percentile20, "20,80th perc." }
});

const QMap<ChannelAction::DifferentialProfileConfig, QString> ChannelAction::differentialProfileConfigs = QMap<ChannelAction::DifferentialProfileConfig, QString>({
    { ChannelAction::DifferentialProfileConfig::Mean, "Mean" },
    { ChannelAction::DifferentialProfileConfig::Median, "Median" },
    { ChannelAction::DifferentialProfileConfig::Min, "Min" },
    { ChannelAction::DifferentialProfileConfig::Max, "Max" }
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
    _profileConfigAction(this, "Profile configuration"),
    _colorAction(this, "Color"),
    _opacityAction(this, "Opacity"),
    _spec()
{
    setEventCore(_dimensionsViewerPlugin->getCore());

    _enabledAction.setCheckable(true);
    _enabledAction.setChecked(false);

    _profileTypeAction.setOptions(profileTypes.values());
    _profileTypeAction.setCurrentIndex(static_cast<std::int32_t>(profileType));
    
    const auto updateUI = [this, configurationAction]() {
        const auto numDatasets      = _datasetName1Action.getOptions().count();
        const auto isEnabled        = _enabledAction.isChecked();
        const auto isDifferential   = _profileTypeAction.getCurrentText() == profileTypes.value(ProfileType::Differential);

        switch (_profileTypeAction.getCurrentIndex())
        {
            case static_cast<std::int32_t>(ProfileType::Mean):
                _profileConfigAction.setOptions(meanProfileConfigs.values());
                break;

            case static_cast<std::int32_t>(ProfileType::Median) :
                _profileConfigAction.setOptions(medianProfileConfigs.values());
                break;
                
            case static_cast<std::int32_t>(ProfileType::Differential) :
                _profileConfigAction.setOptions(differentialProfileConfigs.values());
                break;

            default:
                break;
        }

        _enabledAction.setEnabled(numDatasets >= 1);
        _datasetName1Action.setEnabled(isEnabled && numDatasets >= 1);
        _datasetName2Action.setEnabled(isEnabled && numDatasets >= 2);
        _colorAction.setEnabled(isEnabled);
        _opacityAction.setEnabled(isEnabled);
        _profileTypeAction.setEnabled(isEnabled);
        _profileConfigAction.setEnabled(isEnabled);
        _datasetName2Action.setVisible(isDifferential);
    };

    connect(&_enabledAction, &StandardAction::toggled, [this, updateUI](bool state) {
        updateUI();
        updateSpec();
    });

    connect(&_datasetName1Action, &OptionAction::optionsChanged, [this, updateUI](const QStringList& options) {
        updateUI();
    });

    connect(&_datasetName2Action, &OptionAction::optionsChanged, [this, updateUI](const QStringList& options) {
        updateUI();
    });

    connect(&_datasetName1Action, &OptionAction::currentIndexChanged, [this, updateUI](const std::int32_t& currentIndex) {
        updateSpec();
    });

    connect(&_datasetName2Action, &OptionAction::currentIndexChanged, [this, updateUI](const std::int32_t& currentIndex) {
        updateSpec();
    });

    connect(&_profileTypeAction, &OptionAction::currentIndexChanged, [this, updateUI](const std::int32_t& currentIndex) {
        updateUI();
        updateSpec();
    });

    connect(&_profileConfigAction, &OptionAction::currentIndexChanged, [this, updateUI](const std::int32_t& currentIndex) {
        updateUI();
        updateSpec();
    });

    connect(&_colorAction, &ColorAction::colorChanged, [this](const QColor& color) {
        updateSpec();
    });

    connect(&_opacityAction, &DecimalAction::valueChanged, [this](const double& value) {
        updateSpec();
    });

    connect(&configurationAction->_interactiveAction, &StandardAction::toggled, [this, updateUI](bool state) {
        updateSpec();
    });

    updateUI();

    updateSpec();

    _spec["modified"] = 0;

    registerDataEventByType(PointType, [this](hdps::DataEvent* dataEvent) {
        if (dataEvent->getType() == EventType::SelectionChanged) {
            const auto datasetName = static_cast<SelectionChangedEvent*>(dataEvent)->dataSetName;
            
            if (datasetName == _datasetName1Action.getCurrentText() || datasetName == _datasetName2Action.getCurrentText())
                updateSpec();
        }
    });
}

bool ChannelAction::canDisplaySpec() const
{
    return _enabledAction.isChecked() && !_spec["dimensions"].toList().isEmpty();
}

void ChannelAction::updateSpec()
{
    Points* points1 = nullptr;
    Points* points2 = nullptr;

    const auto isInteractive = getConfiguration()->getInteractiveAction().isChecked();

	std::vector<std::uint32_t> pointIndices1, pointIndices2;

    const auto datasetName1 = _datasetName1Action.getCurrentText();
    const auto datasetName2 = _datasetName2Action.getCurrentText();

    if (!datasetName1.isEmpty()) {
        points1 = &dynamic_cast<Points&>(_dimensionsViewerPlugin->getCore()->requestData(datasetName1));

        if (isInteractive) {
            const auto& selection = dynamic_cast<Points&>(points1->getSelection());

            if (selection.indices.empty()) {
                pointIndices1.resize(points1->getNumPoints());
                std::iota(pointIndices1.begin(), pointIndices1.end(), 0);
            }
            else {
                pointIndices1 = selection.indices;
            }
        }
        else {
            if (points1->indices.empty()) {
                pointIndices1.resize(points1->getNumPoints());
                std::iota(pointIndices1.begin(), pointIndices1.end(), 0);
            }
            else {
                pointIndices1 = points1->indices;
            }
        }
    }

    if (_profileTypeAction.getCurrentIndex() == static_cast<std::int32_t>(ProfileType::Differential) && !datasetName2.isEmpty()) {
        points2 = &dynamic_cast<Points&>(_dimensionsViewerPlugin->getCore()->requestData(datasetName2));

        if (isInteractive) {
            const auto& selection = dynamic_cast<Points&>(points2->getSelection());
            
            if (selection.indices.empty()) {
                pointIndices2.resize(points2->getNumPoints());
                std::iota(pointIndices2.begin(), pointIndices2.end(), 0);
            }
            else {
                pointIndices2 = selection.indices;
            }
        }
        else {
            if (points2->indices.empty()) {
                pointIndices2.resize(points2->getNumPoints());
                std::iota(pointIndices2.begin(), pointIndices2.end(), 0);
            }
            else {
                pointIndices2 = points2->indices;
            }
        }
    }

    if (points1 == nullptr)
        return;

	std::vector<std::uint32_t> dimensionIndices;

	dimensionIndices.resize(points1->getNumDimensions());
	std::iota(dimensionIndices.begin(), dimensionIndices.end(), 0);
	
	QVariantList dimensions;

    QVector<float> dimensionValues1, dimensionValues2;

    dimensionValues1.resize(pointIndices1.size());
    dimensionValues2.resize(pointIndices2.size());

    const auto showRange = _profileConfigAction.getCurrentIndex() > 0;

    // Get minimum value of sorted float vector
    const auto getMin = [](QVector<float>& dimensions) -> float {
        return dimensions.first();
    };

    // Get minimum value of sorted float vector
    const auto getMax = [](QVector<float>& dimensions) -> float {
        return dimensions.last();
    };

    // Get mean value of sorted float vector
    const auto getMean = [](QVector<float>& dimensions) -> float {
        if (dimensions.isEmpty())
            return 0.0f;

        return std::accumulate(dimensions.begin(), dimensions.end(), 0.0) / static_cast<float>(dimensions.size());
    };

    // Get median value of sorted float vector
    const auto getMedian = [](QVector<float>& dimensions) -> float {
        return dimensions[static_cast<int>(floorf(dimensions.count() / 2.0f))];
    };

    // Get percentile value of sorted float vector
    const auto getPercentile = [](QVector<float>& dimensions, const float& percentile) -> float {
        return dimensions[static_cast<std::int32_t>(floorf(static_cast<float>(dimensions.count()) * percentile))];
    };

	if (_enabledAction.isChecked() && !pointIndices1.empty()) {
		for (auto dimensionIndex : dimensionIndices) {
			auto localPointIndex1 = 0;

            points1->visitSourceData([&, this](auto& pointData1) {
                for (const auto& pointIndex1 : pointIndices1) {
                    dimensionValues1[localPointIndex1] = pointData1[pointIndex1][dimensionIndex];
                    localPointIndex1++;
                }
            });

			QVariantMap dimension;

			dimension["chn"]		= _index;
			dimension["dimId"]		= dimensionIndex;
			dimension["dimName"]	= points1->getDimensionNames().at(dimensionIndex);

            const auto mean = getMean(dimensionValues1);

			std::vector<float> diff(dimensionValues1.size());

			std::transform(dimensionValues1.begin(), dimensionValues1.end(), diff.begin(), [mean](double x) { return x - mean; });

			switch (static_cast<ProfileType>(_profileTypeAction.getCurrentIndex()))
			{
				case ProfileType::Mean: {
					dimension["agg"] = mean;

                    switch (static_cast<MeanProfileConfig>(_profileConfigAction.getCurrentIndex()))
                    {
                        case MeanProfileConfig::StandardDeviation1: {
                            double sqSum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
                            double stdDev1 = std::sqrt(sqSum / dimensionValues1.size());

                            dimension["v1"] = mean - stdDev1;
                            dimension["v2"] = mean + stdDev1;
                            break;
                        }

                        case MeanProfileConfig::StandardDeviation2: {
                            double sqSum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
                            double stdDev2 = 2.0 * std::sqrt(sqSum / dimensionValues1.size());

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
					std::sort(dimensionValues1.begin(), dimensionValues1.end());

                    const auto noDimensions = dimensionValues1.size();

					dimension["agg"] = getMedian(dimensionValues1);

                    switch (static_cast<MedianProfileConfig>(_profileConfigAction.getCurrentIndex()))
                    {
                        case MedianProfileConfig::Percentile5: {
                            dimension["v1"] = getPercentile(dimensionValues1, 0.05f);
                            dimension["v2"] = getPercentile(dimensionValues1, 0.95f);
                            break;
                        }

                        case MedianProfileConfig::Percentile10: {
                            dimension["v1"] = getPercentile(dimensionValues1, 0.1f);
                            dimension["v2"] = getPercentile(dimensionValues1, 0.9f);
                            break;
                        }

                        case MedianProfileConfig::Percentile15: {
                            dimension["v1"] = getPercentile(dimensionValues1, 0.15f);
                            dimension["v2"] = getPercentile(dimensionValues1, 0.85f);
                            break;
                        }

                        case MedianProfileConfig::Percentile20: {
                            dimension["v1"] = getPercentile(dimensionValues1, 0.2f);
                            dimension["v2"] = getPercentile(dimensionValues1, 0.8f);
                            break;
                        }

                        default:
                            break;
                    }

					break;
				}

                case ProfileType::Differential: {
                    points2->visitSourceData([&, this](auto& pointData2) {
                        auto localPointIndex2 = 0;

                        for (const auto& pointIndex2 : pointIndices2) {
                            dimensionValues2[localPointIndex2] = pointData2[pointIndex2][dimensionIndex];
                            localPointIndex2++;
                        }

                        std::sort(dimensionValues2.begin(), dimensionValues2.end());

                        auto agg1 = 0.0f;
                        auto agg2 = 0.0f;

                        switch (static_cast<MedianProfileConfig>(_profileConfigAction.getCurrentIndex()))
                        {
                            case DifferentialProfileConfig::Mean: {
                                agg1 = getMean(dimensionValues1);
                                agg2 = getMean(dimensionValues2);
                                break;
                            }

                            case DifferentialProfileConfig::Median: {
                                agg1 = getMedian(dimensionValues1);
                                agg2 = getMedian(dimensionValues2);
                                break;
                            }

                            case DifferentialProfileConfig::Min: {
                                agg1 = getMin(dimensionValues1);
                                agg2 = getMin(dimensionValues2);
                                break;
                            }

                            case DifferentialProfileConfig::Max: {
                                agg1 = getMax(dimensionValues1);
                                agg2 = getMax(dimensionValues2);
                                
                                break;
                            }

                            default:
                                break;
                        }

                        dimension["agg"] = fabs(agg2 - agg1);

                        dimension["v1"] = dimension["agg"];
                        dimension["v2"] = dimension["agg"];
                    });
                }

				default:
					break;
			}

			if (showRange) {
				auto result = std::minmax_element(dimensionValues1.begin(), dimensionValues1.end());

                dimension["min"] = *result.first;
				dimension["max"] = *result.second;
			}

			dimensions.append(dimension);
		}
	}

    _spec["modified"]       = _spec["modified"].toInt() + 1;
	_spec["enabled"]		= _enabledAction.isChecked();
	_spec["index"]			= _index;
	_spec["displayName"]    = _displayName;
	_spec["datasetName"]	= _datasetName1Action.getCurrentText();
	_spec["dimensions"]		= dimensions;
	_spec["color"]			= _colorAction.getColor();
	_spec["opacity"]		= _opacityAction.getValue();
	_spec["profileType"]	= _profileTypeAction.getCurrentIndex();
	_spec["bandType"]		= _profileConfigAction.getCurrentIndex();
	_spec["showRange"]		= showRange;
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
    auto profileConfigWidget    = new OptionAction::Widget(this, &channelAction->_profileConfigAction, false);
    auto colorWidget            = new ColorAction::Widget(this, &channelAction->_colorAction, false);
    auto opacityWidget          = new DecimalAction::Widget(this, &channelAction->_opacityAction, DecimalAction::Widget::Configuration::Slider);

    auto datasetNamesLayout = new QHBoxLayout();

    datasetNamesLayout->setMargin(0);
    datasetNamesLayout->addWidget(datasetName1Widget);
    datasetNamesLayout->addWidget(datasetName2Widget);

    datasetNamesWidget->setLayout(datasetNamesLayout);

    profileTypeWidget->setFixedWidth(80);
    profileConfigWidget->setFixedWidth(100);

    _mainLayout.addWidget(enabledWidget);
    _mainLayout.addWidget(datasetNamesWidget, 1);
    _mainLayout.addWidget(profileTypeWidget);
    _mainLayout.addWidget(profileConfigWidget);
    _mainLayout.addWidget(colorWidget);
    _mainLayout.addWidget(opacityWidget);
}