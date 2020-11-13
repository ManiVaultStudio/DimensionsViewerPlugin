#pragma once

#include "ModelItem.h"
#include "Profile.h"
#include "Differential.h"
#include "Styling.h"

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

public: // Columns and rows

    /** Model item columns */
    enum class Column {
        Type,
        Index,
        InternalName,
        DisplayName,
        Enabled,
        DatasetNames,
        DatasetName,

        _ProfileStart = DatasetName,                    /** Start of the profile section */
            ProfileTypes,                               /** Available profile types */
            ProfileType,                                /** Current profile type */
            RangeTypes,                                 /** Available range types */
            RangeType,                                  /** Current range type */
        _ProfileEnd = RangeType,                        /** End of the profile section */

        _DifferentialStart = _ProfileEnd,               /** Start of the differential section */
            Differential,                               /** TODO */
            DifferentialOperandNames1,                  /** TODO */
            DifferentialOperandNames2,                  /** TODO */
            DifferentialOperandName1,                   /** TODO */
            DifferentialOperandName2,                   /** TODO */
        _DifferentialEnd = DifferentialOperandName2,    /** End of the differential section */

        _StylingStart = _DifferentialEnd,               /** Start of the styling section */
            Styling,                                    /** TODO */
            LineTypes,                                  /** Line types */
            LineTypeProfile,                            /** Line type for drawing data profile */
            LineTypeRange,                              /** Line type for drawing data range */
            Opacity,                                    /** Opacity */
            Color,                                      /** Color */
        _StylingEnd = Color,                            /** End of the styling section */

        Linked,                                         /** TODO */

        _PointsStart = Linked,                          /** TODO */
            NoDimensions,                               /** TODO */
            NoPoints,                                   /** TODO */
        _PointsEnd = NoPoints,                          /** TODO */

        _Start  = Type,
        _End    = _PointsEnd,
        _Count  = _End + 1
    };

    /** Maps column name to column enum */
    static QMap<QString, Column> const columns;

    /** Get column name from column enum */
    static QString getColumnTypeName(const Column& column) {
        return columns.key(column);
    }

    /** Get column enum from column name */
    static Column getColumnTypeEnum(const QString& columnName) {
        return columns[columnName];
    }

    /** Model item rows */
    enum class Row {
        Profile,
        Styling,

        _Start  = Profile,
        _End    = Styling,
        _Count  = _End + 1
    };


    /** Maps row name to row enum */
    static QMap<QString, Row> const rows;

    /** Get row name from row enum */
    static QString getRowTypeName(const Row& row) {
        return rows.key(row);
    }

    /** Get row enum from row name */
    static Row getRowTypeEnum(const QString& rowName) {
        return rows[rowName];
    }

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
	Channel(ModelItem* parent, const std::uint32_t& index, const QString& displayName, const bool& enabled, const QString& datasetName, const Profile::ProfileType& profileType, const QColor& color, const float& opacity = 1.0f);

public: // ModelIndex: Model

    /** Returns the number of columns in the item */
    int columnCount() const override;

    /**
     * Returns the item flags for the given model index
     * @param index Model index
     * @return Item flags for the index
     */
    Qt::ItemFlags getFlags(const QModelIndex& index) const override;

    /**
     * Get data role
     * @param column Column to fetch data from
     * @param role Data role
     * @return Data in variant form
     */
    QVariant getData(const std::int32_t& column, const std::int32_t& role) const override;

    /**
     * Get data role
     * @param column Column to fetch data from
     * @param role Data role
     * @return Data in variant form
     */
    QVariant getData(const Column& column, const std::int32_t& role) const;

    /**
     * Sets data
     * @param index Model index
     * @param value Data value in variant form
     * @param role Data role
     * @return Model indices that are affected by the operation
     */
    QModelIndexList setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role = Qt::EditRole) override;

public: // Points data functions

    /** Returns the number of dimensions */
    std::int32_t getNoDimensions() const;

    /** Returns the number of points */
    std::int32_t getNoPoints() const;

    /** Resolved points data based on points dataset name */
    void resolvePoints();

protected: // Miscellaneous
	
    /** Get parent channels model item */
    const Channels* getChannels() const;

    /** Get channels at row */
    const Channel* getSibling(const std::int32_t& row) const;

    Channel* getSibling(const std::int32_t& row);

    /** Returns whether the channel can be displayed in the viewer */
    bool canDisplay() const;

	/** Returns if the referenced dataset is a subset */
	bool isSubset() const;

public: // Getters

    /** Get profile settings */
    Profile& getProfile() {
        return _profile;
    }

    /** Get differential settings */
    Differential& getDifferential() {
        return _differential;
    }

    /** Get styling settings */
    Styling& getStyling() {
        return _styling;
    }

protected: // Spec

	/** Updates the visualization specification */
	void updateSpec();

    /** Returns the visualization specification */
    QVariantMap getSpec();

signals:

    /** Signals that the channel spec has changed */
    void specChanged(Channel* channel);

private:
	const std::uint32_t		_index;				        /** Index */
	const QString			_internalName;		        /** Internal name (e.g. channel1, channel2) */
	QString			        _displayName;		        /** Display name (e.g. dataset, Subset1 and Subset 2) */
	bool					_enabled;			        /** Whether the channel is enabled or not */
	bool					_linked;                    /** Whether settings are linked to the settings of the first channel */
	QStringList             _datasetNames;		        /** Dataset names */
	QString					_datasetName;		        /** Dataset name */
    Profile				    _profile;		            /** Profile settings */
    Differential            _differential;              /** Differential settings */
    Styling				    _styling;		            /** Style settings */
	QVariantMap				_spec;				        /** Specification for use in JS visualization client (Vega) */
	Points*					_points;			        /** Pointer to points dataset */

protected:
	friend class Channels;
	friend class Differential;
};