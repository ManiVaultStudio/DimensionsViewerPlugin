#include "ChannelAction.h"
#include "ChannelsAction.h"
#include "ConfigurationAction.h"
#include "DimensionsViewerPlugin.h"

#include "Application.h"
#include "PointData.h"

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
    { ChannelAction::MeanProfileConfig::StandardDeviation1, "+/- 1 Std" },
    { ChannelAction::MeanProfileConfig::StandardDeviation2, "+/- 2 Std" }
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

ChannelAction::ChannelAction(ChannelsAction* channelsAction, const QString& displayName, const ProfileType& profileType /*= ProfileType::Mean*/) :
    PluginAction(channelsAction->getDimensionsViewerPlugin(), QString("Channel %1").arg(channelsAction->getNumChannels())),
    hdps::EventListener(),
	_index(channelsAction->getNumChannels()),
	_internalName(QString("channel_%1").arg(QString::number(channelsAction->getNumChannels()))),
	_displayName(displayName),
    _channelsAction(channelsAction),
    _enabledAction(this, _displayName),
    _datasetName1Action(this, "Dataset 1"),
    _datasetName2Action(this, "Dataset 2"),
    _profileTypeAction(this, "Profile type"),
    _profileConfigAction(this, "Profile configuration"),
    _useSelectionAction(this, "Selection"),
    _stylingAction(this),
    _cachedStylingAction(this),
    _spec()
{
    setEventCore(_dimensionsViewerPlugin->getCore());

    _enabledAction.setChecked(false);

    _profileTypeAction.setOptions(profileTypes.values());
    _profileTypeAction.setCurrentIndex(static_cast<std::int32_t>(profileType));
    
    //_useSelectionAction.setChecked(configurationAction->getNumChannels() == 0);

    _cachedStylingAction = _stylingAction;

    const auto updateUI = [this, channelsAction](const bool& init = false) {
        const auto numDatasets      = _datasetName1Action.getOptions().count();
        const auto isEnabled        = _enabledAction.isChecked();
        const auto useSelection     = _useSelectionAction.isChecked();
        const auto isDifferential   = _profileTypeAction.getCurrentText() == profileTypes.value(ProfileType::Differential);

        switch (_profileTypeAction.getCurrentIndex())
        {
            case static_cast<std::int32_t>(ProfileType::Mean) :
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

        if (init) {
            _profileConfigAction.setCurrentIndex(1);
        }

        _enabledAction.setEnabled(numDatasets >= 1);
        _stylingAction.getColorAction().setEnabled(isEnabled);
        _datasetName1Action.setEnabled(isEnabled && numDatasets >= 1);
        _datasetName2Action.setEnabled(isEnabled && numDatasets >= 2);
        _profileTypeAction.setEnabled(isEnabled);
        _profileConfigAction.setEnabled(isEnabled);
        _useSelectionAction.setEnabled(isEnabled);
        _stylingAction.setEnabled(isEnabled);

        _datasetName2Action.setVisible(isDifferential);
    };

    connect(&_enabledAction, &ToggleAction::toggled, [this, updateUI](bool state) {
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

        _profileConfigAction.setCurrentIndex(_profileTypeAction.getCurrentIndex() == static_cast<std::int32_t>(ProfileType::Differential) ? 0 : 1);
    });

    connect(&_profileConfigAction, &OptionAction::currentIndexChanged, [this, updateUI](const std::int32_t& currentIndex) {
        updateUI();
        updateSpec();
    });

    connect(&_useSelectionAction, &ToggleAction::toggled, [this, updateUI](bool state) {
        updateUI();
        updateSpec();
    });

    connect(&_stylingAction.getShowRangeAction(), &ToggleAction::toggled, [this](bool state) {
        updateSpec();
    });

    connect(&_stylingAction.getShowPointsAction(), &ToggleAction::toggled, [this](bool state) {
        updateSpec(true);
    });

    connect(&_stylingAction.getColorAction(), &ColorAction::colorChanged, [this](const QColor& color) {
        updateSpec(true);
    });

    connect(&_stylingAction.getOpacityAction(), &DecimalAction::valueChanged, [this](const double& value) {
        updateSpec(true);
    });

    connect(&_stylingAction.getPrimaryLineTypeAction(), &OptionAction::currentIndexChanged, [this](const std::int32_t& currentIndex) {
        updateSpec(true);
    });

    connect(&_stylingAction.getPrimaryLineThicknessAction(), &DecimalAction::valueChanged, [this](const double& value) {
        updateSpec(true);
    });

    connect(&_stylingAction.getSecondaryLineTypeAction(), &OptionAction::currentIndexChanged, [this](const std::int32_t& currentIndex) {
        updateSpec(true);
    });

    connect(&_stylingAction.getSecondaryLineThicknessAction(), &DecimalAction::valueChanged, [this](const double& value) {
        updateSpec(true);
    });

    auto& dimensionsAction = getChannelsAction()->getConfigurationAction()->getDimensionsAction();

    connect(&dimensionsAction.getSelectionCenterIndexAction(), &IntegralAction::valueChanged, [this](const std::int32_t& value) {
        updateSpec();
    });

    connect(&dimensionsAction.getSelectionRadiusAction(), &IntegralAction::valueChanged, [this](const std::int32_t& value) {
        updateSpec();
    });

    auto& subsamplingAction = getChannelsAction()->getConfigurationAction()->getSubsamplingAction();

    connect(&subsamplingAction, &SubsamplingAction::toggled, [this](bool state) {
        updateSpec();
    });

    connect(&subsamplingAction.getSubsamplingFactorAction(), &DecimalAction::valueChanged, [this](const double& value) {
        updateSpec();
    });

    updateUI(true);

    updateSpec();

    registerDataEventByType(PointType, [this](hdps::DataEvent* dataEvent) {
        if (dataEvent->getType() == EventType::SelectionChanged) {
            if (!_enabledAction.isChecked())
                return;

            const auto datasetName = static_cast<SelectionChangedEvent*>(dataEvent)->dataSetName;
            
            if (datasetName == _datasetName1Action.getCurrentText() || datasetName == _datasetName2Action.getCurrentText())
                updateSpec();
        }
    });
}

QStringList ChannelAction::getDimensionNames() const
{
    QStringList dimensionNames;

    const auto datasetName1 = _datasetName1Action.getCurrentText();

    if (datasetName1.isEmpty())
        return dimensionNames;

    auto points1 = getPoints1();

    for (auto dimensionName : points1->getDimensionNames())
        dimensionNames << dimensionName;

    return dimensionNames;
}

std::int32_t ChannelAction::getNumPoints() const
{
    auto points1 = getPoints1();

    if (!points1)
        return 0;

    return getPoints1()->getNumPoints();
}

std::int32_t ChannelAction::getNumDimensions() const
{
    auto points1 = getPoints1();

    if (!points1)
        return 0;

    return getPoints1()->getNumDimensions();
}

bool ChannelAction::canDisplaySpec() const
{
    return _enabledAction.isChecked() && !_spec["dimensions"].toList().isEmpty();
}

void ChannelAction::updateSpec(const bool& ignoreDimensions /*= false*/)
{
    const auto showRange = !isDifferential() && _stylingAction.getShowRangeAction().isChecked();

    std::vector<std::uint32_t> indices1, indices2;

    if (!ignoreDimensions) {
        Points* points1 = nullptr;
        Points* points2 = nullptr;

        const auto datasetName1 = _datasetName1Action.getCurrentText();
        const auto datasetName2 = _datasetName2Action.getCurrentText();

        const auto getIndices = [this](Points* points) -> std::vector<std::uint32_t> {
            if (!_useSelectionAction.isChecked()) {
                if (points->indices.empty()) {
                    std::vector<std::uint32_t> indices;

                    indices.resize(points->getNumPoints());
                    std::iota(indices.begin(), indices.end(), 0);

                    return indices;
                }
                else {
                    return points->indices;
                }
            }

            return dynamic_cast<Points&>(points->getSelection()).indices;
        };

        auto& subsamplingAction = getChannelsAction()->getConfigurationAction()->getSubsamplingAction();
            
        const auto getSubsampledIndices = [this, getIndices, &subsamplingAction](Points* points) -> std::vector<std::uint32_t> {
            auto indices = getIndices(points);

            if (subsamplingAction.shouldSubsample()) {
                std::vector<std::uint32_t> temp;

                const auto stepSize = static_cast<std::int32_t>(1.0 / subsamplingAction.getNormalizedFactor());
                temp.reserve(indices.size());

                for (int i = 0; i < indices.size(); i += stepSize)
                    temp.push_back(indices[i]);

                indices = temp;
            }

            return indices;
        };

        if (!datasetName1.isEmpty()) {
            points1 = &dynamic_cast<Points&>(DataSet::getSourceData(_dimensionsViewerPlugin->getCore()->requestData(datasetName1)));
            indices1 = getSubsampledIndices(points1);
        }

        if (_profileTypeAction.getCurrentIndex() == static_cast<std::int32_t>(ProfileType::Differential) && !datasetName2.isEmpty()) {
            points2 = &dynamic_cast<Points&>(DataSet::getSourceData(_dimensionsViewerPlugin->getCore()->requestData(datasetName2)));
            indices2 = getSubsampledIndices(points2);
        }

        if (points1 == nullptr)
            return;

        std::vector<std::uint32_t> dimensionIndices;

        auto& dimensionsAction = getChannelsAction()->getConfigurationAction()->getDimensionsAction();

        const auto numDimensions    = static_cast<std::int32_t>(points1->getNumDimensions());
        const auto selectionCenter  = dimensionsAction.getSelectionCenterIndexAction().getValue();
        const auto selectionRadius  = dimensionsAction.getSelectionRadiusAction().getValue();
        const auto dimensionStart   = std::max(0, selectionCenter - selectionRadius);
        const auto dimensionEnd     = std::min(numDimensions - 1, selectionCenter + selectionRadius);
        const auto noDimensions     = dimensionEnd - dimensionStart + 1;

        dimensionIndices.resize(noDimensions);
        std::iota(dimensionIndices.begin(), dimensionIndices.end(), dimensionStart);

        QVariantList dimensions;

        std::vector<float> dimensionValues1, dimensionValues2;

        dimensionValues1.resize(indices1.size());
        dimensionValues2.resize(indices2.size());

        // Get minimum value of sorted float vector
        const auto getMin = [](std::vector<float>& dimensions) -> float {
            return dimensions.front();
        };

        // Get minimum value of sorted float vector
        const auto getMax = [](std::vector<float>& dimensions) -> float {
            return dimensions.back();
        };

        // Get mean value of sorted float vector
        const auto getMean = [](std::vector<float>& dimensions) -> float {
            if (dimensions.empty())
                return 0.0f;

            return std::accumulate(dimensions.begin(), dimensions.end(), 0.0) / static_cast<float>(dimensions.size());
        };

        // Get median value of sorted float vector
        const auto getMedian = [](std::vector<float>& dimensions) -> float {
            return dimensions[static_cast<int>(floorf(dimensions.size() / 2.0f))];
        };

        // Get percentile value of sorted float vector
        const auto getPercentile = [](std::vector<float>& dimensions, const float& percentile) -> float {
            return dimensions[static_cast<std::int32_t>(floorf(static_cast<float>(dimensions.size()) * percentile))];
        };

        if (_enabledAction.isChecked() && !indices1.empty()) {
            const auto noDimensions = points1->getNumDimensions();

            for (auto dimensionIndex : dimensionIndices) {
                dimensionValues1 = points1->visitFromBeginToEnd<std::vector<float>>([noDimensions, dimensionIndex, &indices1](auto begin, auto end) {
                    std::vector<float> values;

                    values.reserve(indices1.size());

                    for (const auto& index : indices1)
                        values.push_back(begin[index * noDimensions + dimensionIndex]);

                    return values;
                });

                std::sort(dimensionValues1.begin(), dimensionValues1.end());

                QVariantMap dimension;

                dimension["chn"]        = _index;
                dimension["dimId"]      = dimensionIndex;
                dimension["dimName"]    = points1->getDimensionNames().at(dimensionIndex);

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
                    dimensionValues2 = points2->visitFromBeginToEnd<std::vector<float>>([noDimensions, dimensionIndex, &indices2](auto begin, auto end) {
                        std::vector<float> values;

                        values.reserve(indices2.size());

                        for (const auto& index : indices2)
                            values.push_back(begin[index * noDimensions + dimensionIndex]);

                        return values;
                    });

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

        _spec["dimensions"] = dimensions;
    }

    _spec["enabled"]                = _enabledAction.isChecked();// && !indices1.empty();
	_spec["index"]			        = _index;
	_spec["displayName"]            = _displayName;
	_spec["datasetName"]	        = _datasetName1Action.getCurrentText();
	_spec["color"]			        = _stylingAction.getColorAction().getColor();
	_spec["opacity"]		        = _stylingAction.getOpacityAction().getValue();
	_spec["profileType"]	        = _profileTypeAction.getCurrentIndex();
	_spec["bandType"]		        = _profileConfigAction.getCurrentIndex();
	_spec["primaryLineType"]        = _stylingAction.getPrimaryLineTypeAction().getCurrentText();
	_spec["primaryLineThickness"]   = _stylingAction.getPrimaryLineThicknessAction().getValue();
	_spec["secondaryLineType"]      = _stylingAction.getSecondaryLineTypeAction().getCurrentText();
	_spec["secondaryLineThickness"] = _stylingAction.getSecondaryLineThicknessAction().getValue();
	_spec["showRange"]		        = showRange;
	_spec["showPoints"]		        = _stylingAction.getShowPointsAction().isChecked();

    getChannelsAction()->getConfigurationAction()->setModified();
}

Points* ChannelAction::getPoints1() const
{
    const auto datasetName1 = _datasetName1Action.getCurrentText();

    if (datasetName1.isEmpty())
        return nullptr;

    return &dynamic_cast<Points&>(DataSet::getSourceData(_dimensionsViewerPlugin->getCore()->requestData(datasetName1)));
}

Points* ChannelAction::getPoints2() const
{
    const auto datasetName2 = _datasetName1Action.getCurrentText();

    if (datasetName2.isEmpty())
        return nullptr;

    return &dynamic_cast<Points&>(DataSet::getSourceData(_dimensionsViewerPlugin->getCore()->requestData(datasetName2)));
}

ChannelAction::Widget::Widget(QWidget* parent, ChannelAction* channelAction) :
    WidgetAction::Widget(parent, channelAction, State::Standard),
    _mainLayout()
{
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    setLayout(&_mainLayout);
    
    _mainLayout.setMargin(0);

    auto enabledWidget          = channelAction->_enabledAction.createWidget(this);
    auto colorWidget            = channelAction->getStylingAction().getColorAction().createWidget(this);
    auto datasetNamesWidget     = new QWidget(this);
    auto datasetName1Widget     = channelAction->_datasetName1Action.createWidget(this);
    auto datasetName2Widget     = channelAction->_datasetName2Action.createWidget(this);
    auto profileTypeWidget      = channelAction->_profileTypeAction.createWidget(this);
    auto profileConfigWidget    = channelAction->_profileConfigAction.createWidget(this);
    auto useSelectionWidget     = channelAction->_useSelectionAction.createWidget(this);
    auto stylingWidget          = channelAction->_stylingAction.createCollapsedWidget(this);

    auto datasetNamesLayout     = new QHBoxLayout();

    datasetNamesLayout->setMargin(0);
    datasetNamesLayout->addWidget(datasetName1Widget);
    datasetNamesLayout->addWidget(datasetName2Widget);

    datasetNamesWidget->setLayout(datasetNamesLayout);

    profileTypeWidget->setFixedWidth(80);
    profileConfigWidget->setFixedWidth(100);

    _mainLayout.addWidget(enabledWidget);
    _mainLayout.addWidget(colorWidget);
    _mainLayout.addWidget(profileTypeWidget);
    _mainLayout.addWidget(datasetNamesWidget, 1);
    _mainLayout.addWidget(profileConfigWidget);
    _mainLayout.addWidget(stylingWidget);
    _mainLayout.addWidget(useSelectionWidget);
}