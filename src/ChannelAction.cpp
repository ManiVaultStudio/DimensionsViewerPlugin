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
    { ChannelAction::MeanBandType::None, "None" },
    { ChannelAction::MeanBandType::StandardDeviation1, "Std 1" },
    { ChannelAction::MeanBandType::StandardDeviation2, "Std 2" }
});

const QMap<ChannelAction::MedianBandType, QString> ChannelAction::medianBandTypes = QMap<ChannelAction::MedianBandType, QString>({
    { ChannelAction::MedianBandType::None, "None" },
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
    _bandTypeAction(this, "Band type"),
    _colorAction(this, "Color"),
    _opacityAction(this, "Opacity"),
    _spec(),
    _points1(nullptr),
    _points2(nullptr)
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
        _colorAction.setEnabled(isEnabled);
        _opacityAction.setEnabled(isEnabled);
        _profileTypeAction.setEnabled(isEnabled);
        _bandTypeAction.setEnabled(isEnabled && !isDifferential);
        _datasetName2Action.setVisible(isDifferential);
    };

    connect(&_enabledAction, &StandardAction::toggled, [this, updateUI](bool state) {
        updateUI();
        updateSpec();
    });

    connect(&_datasetName1Action, &OptionAction::currentIndexChanged, [this](const std::int32_t& currentIndex) {
        updateSpec();
    });

    connect(&_datasetName2Action, &OptionAction::currentIndexChanged, [this](const std::int32_t& currentIndex) {
        updateSpec();
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

    connect(&_profileTypeAction, &OptionAction::currentIndexChanged, [this, updateUI](const std::int32_t& currentIndex) {
        updateUI();
        updateSpec();
    });

    connect(&_bandTypeAction, &OptionAction::currentIndexChanged, [this, updateUI](const std::int32_t& currentIndex) {
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
        updateUI();
        updateSpec();
    });

    updateUI();

    updateSpec();

    _spec["modified"] = 0;
}

bool ChannelAction::canDisplaySpec() const
{
    return _enabledAction.isChecked() && !_spec["dimensions"].toList().isEmpty();
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
    if (_points1 == nullptr)
        return;

    const auto isInteractive = getConfiguration()->getInteractiveAction().isChecked();

	std::vector<std::uint32_t> pointIndices1, pointIndices2;

    if (_points1 != nullptr) {
        if (isInteractive) {
            const auto& selection = dynamic_cast<Points&>(_points1->getSelection());

            if (selection.indices.empty()) {
                pointIndices1.resize(_points1->getNumPoints());
                std::iota(pointIndices1.begin(), pointIndices1.end(), 0);
            }
            else {
                pointIndices1 = selection.indices;
            }
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
            const auto& selection2 = dynamic_cast<Points&>(_points2->getSelection());
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
    
    const auto showRange = _bandTypeAction.getCurrentIndex() > 0;

	if (_enabledAction.isChecked() && !pointIndices1.empty()) {
		_points1->visitSourceData([&, this](auto& pointData) {
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

				if (showRange) {
					auto result = std::minmax_element(dimensionValues.begin(), dimensionValues.end());

                    dimension["min"] = *result.first;
					dimension["max"] = *result.second;
				}

				dimensions.append(dimension);
			}
		});
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
	_spec["bandType"]		= _bandTypeAction.getCurrentIndex();
	_spec["showRange"]		= showRange;
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
    auto bandTypeWidget         = new OptionAction::Widget(this, &channelAction->_bandTypeAction, false);
    auto colorWidget            = new ColorAction::Widget(this, &channelAction->_colorAction, false);
    auto opacityWidget          = new DecimalAction::Widget(this, &channelAction->_opacityAction, DecimalAction::Widget::Configuration::Slider);

    auto datasetNamesLayout = new QHBoxLayout();

    datasetNamesLayout->setMargin(0);
    datasetNamesLayout->addWidget(datasetName1Widget);
    datasetNamesLayout->addWidget(datasetName2Widget);

    datasetNamesWidget->setLayout(datasetNamesLayout);

    profileTypeWidget->setFixedWidth(70);
    bandTypeWidget->setFixedWidth(90);

    _mainLayout.addWidget(enabledWidget);
    _mainLayout.addWidget(datasetNamesWidget, 1);
    _mainLayout.addWidget(profileTypeWidget);
    _mainLayout.addWidget(bandTypeWidget);
    _mainLayout.addWidget(colorWidget);
    _mainLayout.addWidget(opacityWidget);
}