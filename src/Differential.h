#pragma once

#include <QObject>
#include <QMap>

class Channel;

/**
 * Differential utility class
 *
 * @author T. Kroes
 */
class Differential {

public: // Enumerations

    enum class Operand {
        ChannelA,
        ChannelB
    };

protected: // Construction

    /** Constructor
     * @param channel Parent channel
     */
	Differential(Channel* channel);

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