#pragma once

#include "PluginAction.h"
#include "StylingAction.h"

#include "event/EventListener.h"

#include <QVector>
#include <QColor>
#include <QVariantMap>

class Points;
class ConfigurationAction;

/**
 * Channel class
 *
 * Class for reading/writing channel visualization properties and for synchronization with the JS Vega web view
 * 
 * @author T. Kroes
 */
class ChannelAction : public PluginAction, public hdps::EventListener {

public: // Enumerations

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
    };

    static const QMap<DifferentialProfileConfig, QString> differentialProfileConfigs;

protected: // Widget

    class Widget : public PluginAction::Widget {
    public:
        Widget(QWidget* parent, ChannelAction* channelAction);

    protected:
        QHBoxLayout     _mainLayout;
    };

    QWidget* getWidget(QWidget* parent, const Widget::State& widgetType = Widget::State::Standard) override {
        return new ChannelAction::Widget(parent, this);
    };

protected: // Construction

	/**
	 * Constructor
	 * @param configurationAction Parent configuration action
	 * @param displayName Channel name in the GUI
	 * @param profileType Type of profile
	 */
	ChannelAction(ConfigurationAction* configurationAction, const QString& displayName, const ProfileType& profileType = ProfileType::Mean);

public: // Getters/setters

    std::uint32_t getIndex() const { return _index; }
    QString getInternalName() const { return _internalName; }
    QString getDisplayName() const { return _displayName; }
    
    bool isDifferential() const {
        return _profileTypeAction.getCurrentIndex() == static_cast<std::uint32_t>(ProfileType::Differential);
    }

public: // Actions

    ConfigurationAction* getConfiguration() { return _configuration; }

    hdps::gui::ToggleAction& getEnabledAction() { return _enabledAction; }
    hdps::gui::OptionAction& getDatasetName1Action() { return _datasetName1Action; }
    hdps::gui::OptionAction& getDatasetName2Action() { return _datasetName2Action; }
    hdps::gui::OptionAction& getProfileTypeAction() { return _profileTypeAction; }
    hdps::gui::OptionAction& getBandTypeAction() { return _profileConfigAction; }
    StylingAction& getStylingAction() { return _stylingAction; }
    

public: // Visualization spec

	QVariantMap getSpec() { return _spec; };
    bool canDisplaySpec() const;

private: // Miscellaneous
	
	/** Returns if the referenced dataset is a subset */
	bool isSubset() const;

	/** Updates the visualization specification */
	void updateSpec(const bool& ignoreDimensions = false);

protected:
	const std::uint32_t		    _index;				            /** Channel index */
	const QString			    _internalName;		            /** Channel internal name (e.g. channel1, channel2) */
	const QString			    _displayName;		            /** Channel display name (e.g. dataset, Subset1 and Subset 2) */
    ConfigurationAction*        _configuration;
    hdps::gui::ToggleAction     _enabledAction;
    hdps::gui::OptionAction     _datasetName1Action;
    hdps::gui::OptionAction     _datasetName2Action;
    hdps::gui::OptionAction     _profileTypeAction;
    hdps::gui::OptionAction     _profileConfigAction;
    StylingAction               _stylingAction;
    QVariantMap				    _spec;				            /** Specification for use in JS visualization client (Vega) */

    friend class ConfigurationAction;
    friend class StylingAction;
};