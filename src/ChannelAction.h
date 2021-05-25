#pragma once

#include "PluginAction.h"

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
class ChannelAction : public PluginAction {

public: // Enumerations

	/** Profile type (e.g. mean and median) */
	enum class ProfileType {
		Mean,               /** Display statistical mean */
		Median,             /** Display statistical median */
		Differential,       /** Display differential profile (difference between two profiles) */

		End = Differential
	};

	/** Get string representation of profile type enumeration */
	static QString getProfileTypeName(const ProfileType& profileType) {
		switch (profileType) {
			case ProfileType::Mean:
				return "Mean";

			case ProfileType::Median:
				return "Median";

            case ProfileType::Differential:
                return "Differential";

            default:
                return QString();
		}

		return QString();
	}

	/** Get profile type names in a string list */
	static QStringList getProfileTypeNames() {
		QStringList profileTypeNames;

		for (int i = 0; i <= static_cast<int>(ProfileType::End); ++i)
			profileTypeNames << getProfileTypeName(static_cast<ProfileType>(i));

		return profileTypeNames;
	}

	/** Band type (e.g. standard deviation) */
	enum class BandType {
		None,                   /** Do not display standard deviation */
		StandardDeviation1,     /** Display one standard deviation */
		StandardDeviation2,     /** Display two standard deviations */

		End = StandardDeviation2
	};

	/** Get string representation of band type enumeration */
	static QString getBandTypeName(const BandType& bandType) {
		switch (bandType) {
			case BandType::None:
				return "None";

			case BandType::StandardDeviation1:
				return "1 SD";

			case BandType::StandardDeviation2:
				return "2 SD";
		}

		return QString();
	}

	/** Get band type names in a string list */
	static QStringList getBandTypeNames() {
		QStringList bandTypeNames;

		for (int i = 0; i <= static_cast<int>(BandType::End); ++i)
			bandTypeNames << getBandTypeName(static_cast<BandType>(i));

		return bandTypeNames;
	}

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
    hdps::gui::OptionAction& getProfileTypeAction() { return _profileTypeAction; }
    hdps::gui::OptionAction& getDatasetName1Action() { return _datasetName1Action; }
    hdps::gui::OptionAction& getDatasetName2Action() { return _datasetName2Action; }
    hdps::gui::ColorAction& getColorAction() { return _colorAction; }
    hdps::gui::DecimalAction& getOpacityAction() { return _opacityAction; }
    hdps::gui::OptionAction& getBandTypeAction() { return _bandTypeAction; }
    hdps::gui::StandardAction& getShowRangeAction() { return _showRangeAction; }
    hdps::gui::StandardAction& getLockedAction() { return _lockedAction; }

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

signals:

    /** Signals that the channel spec has changed */
    //void specChanged(Channel* channel);

    

protected:
	const std::uint32_t		    _index;				            /** Channel index */
	const QString			    _internalName;		            /** Channel internal name (e.g. channel1, channel2) */
	const QString			    _displayName;		            /** Channel display name (e.g. dataset, Subset1 and Subset 2) */
    ConfigurationAction*        _configuration;
    hdps::gui::StandardAction   _enabledAction;
    hdps::gui::OptionAction     _datasetName1Action;
    hdps::gui::OptionAction     _datasetName2Action;
    hdps::gui::OptionAction     _profileTypeAction;
    hdps::gui::ColorAction      _colorAction;
    hdps::gui::DecimalAction    _opacityAction;
    hdps::gui::OptionAction     _bandTypeAction;
    hdps::gui::StandardAction   _showRangeAction;
    hdps::gui::StandardAction   _lockedAction;
    QVariantMap				    _spec;				            /** Specification for use in JS visualization client (Vega) */
    Points*					    _points1;			            /** Pointer to points dataset */
    Points*					    _points2;			            /** Pointer to points dataset */

    friend class ConfigurationAction;
};