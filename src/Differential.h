#pragma once

#include "TreeItem.h"

#include <QObject>
#include <QMap>

class Channel;

/**
 * Differential utility class
 *
 * @author T. Kroes
 */
class Differential : public TreeItem {

public: // Columns and rows

    /** Tree item columns */
    enum class Column {
        Differential,                               /** TODO */
        DifferentialOperandNamesA,                  /** TODO */
        DifferentialOperandNamesB,                  /** TODO */
        DifferentialOperandA,                       /** TODO */
        DifferentialOperandB,                       /** TODO */

        _Start  = Differential,
        _End    = DifferentialOperandB,
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

public: // Enumerations

    enum class Operand {
        ChannelA,
        ChannelB
    };

protected: // Construction

    /**
     * Constructor
     * @param parent Parent tree item
     */
	Differential(TreeItem* parent);

public: // TreeItem: model API

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

public: // TreeItem: visitor API

    /** Accept visitor */
    void accept(Visitor* visitor) const override;

public:

    /**
     * Get (selectable) channel names for \p operand
     * @param operand Operand
     * @return Channels names
     */
    QStringList getOperandChannelNames(const Operand& operand) const;

    /**
     * Get (selected) channel name for \p operand
     * @param operand Operand
     * @return Channels name
     */
    QString getOperandChannelName(const Operand& operand) const;

    /**
     * Set (selected) channel name for \p operand
     * @param operand Operand
     * @param channelName Channel name
     */
    void setOperandChannelName(const Operand& operand, const QString& channelName);

    /** Gets the number of possible combinations */
    std::int32_t getNumCombinations() const;

    /** Gets whether enough candidate channels are available for a differential channel analysis */
    bool isPrimed() const;

    /** Gets whether the differential is valid and can be visualized */
    bool isValid() const;

    /** Updates the differential inputs and determines whether the differential is valid and can be visualized */
    void update();

private:

    /** Get candidate channel names */
    QStringList getCandidateChannelNames() const;

private:
    Channel*                    _channel;                   /** Parent channel */
    QMap<Operand, QStringList>  _operandChannelNames;       /** Operand channel names (selectable channel name(s) in dropdown UI) */
    QMap<Operand, QString>      _operandChannelName;        /** Operand channel name (selected channel name in dropdown UI) */

    friend class Channel;
};