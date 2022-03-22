#include "ChannelAction.h"
#include "SettingsAction.h"
#include "DimensionsViewerPlugin.h"
#include "Layer.h"

#include <Application.h>
#include <PointData.h>
#include <event/Event.h>

#include <QDebug>
#include <QVariantList>
#include <QComboBox>
#include <QUuid>

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
    { ChannelAction::DifferentialProfileConfig::Max, "Max" },
    { ChannelAction::DifferentialProfileConfig::AbsMean, "Mean (absolute)" },
    { ChannelAction::DifferentialProfileConfig::AbsMedian, "Median (absolute)" },
    { ChannelAction::DifferentialProfileConfig::AbsMin, "Min (absolute)" },
    { ChannelAction::DifferentialProfileConfig::AbsMax, "Max (absolute)" }
});

ChannelAction::ChannelAction(Layer& layer, const QString& displayName, const ProfileType& profileType /*= ProfileType::Mean*/) :
    WidgetAction(&layer),
    _layer(layer),
    _index(0),
    _internalName(QUuid::createUuid().toString(QUuid::WithoutBraces)),
    _displayName(displayName),
    _removeAction(this, "Remove"),
    _enabledAction(this, _displayName),
    _dataset1Action(this, "Dataset 1"),
    _dataset2Action(this, "Dataset 2"),
    _profileTypeAction(this, "Profile type"),
    _profileConfigAction(this, "Profile configuration"),
    _useSelectionAction(this, "Selection"),
    _stylingAction(layer, this),
    _spec()
{
    setText("Settings");

    _removeAction.setDefaultWidgetFlags(TriggerAction::Icon);
    _removeAction.setIcon(Application::getIconFont("FontAwesome").getIcon("trash-alt"));

    _enabledAction.setChecked(false);

    _dataset1Action.setDatasets({ _layer.getDatasetReference() });
    _dataset2Action.setDatasets({ _layer.getDifferentialDatasetCandidates() });

    connect(&_layer, &Layer::differentialDatasetCandidatesChanged, this, [this]() -> void {
        _dataset2Action.setDatasets(_layer.getDifferentialDatasetCandidates());
    });

    _profileTypeAction.setOptions(profileTypes.values());
    _profileTypeAction.setCurrentIndex(static_cast<std::int32_t>(profileType));

    const auto updateUI = [this](const bool& init = false) {
        const auto numDatasets      = _dataset1Action.getOptions().count();
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

        if (init)
            _profileConfigAction.setCurrentIndex(1);

        _enabledAction.setEnabled(numDatasets >= 1);
        _stylingAction.getColorAction().setEnabled(isEnabled);
        _dataset1Action.setEnabled(isEnabled && numDatasets >= 1 && _index >= 1);
        _dataset2Action.setEnabled(isEnabled && isDifferential);
        _profileTypeAction.setEnabled(isEnabled);
        _profileConfigAction.setEnabled(isEnabled);
        _useSelectionAction.setEnabled(isEnabled);
        _stylingAction.setEnabled(isEnabled);
    };

    connect(&_enabledAction, &ToggleAction::toggled, [this, updateUI](bool state) {
        updateUI();
        updateSpec();
    });

    connect(&_dataset1Action, &OptionAction::modelChanged, [this, updateUI]() {
        updateUI();
    });

    connect(&_dataset2Action, &OptionAction::modelChanged, [this, updateUI]() {
        updateUI();
    });

    connect(&_dataset1Action, &OptionAction::currentIndexChanged, [this, updateUI](const std::int32_t& currentIndex) {
        updateSpec();
    });

    connect(&_dataset2Action, &OptionAction::currentIndexChanged, [this, updateUI](const std::int32_t& currentIndex) {
        updateSpec();
    });

    connect(&_profileTypeAction, &OptionAction::currentIndexChanged, [this, updateUI](const std::int32_t& currentIndex) {
        updateUI();
        updateSpec();

        _profileConfigAction.reset();
        _profileConfigAction.setCurrentIndex(isDifferential() ? 0 : 1);
    });

    connect(&_profileConfigAction, &OptionAction::currentIndexChanged, [this, updateUI](const std::int32_t& currentIndex) {
        updateUI();
        updateSpec();
    });

    connect(&_useSelectionAction, &ToggleAction::toggled, [this, updateUI](bool state) {
        updateUI();
        updateSpec();
    });

    /* TODO
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
    */

    updateUI(true);

    updateSpec();

    // Update the visualization specification whenever the dataset selection changes
    connect(&_layer.getDatasetReference(), &Dataset<Points>::dataSelectionChanged, this, [this]() -> void {
        updateSpec();
    });
}

