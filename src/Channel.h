#pragma once

#include "ModelItem.h"
#include "Profile.h"
#include "Global.h"

#include <QObject>
#include <QVector>
#include <QColor>
#include <QVariantMap>

class Points;
class DimensionsViewerPlugin;
class Configuration;
class Channels;

/**
 * Channel model item class
 *
 * @author T. Kroes
 */
class Channel : public ModelItem {

    Q_OBJECT

public: // Columns

    /** Data columns */
    enum class Column {
        Name,
        Index,
        InternalName,
        DisplayName,
        Enabled,
        DatasetNames,
        DatasetName,
        Color,
        Opacity,
        ProfileTypes,
        ProfileType,
        RangeTypes,
        RangeType,
        Settings,

        Start = Name,
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

public: // Get/set data roles

    static QMap<Column, std::function<QVariant(Channel* channel, const QModelIndex& index)>> const getEditRoles;
    static QMap<Column, std::function<QVariant(Channel* channel, const QModelIndex& index)>> const getDisplayRoles;
    static QMap<Column, std::function<QModelIndexList(Channel* channel, const QModelIndex& modelIndex, const QVariant& value)>> const setEditRoles;

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

    /** Returns the number of columns in the item */
    int columnCount() const override;

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

protected: // Utility methods for obtaining model indices

    /** Get configuration model index */
    static QModelIndex getConfigurationModelIndex(const QModelIndex& index);

    /** Get global model index */
    static QModelIndex getGlobalModelIndex(const QModelIndex& index);

public: // Points wrapper functions

    /** Returns the number of dimensions */
    std::int32_t getNoDimensions() const;

    /** Returns the number of points */
    std::int32_t getNoPoints() const;

protected: // Miscellaneous
	
    /** Get parent channels model item */
    Channels* getChannels();

    /** Returns whether the channel can be displayed in the viewer */
    bool canDisplay() const;

	/** Returns if the referenced dataset is a subset */
	bool isSubset() const;

	/** Updates the visualization specification */
	void updateSpec();

    /** Returns the visualization specification */
    QVariantMap getSpec();

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
	QColor					_color;				/** Color */
	float					_opacity;			/** Opacity */
    Profile				    _profile;		    /** Profile */
	QVariantMap				_spec;				/** Specification for use in JS visualization client (Vega) */
	Points*					_points;			/** Pointer to points dataset */

protected:
	friend class Channels;
};