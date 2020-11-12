#pragma once

#include <QObject>

class Channel;

/**
 * Differential class
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
    QStringList getChannelNames(const Operand& operand) const;

    /**
     * Get (selected) channel name for \p operand
     * @param operand Operand
     * @return Channels name
     */
    QString getChannelName(const Operand& operand) const;

    /** Get candidate channel names */
    QStringList getCandidateChannelNames() const;

    /** TODO */
    void update();

protected:
    Channel*        _channel;               /** Parent channel */
    QStringList     _operandNames[2];       /** Operand names */
    QString         _operandName[2];        /** Operand name */
    bool            _valid;                 /** TODO */

    friend class Channel;
};