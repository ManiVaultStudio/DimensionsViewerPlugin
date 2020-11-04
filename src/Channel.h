#pragma once

#include "ModelItem.h"
#include "Profile.h"

#include <QObject>
#include <QVector>
#include <QColor>
#include <QVariantMap>

class Points;
class DimensionsViewerPlugin;
class Configuration;

/**
 * Channel model item class
 *
 * @author T. Kroes
 */
class Channel : public ModelItem {

	Q_OBJECT

protected: // Construction

	/**
	 * Constructor
	 * @param parent Parent model item
	 * @param index Channel index
	 * @param displayName Channel name in the user interface
	 * @param enabled Whether the channel is enabled
	 * @param datasetName The name of the channel dataset
	 * @param dataName The data name of the channel
	 * @param color The color of the channel
	 * @param opacity Render opacity
	 */
	Channel(ModelItem* parent, const std::uint32_t& index, const QString& displayName, const bool& enabled, const QString& datasetName, const QString& dataName, const QColor& color, const float& opacity = 1.0f);

public: // ModelIndex: MVC

    /**
     * Returns the number of rows in the model
     * @param parentIndex Parent index
     */
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    /** Returns the number of columns */
    int getColumnCount() const override;

    /**
     * Returns the item flags for the given model index
     * @param index Model index
     * @return Item flags for the index
     */
    Qt::ItemFlags getFlags(const QModelIndex& index) const override;

    /**
     * Returns the data for the given model index and data role
     * @param index Model index
     * @param role Data role
     * @return Data in variant form
     */
    QVariant getData(const QModelIndex& index, const int& role) const override;

    /**
     * Sets the data value for the given model index and data role
     * @param index Model index
     * @param value Data value in variant form
     * @param role Data role
     * @return Model indices that are affected by the operation
     */
    QModelIndexList setData(const QModelIndex& index, const QVariant& value, const int& role) override;

    /**
     * Returns the model index belonging to the given model row and column
     * @param row Model row
     * @param column Model column
     * @param parent Parent model index
     * @return Model index for the given model row and column
     */
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;

    /**
     * Returns the parent model index
     * @param index Model index
     * @return Parent model index for the given model index
     */
    QModelIndex parent(const QModelIndex& index) const override;

public: // ModelIndex: Hierarchy

    /**
     * Returns a model item node by index
     * @param index Index of the child model item
     * @return Model item at index
     */
    ModelItem* getChild(const int& index) const override;

    /** Returns the number of children */
    int getChildCount() const override;

public: // Getters/setters

	/** Returns the channel index */
	std::uint32_t getIndex() const {
		return _index;
	};

	/** Returns the channel internal name */
	QString getInternalName() const {
		return _internalName;
	};

	/** Returns the channel display name */
	QString getDisplayName() const {
		return _displayName;
	};

    /**
     * Sets name
     * @param displayName Display name
     */
    void setDisplayName(const QString& displayName);

	/** Returns whether the channel is enabled */
	bool isEnabled() const {
		return _enabled;
	};

	/**
	 * Sets whether the channel is enabled
	 * @param enabled Whether the channel is enabled
	 */
	void setEnabled(const bool& enabled);

	/** Returns the dataset name */
	QString getDatasetName() const {
		return _datasetName;
	};

	/**
	 * Sets the dataset name
	 * @param datasetName Name of the dataset
	 */
	void setDatasetName(const QString& datasetName);

	/** Returns the data name */
	QString getDataName() const {
		return _dataName;
	};

	/** Returns the color */
	QColor getColor() const {
		return _color;
	};

	/**
	 * Sets the color
	 * @param color Color
	 */
	void setColor(const QColor& color);

	/** Returns the render opacity */
	float getOpacity() const {
		return _opacity;
	};

	/**
	 * Sets the render opacity
	 * @param opacity Render opacity
	 */
	void setOpacity(const float& opacity);

	/** Returns the profile type */
    Profile::ProfileType getProfileType() const;

	/**
	 * Sets the profile type
	 * @param profileType Profile type
	 */
	void setProfileType(const Profile::ProfileType& profileType);

	/** Returns the range type */
    Profile::RangeType getRangeType() const;

	/**
	 * Sets the range type
	 * @param rangeType Range type
	 */
	void setRangeType(const Profile::RangeType& rangeType);

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
    void specChanged(Channel* channel);

private:
    Configuration*              _configuration;                 /** Parent configuration */
	const std::uint32_t		    _index;				            /** Channel index */
	const QString			    _internalName;		            /** Channel internal name (e.g. channel1, channel2) */
	QString			            _displayName;		            /** Channel display name (e.g. dataset, Subset1 and Subset 2) */
	bool					    _enabled;			            /** Whether the channel is enabled or not */
	QString					    _datasetName;		            /** Channel dataset name */
	QString					    _dataName;			            /** Channel data name */
	QColor					    _color;				            /** Channel color */
	float					    _opacity;			            /** Channel opacity */
    Profile				        _profile;		                /** TODO */
	QVariantMap				    _spec;				            /** Specification for use in JS visualization client (Vega) */
	Points*					    _points;			            /** Pointer to points dataset */

protected:
	friend class Channels;
};

//      ChannelNameStart,				                                    /** Channel name first column */
        //      ChannelNameEnd = ChannelNameStart + noChannels,                     /** Channel name last column */
              //ChannelEnabledStart,				                                /** Channel enabled first column */
              //ChannelEnabledEnd = ChannelEnabledStart + noChannels,               /** Channel enabled last column */
              //ChannelDatasetNameStart,			                                /** Channel dataset name first column */
              //ChannelDatasetNameEnd = ChannelDatasetNameStart + noChannels,       /** Channel dataset name last column */
              //ChannelDataNameStart,				                                /** Channel data name first column */
              //ChannelDataNameEnd = ChannelDataNameStart + noChannels,             /** Channel data name last column */
              //ChannelColorStart,					                                /** Channel color first column */
              //ChannelColorEnd = ChannelColorStart + noChannels,                   /** Channel color last column */
              //ChannelOpacityStart,				                                /** Channel opacity first column */
              //ChannelOpacityEnd = ChannelOpacityStart + noChannels,               /** Channel opacity last column */
              //ChannelProfileTypeStart,			                                /** Channel profile type first column */
              //ChannelProfileTypeEnd = ChannelProfileTypeStart + noChannels,       /** Channel profile type last column */
        //      ChannelRangeTypeStart,				                                /** Channel range type first column */
        //      ChannelRangeTypeEnd = ChannelRangeTypeStart + noChannels,           /** Channel range type last column */
        //      ChannelSettingsStart,				                                /** Channel settings type first column */
        //      ChannelSettingsEnd = ChannelSettingsStart + noChannels,             /** Channel settings type last column */