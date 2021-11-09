#pragma once

#include "PluginAction.h"
#include "StylingAction.h"

#include "event/EventListener.h"

#include <QVector>
#include <QColor>
#include <QVariantMap>
#include <QHBoxLayout>

class Points;
class ChannelsAction;

/**
 * Channel class
 *
 * Class for reading/writing channel visualization properties and for synchronization with the JS Vega web view
 * 
 * @author T. Kroes
 */
class ChannelAction : public PluginAction, public hdps::EventListener {

public:

	enum class ProfileType {
		Mean,
		Median,
		Differential,

		End = Differential
	};

    static const QMap<ProfileType, QString> profileTypes;

	enum class MeanProfileConfig {
		None,
		StandardDeviation1,
		StandardDeviation2,
	};

    static const QMap<MeanProfileConfig, QString> meanProfileConfigs;

    enum class MedianProfileConfig {
        None,
        Percentile5,
        Percentile10,
        Percentile15,
        Percentile20,
    };

    static const QMap<MedianProfileConfig, QString> medianProfileConfigs;

    enum class DifferentialProfileConfig {
        Mean,
        Median,
        Min,
        Max,
        AbsMean,
        AbsMedian,
        AbsMin,
        AbsMax,
    };

    static const QMap<DifferentialProfileConfig, QString> differentialProfileConfigs;

protected:
    class Widget : public hdps::gui::WidgetActionWidget {
    public:
        Widget(QWidget* parent, ChannelAction* channelAction);

    protected:
        QHBoxLayout     _mainLayout;
    };

    QWidget* getWidget(QWidget* parent, const std::int32_t& widgetFlags) override {
        return new ChannelAction::Widget(parent, this);
    };

protected:
	ChannelAction(ChannelsAction* channelsAction, const QString& displayName, const ProfileType& profileType = ProfileType::Mean);

public:
    std::uint32_t getIndex() const {
        return _index;
    }
    
    QString getInternalName() const {
        return _internalName;
    }

    QString getDisplayName() const {
        return _displayName;}
    
    bool isDifferential() const {
        return _profileTypeAction.getCurrentIndex() == static_cast<std::uint32_t>(ProfileType::Differential);
    }

public:
    ChannelsAction* getChannelsAction() { return _channelsAction; }
    hdps::gui::ToggleAction& getEnabledAction() { return _enabledAction; }
    hdps::gui::OptionAction& getDatasetName1Action() { return _datasetName1Action; }
    hdps::gui::OptionAction& getDatasetName2Action() { return _datasetName2Action; }
    hdps::gui::OptionAction& getProfileTypeAction() { return _profileTypeAction; }
    hdps::gui::OptionAction& getBandTypeAction() { return _profileConfigAction; }
    StylingAction& getStylingAction() { return _stylingAction; }
    
public: // Point data wrapper
    QStringList getDimensionNames() const;
    std::int32_t getNumPoints() const;
    std::int32_t getNumDimensions() const;

public: // Vega visualization specification

    /**
     * Get Vega visualization specification for channel
     * @return Visualization spec in QT variant map form
     */
    QVariantMap getSpec() {
        return _spec;
    }

    /**
     * Returns whether the specification should be displayed
     * @return Whether the specification should be displayed
     */
    bool shouldDisplaySpec() const {
        return _enabledAction.isChecked() && !_spec["dimensions"].toList().isEmpty();
    }

    /**
     * Returns whether the channel data selection is correct
     * @return Whether the channel data selection is correct
     */
    bool isLoaded() const {
        return !_datasetName1Action.getCurrentText().isEmpty();
    }

private:
    /**
     * Updates the Vega visualization specification

     */
	void updateSpec(const bool& ignoreDimensions = false);

    Points* getPoints1() const;
    Points* getPoints2() const;

protected:
	const std::uint32_t		    _index;				            /** Channel index */
	const QString			    _internalName;		            /** Channel internal name (e.g. channel1, channel2) */
	const QString			    _displayName;		            /** Channel display name (e.g. dataset, Subset1 and Subset 2) */
    ChannelsAction*             _channelsAction;
    hdps::gui::ToggleAction     _enabledAction;
    hdps::gui::OptionAction     _datasetName1Action;
    hdps::gui::OptionAction     _datasetName2Action;
    hdps::gui::OptionAction     _profileTypeAction;
    hdps::gui::OptionAction     _profileConfigAction;
    hdps::gui::ToggleAction     _useSelectionAction;
    StylingAction               _stylingAction;
    QVariantMap				    _spec;				            /** Specification for use in JS visualization client (Vega) */

    friend class ChannelsAction;
    friend class ConfigurationAction;
    friend class StylingAction;
};