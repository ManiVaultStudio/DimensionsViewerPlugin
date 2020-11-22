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
class Differential : public tree::Item
{

public: // Enumerations

    /** Child enumeration (each child enum item maps to a child tree item) */
    enum class Child {
        Differential,                   /** Differential */
        DifferentialOperandNamesA,      /** Candidate channel names for operand A */
        DifferentialOperandNamesB,      /** Candidate channel names for operand B */
        DifferentialOperandA,           /** Selected channel name for operand A */
        DifferentialOperandB,           /** Selected channel name for operand B */

        _Start  = Differential,
        _End    = DifferentialOperandB,
        _Count  = _End + 1
    };

    /** Children set alias */
    using Children = QSet<Child>;

    /** Maps child name to child enum */
    static QMap<QString, Child> const children;

    /** Operands enumeration (input to differential profile) */
    enum class Operand {
        ChannelA,
        ChannelB
    };

protected: // Construction

    /**
     * Constructor
     * @param name Name of the item
     * @param parent Parent tree item
     */
	Differential(Item* parent, const QString& name);

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