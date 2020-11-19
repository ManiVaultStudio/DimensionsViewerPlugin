#pragma once

#include "Item.h"

#include <QObject>
#include <QMap>

class Channel;

/**
 * Differential tree item class
 *
 * @author T. Kroes
 */
class Differential : public tree::Item {

public: // Columns and rows

    /** Tree item columns */
    enum class Column {

        /** Derived tree item columns */
        Type,                           /** Type of tree item */
        Name,                           /** Name of tree item */
        Enabled,                        /** Whether the tree item is enabled or not */
        Modified,                       /** Last modified integer stamp */
        UUID,                           /** Universal unique identifier */

        /** Differential tree item columns */
        Differential,                   /** Differential */
        DifferentialOperandNamesA,      /** Candidate channel names for operand A */
        DifferentialOperandNamesB,      /** Candidate channel names for operand B */
        DifferentialOperandA,           /** Selected channel name for operand A */
        DifferentialOperandB,           /** Selected channel name for operand B */

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
	Differential(Item* parent);

public: // TreeItem: visitor API

    /** Accept visitor */
    void accept(tree::Visitor* visitor) const override;

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
    QMap<Operand, QStringList>  _operandChannelNames;       /** Operand channel names (selectable channel name(s) in dropdown UI) */
    QMap<Operand, QString>      _operandChannelName;        /** Operand channel name (selected channel name in dropdown UI) */

    friend class Channel;
};