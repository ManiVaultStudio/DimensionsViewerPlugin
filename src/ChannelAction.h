#pragma once

#include "PluginAction.h"

#include "event/EventListener.h"

#include <QStandardItem>
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

    Q_OBJECT

public: // Enumerations

	enum class ProfileType {
		Mean,
		Median,
		Differential,

		End = Differential
	};

    static const QMap<ProfileType, QString> profileTypes;

	enum class MeanBandType {
		None,
		StandardDeviation1,
		StandardDeviation2,

		End = StandardDeviation2
	};

    static const QMap<MeanBandType, QString> meanBandTypes;

    enum class MedianBandType {
        None,
        Percentile5,
        Percentile10,

        End = Percentile10
    };

    static const QMap<MedianBandType, QString> medianBandTypes;

public:
    class Widget : public PluginAction::Widget {
    public:
        Widget(QWidget* parent, ChannelAction* channelAction);

    protected:
        QHBoxLayout     _mainLayout;
    };

    QWidget* createWidget(QWidget* parent) override {
        return new Widget(parent, this);
    }

protected: // Construction

	/**
	 * Constructor
	 * @param configurationAction Parent configuration action
	 * @param index Channel index
	 * @param datasetName The name of the channel dataset
	 * @param dataName The data name of the channel
	 * @param color The color of the channel
	 * @param opacity Render opacity
	 * @param lock Whether settings are locked
	 */
	ChannelAction(ConfigurationAction* configurationAction, const ProfileType& profileType = ProfileType::Mean, const bool& lock = false);

public: // Getters/setters

    QString getInternalName() const { return _internalName; }

    QString getDisplayName() const { return _displayName; }

    ConfigurationAction* getConfiguration() { return _configuration; }


    
    QString getDatasetName() const { return _datasetName1Action.getCurrentText(); }
    void setDatasetName(const QString& datasetName) { _datasetName1Action.setCurrentText(datasetName); }

    hdps::gui::StandardAction& getEnabledAction() { return _enabledAction; }
    hdps::gui::OptionAction& getDatasetName1Action() { return _datasetName1Action; }
    hdps::gui::OptionAction& getDatasetName2Action() { return _datasetName2Action; }
    hdps::gui::OptionAction& getProfileTypeAction() { return _profileTypeAction; }
    hdps::gui::OptionAction& getBandTypeAction() { return _bandTypeAction; }
    hdps::gui::ColorAction& getColorAction() { return _colorAction; }
    hdps::gui::DecimalAction& getOpacityAction() { return _opacityAction; }

    /** Returns whether the channel can be displayed in the viewer */
    bool canDisplay() const;

	/** Returns the visualization specification */
	QVariantMap getSpec() {
		return _spec;
	};

public: // Points wrapper functions

    /** Returns the number of dimensions */
    std::int32_t getNoDimensions() const;

    /** Returns the number of points */
    std::int32_t getNoPoints() const;

private: // Miscellaneous
	
	/** Returns if the referenced dataset is a subset */
	bool isSubset() const;

	/** Updates the visualization specification */
	void updateSpec();

    void computeMeanSpec();
    void computeMedianSpec();
    void computeDifferentialSpec();

signals:

    /** Signals that the channel spec has changed */
    void specChanged(ChannelAction* channel);

protected:
	const std::uint32_t		    _index;				            /** Channel index */
	const QString			    _internalName;		            /** Channel internal name (e.g. channel1, channel2) */
	const QString			    _displayName;		            /** Channel display name (e.g. dataset, Subset1 and Subset 2) */
    ConfigurationAction*        _configuration;
    hdps::gui::StandardAction   _enabledAction;
    hdps::gui::OptionAction     _datasetName1Action;
    hdps::gui::OptionAction     _datasetName2Action;
    hdps::gui::OptionAction     _profileTypeAction;
    hdps::gui::OptionAction     _bandTypeAction;
    hdps::gui::ColorAction      _colorAction;
    hdps::gui::DecimalAction    _opacityAction;
    QVariantMap				    _spec;				            /** Specification for use in JS visualization client (Vega) */
    Points*					    _points1;			            /** Pointer to points dataset */
    Points*					    _points2;			            /** Pointer to points dataset */

    friend class ConfigurationAction;
};