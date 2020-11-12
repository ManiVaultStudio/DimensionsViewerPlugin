#pragma once

#include <QObject>

/**
 * Differential class
 *
 * @author T. Kroes
 */
class Differential {

protected: // Construction

    /** Default constructor */
	Differential();

protected:
    QStringList     _profileDatasetNames[2];        /** Profile 1-2 dataset names (for differential profile) */
    QString         _profileDatasetName[2];         /** Profile 1-2 selected dataset name (for differential profile) */

    friend class Channel;
};