QStringList ChannelAction::getDimensionNames() const
{
    QStringList dimensionNames;

    if (!_dataset1Action.getCurrentDataset().isValid())
        return dimensionNames;

    for (auto dimensionName : getPrimaryDataset()->getDimensionNames())
        dimensionNames << dimensionName;

    return dimensionNames;
}

std::int32_t ChannelAction::getNumPoints() const
{
    if (!_dataset1Action.getCurrentDataset().isValid())
        return 0;

    return getPrimaryDataset()->getNumPoints();
}

std::int32_t ChannelAction::getNumDimensions() const
{
    if (!_dataset1Action.getCurrentDataset().isValid())
        return 0;

    return getPrimaryDataset()->getNumDimensions();
}

void ChannelAction::updateSpec(const bool& ignoreDimensions /*= false*/)
{
    // TODO
    //if (getChannelsAction()->getConfigurationAction()->isLoading())
        //return;

    if (!getPrimaryDataset().isValid())
        return;

    const auto showRange = !isDifferential() && _stylingAction.getShowRangeAction().isChecked();

    std::vector<std::uint32_t> indices1, indices2;

    if (!ignoreDimensions) {
        const auto getIndices = [this](Dataset<Points> points) -> std::vector<std::uint32_t> {
            if (!points.isValid())
                return std::vector<std::uint32_t>();

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

            return points->getSelection<Points>()->indices;
        };

        auto& subsamplingAction = _layer.getSettingsAction().getSubsamplingAction();
            
        const auto getSubsampledIndices = [this, getIndices, &subsamplingAction](Dataset<Points> points) -> std::vector<std::uint32_t> {
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

        indices1 = getSubsampledIndices(getPrimaryDataset()->getSourceDataset<Points>());

        if (getDifferentialDataset().isValid())
            indices2 = getSubsampledIndices(getDifferentialDataset()->getSourceDataset<Points>());

        std::vector<std::uint32_t> dimensionIndices;

        auto& dimensionsAction = _layer.getSettingsAction().getDimensionsAction();

        const auto numDimensions    = static_cast<std::int32_t>(getPrimaryDataset()->getNumDimensions());
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
            const auto noDimensions = getPrimaryDataset()->getNumDimensions();

            for (auto dimensionIndex : dimensionIndices) {
                dimensionValues1 = getPrimaryDataset()->visitFromBeginToEnd<std::vector<float>>([noDimensions, dimensionIndex, &indices1](auto begin, auto end) {
                    std::vector<float> values;

                    values.reserve(indices1.size());

                    for (const auto& index : indices1)
                        values.push_back(begin[index * noDimensions + dimensionIndex]);

                    return values;
                });

                std::sort(dimensionValues1.begin(), dimensionValues1.end());

                QVariantMap dimension;

                dimension["chn"]        = QString::number(_index);
                dimension["dimId"]      = dimensionIndex;
                dimension["dimName"]    = getDimensionNames().at(dimensionIndex);

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
                        dimensionValues2 = getDifferentialDataset()->visitFromBeginToEnd<std::vector<float>>([noDimensions, dimensionIndex, &indices2](auto begin, auto end) {
                            std::vector<float> values;

                            values.reserve(indices2.size());

                            for (const auto& index : indices2)
                                values.push_back(begin[index * noDimensions + dimensionIndex]);

                            return values;
                        });

                        std::sort(dimensionValues2.begin(), dimensionValues2.end());

                        auto agg1 = 0.0f;
                        auto agg2 = 0.0f;

                        switch (static_cast<DifferentialProfileConfig>(_profileConfigAction.getCurrentIndex()))
                        {
                            case DifferentialProfileConfig::Mean: {
                                dimension["agg"] = getMean(dimensionValues2) - getMean(dimensionValues1);
                                break;
                            }

                            case DifferentialProfileConfig::Median: {
                                dimension["agg"] = getMedian(dimensionValues2) - getMedian(dimensionValues1);
                                break;
                            }

                            case DifferentialProfileConfig::Min: {
                                dimension["agg"] = getMin(dimensionValues2) - getMin(dimensionValues1);
                                break;
                            }

                            case DifferentialProfileConfig::Max: {
                                dimension["agg"] = getMax(dimensionValues2) - getMax(dimensionValues1);
                                break;
                            }

                            case DifferentialProfileConfig::AbsMean: {
                                dimension["agg"] = fabs(getMean(dimensionValues2) - getMean(dimensionValues1));
                                break;
                            }

                            case DifferentialProfileConfig::AbsMedian: {
                                dimension["agg"] = fabs(getMedian(dimensionValues2) - getMedian(dimensionValues1));
                                break;
                            }

                            case DifferentialProfileConfig::AbsMin: {
                                dimension["agg"] = fabs(getMin(dimensionValues2) - getMin(dimensionValues1));
                                break;
                            }

                            case DifferentialProfileConfig::AbsMax: {
                                dimension["agg"] = fabs(getMax(dimensionValues2) - getMax(dimensionValues1));
                                break;
                            }

                            default:
                                break;
                        }

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
    _spec["index"]                  = _index;
    _spec["displayName"]            = _displayName;
    _spec["datasetName"]            = _dataset1Action.getCurrentText();
    _spec["color"]                  = _stylingAction.getColorAction().getColor();
    _spec["opacity"]                = _stylingAction.getOpacityAction().getValue();
    _spec["profileType"]            = _profileTypeAction.getCurrentIndex();
    _spec["bandType"]               = _profileConfigAction.getCurrentIndex();
    _spec["primaryLineType"]        = _stylingAction.getPrimaryLineTypeAction().getCurrentText();
    _spec["primaryLineThickness"]   = _stylingAction.getPrimaryLineThicknessAction().getValue();
    _spec["secondaryLineType"]      = _stylingAction.getSecondaryLineTypeAction().getCurrentText();
    _spec["secondaryLineThickness"] = _stylingAction.getSecondaryLineThicknessAction().getValue();
    _spec["showRange"]              = showRange;
    _spec["showPoints"]             = _stylingAction.getShowPointsAction().isChecked();

    _layer.getSettingsAction().setModified();
}

Dataset<Points> ChannelAction::getPrimaryDataset() const
{
    return _dataset1Action.getCurrentDataset();
}

Dataset<Points> ChannelAction::getDifferentialDataset() const
{
    return _dataset2Action.getCurrentDataset();
}

ChannelAction::Widget::Widget(QWidget* parent, ChannelAction* channelAction) :
    WidgetActionWidget(parent, channelAction, State::Standard),
    _mainLayout()
{
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    setLayout(&_mainLayout);

    _mainLayout.setMargin(0);

    auto datasetNamesWidget     = new QWidget(this);
    auto datasetName1Widget     = channelAction->_dataset1Action.createWidget(this);
    auto datasetName2Widget     = channelAction->_dataset2Action.createWidget(this);

    datasetName1Widget->findChild<QComboBox*>("ComboBox")->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    auto datasetNamesLayout = new QHBoxLayout();

    datasetNamesLayout->setMargin(0);
    datasetNamesLayout->addWidget(datasetName1Widget);
    datasetNamesLayout->addWidget(datasetName2Widget);

    datasetNamesWidget->setLayout(datasetNamesLayout);

    _mainLayout.addWidget(channelAction->_enabledAction.createWidget(this));
    _mainLayout.addWidget(channelAction->_layer.getChannelAction().getStylingAction().getColorAction().createWidget(this));
    _mainLayout.addWidget(channelAction->_profileTypeAction.createWidget(this));
    _mainLayout.addWidget(datasetNamesWidget, 1);
    _mainLayout.addWidget(channelAction->_profileConfigAction.createWidget(this));
    _mainLayout.addWidget(channelAction->_layer.getChannelAction().getStylingAction().createCollapsedWidget(this));
    _mainLayout.addWidget(channelAction->_useSelectionAction.createWidget(this));
    _mainLayout.addWidget(channelAction->_removeAction.createWidget(this));
}
