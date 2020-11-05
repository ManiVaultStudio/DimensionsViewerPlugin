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

public: // Columns

    /** Column types */
    enum Column {
        Index,
        InternalName,
        DisplayName,
        Enabled,
        DatasetNames,
        DatasetName,
        DataName,
        Color,
        Opacity,
        ProfileType,
        RangeType,
        Settings,

        Start = Index,
        End = Settings
    };

    /** Maps column type name to column type enum and vice versa */
    static QMap<QString, Column> const columns;

    /** Get string representation of column type enumeration */
    static QString getColumnTypeName(const Column& column) {
        return columns.key(column);
    }

    /** Get enum representation from column type name */
    static Column getColumnTypeEnum(const QString& columnName) {
        return columns[columnName];
    }

    /** Get/set data roles */
    static QMap<Column, std::function<QVariant(Channel* channel)>> const getEditRoles;
    static QMap<Column, std::function<QVariant(Channel* channel)>> const getDisplayRoles;
    static QMap<Column, std::function<QModelIndexList(Channel* channel, const QVariant& value, const QModelIndex& index)>> const setEditRoles;

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

public: // ModelIndex: Hierarchy

    /**
     * Returns a model item node by index
     * @param index Index of the child model item
     * @return Model item at index
     */
    ModelItem* getChild(const int& index) const override;

    /** Returns the number of children */
    int getChildCount() const override;

    /**
     * Returns the child index
     * @param child Pointer to child model item
     */
    int getChildIndex(ModelItem* child) const override;

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
    Configuration*          _configuration;     /** Parent configuration */
	const std::uint32_t		_index;				/** Index */
	const QString			_internalName;		/** Internal name (e.g. channel1, channel2) */
	QString			        _displayName;		/** Display name (e.g. dataset, Subset1 and Subset 2) */
	bool					_enabled;			/** Whether the channel is enabled or not */
	QStringList             _datasetNames;		/** Dataset names */
	QString					_datasetName;		/** Dataset name */
	QString					_dataName;			/** Data name */
	QColor					_color;				/** Color */
	float					_opacity;			/** Opacity */
    Profile				    _profile;		    /** Profile */
	QVariantMap				_spec;				/** Specification for use in JS visualization client (Vega) */
	Points*					_points;			/** Pointer to points dataset */

protected:
	friend class Channels;
};

/**
     * Gets channel enabled
     * @param channelIndex Index of the channel
     * @param role Data role
     * @return Channel enabled in variant form
     */
     //QVariant isChannelEnabled(const std::int32_t& channelIndex, const std::int32_t& role) const;

    /**
     * Sets channel enabled
     * @param channelIndex Index of the channel
     * @param enabled Channel enabled for profile with \p profileIndex
     * @return Columns that are affected by the operation
     */
     //AffectedColumns setChannelEnabled(const std::int32_t& channelIndex, const bool& enabled);

    /**
     * Gets channel name
     * @param channelIndex Index of the channel
     * @param role Data role
     * @return Channel name
     */
     //QVariant getChannelName(const std::int32_t& channelIndex, const std::int32_t& role) const;

    /**
     * Sets channel name
     * @param channelIndex Index of the channel
     * @param name Channel name
     * @return Columns that are affected by the operation
     */
     //AffectedColumns setChannelName(const std::int32_t& channelIndex, const QString& name);



     /**
      * Gets channel dataset name
      * @param channelIndex Index of the channel
      * @param role Data role
      * @return Channel dataset name in variant form
      */
      //QVariant getChannelDatasetName(const std::int32_t& channelIndex, const std::int32_t& role) const;

      /**
       * Sets channel dataset name
       * @param channelIndex Index of the channel
       * @param datasetName Channel dataset name
       * @return Columns that are affected by the operation
       */
       //AffectedColumns setChannelDatasetName(const std::int32_t& channelIndex, const QString& datasetName);

      /**
       * Gets a channel data name
       * @param channelIndex Index of the channel
       * @param role Data role
       * @return Channel data name in variant form
       */
       //QVariant getChannelDataName(const std::int32_t& channelIndex, const std::int32_t& role) const;

      /**
       * Gets channel color
       * @param channelIndex Index of the channel
       * @param role Data role
       * @return Channel color in variant form
       */
       //QVariant getChannelColor(const std::int32_t& channelIndex, const std::int32_t& role) const;

      /**
       * Sets channel color
       * @param channelIndex Index of the channel
       * @param color Channel color
       * @return Columns that are affected by the operation
       */
       //AffectedColumns setChannelColor(const std::int32_t& channelIndex, const QColor& color);

      /**
       * Gets channel opacity
       * @param channelIndex Index of the channel
       * @param role Data role
       * @return Channel opacity in variant form
       */
       //QVariant getChannelOpacity(const std::int32_t& channelIndex, const std::int32_t& role) const;

      /**
       * Sets the opacity of channel with \p channelIndex
       * @param channelIndex Index of the channel
       * @param opacity The opacity of channel with \p channelIndex
       * @return Columns that are affected by the operation
       */
       //AffectedColumns setChannelOpacity(const std::int32_t& channelIndex, const float& opacity);

      /**
       * Gets channel profile type
       * @param channelIndex Index of the channel
       * @param role Data role
       * @return Channel profile type in variant form
       */
       //QVariant getChannelProfileType(const std::int32_t& channelIndex, const std::int32_t& role) const;

      /**
       * Sets channel profile type
       * @param channelIndex Index of the channel
       * @param profileType Channel profile type
       * @return Columns that are affected by the operation
       */
       //AffectedColumns setChannelProfileType(const std::int32_t& channelIndex, const Profile::ProfileType& profileType);

      /**
       * Gets channel range type
       * @param channelIndex Index of the channel
       * @param role Data role
       * @return Channel range type in variant form
       */
       //QVariant getChannelRangeType(const std::int32_t& channelIndex, const std::int32_t& role) const;

      /**
       * Sets channel range type
       * @param channelIndex Index of the channel
       * @param rangeType Channel range type
       * @return Columns that are affected by the operation
       */
       //AffectedColumns setChannelRangeType(const std::int32_t& channelIndex, const Profile::RangeType& rangeType);