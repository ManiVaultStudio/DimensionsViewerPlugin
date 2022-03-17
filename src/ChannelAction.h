#pragma once

#include "PluginAction.h"
#include "ChannelStylingAction.h"

#include <event/EventListener.h>

#include <actions/WidgetAction.h>
#include <actions/DatasetPickerAction.h>

#include <Dataset.h>

#include <QVector>
#include <QColor>
#include <QVariantMap>
#include <QHBoxLayout>

class Layer;
class Points;
class ChannelsAction;

using namespace hdps;
using namespace hdps::gui;

/**
 * Channel class
 *
 * Class for reading/writing channel visualization properties and for synchronization with the JS Vega web view
 * 
 * @author T. Kroes
 */
class ChannelAction : public WidgetAction, public hdps::EventListener {

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
        Widget(QWidget* parent, ChannelAction* channelConfigurationAction);

    protected:
        QHBoxLayout     _mainLayout;
    };

    QWidget* getWidget(QWidget* parent, const std::int32_t& widgetFlags) override {
        return new Widget(parent, this);
    };

protected:
	ChannelAction(Layer& layer, const QString& displayName, const ProfileType& profileType = ProfileType::Mean);

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
        return !_dataset1Action.getCurrentText().isEmpty();
    }

private:

    /**
     * Updates the Vega visualization specification
     * @param ignoreDimensions Whether to ignore the dimensions
     */
    void updateSpec(const bool& ignoreDimensions = false);

    /**
     * Get the first points dataset
     * @return Smart pointer to the first points dataset
     */
    Dataset<Points> getPoints1() const;

    /**
     * Get the second points dataset
     * @return Smart pointer to the second points dataset
     */
    Dataset<Points> getPoints2() const;

public: // Action getters

    ToggleAction& getEnabledAction() { return _enabledAction; }
    DatasetPickerAction& getDataset1Action() { return _dataset1Action; }
    DatasetPickerAction& getDataset2Action() { return _dataset2Action; }
    OptionAction& getProfileTypeAction() { return _profileTypeAction; }
    OptionAction& getBandTypeAction() { return _profileConfigAction; }
    ChannelStylingAction& getStylingAction() { return _stylingAction; }
    TriggerAction& getRemoveAction() { return _removeAction; }

protected:
    Layer&                      _layer;                         /** Reference to owning layer */
	const std::uint32_t         _index;                         /** Channel index */
	const QString               _internalName;                  /** Channel internal name (e.g. channel1, channel2) */
	const QString               _displayName;                   /** Channel display name (e.g. dataset, Subset1 and Subset 2) */
    hdps::gui::ToggleAction     _enabledAction;                 /** Channel on/off action */
    DatasetPickerAction         _dataset1Action;                /** Dataset picker action for the first dataset */
    DatasetPickerAction         _dataset2Action;                /** Dataset picker action for the second dataset */
    hdps::gui::OptionAction     _profileTypeAction;             /** Profile type picker action */
    hdps::gui::OptionAction     _profileConfigAction;           /** Profile configuration action */
    hdps::gui::ToggleAction     _useSelectionAction;            /** Whether to respect the selection or not */
    ChannelStylingAction        _stylingAction;
    TriggerAction               _removeAction;                  /** Channel remove action */
    QVariantMap                 _spec;                          /** Specification for use in JS visualization client (Vega) */

    friend class Layer;
    friend class ChannelStylingAction;
};