#pragma once

#include "SpecVisitor.h"

#include <QObject>
#include <QTimer>

class DimensionsViewerPlugin;

/**
 * Spec synchronizer class
 *
 * Class for synchronizing configuration specs with the Vega JS client
 *
 * @author T. Kroes
 */
class SpecSynchronizer : public QObject
{
	Q_OBJECT

public: // Construction

    /**
     * Constructor
     * @param dimensionsViewerPlugin Pointer to an instance of the dimensions viewer plugin
     */
    SpecSynchronizer(DimensionsViewerPlugin* dimensionsViewerPlugin);

public slots: // Functions called from the Vega JS client

    /**
     * Get the spec of the current configuration
     * @param modified Modification time stamp
     */
    QVariantMap getSpec(const int& modified);

signals:

    /** Signals that the configuration selection has changed */
    void selectionChanged();

private:
	DimensionsViewerPlugin*     _dimensionsViewerPlugin;        /** Pointer to an instance of the dimensions viewer plugin */
    QTimer                      _timer;                         /** Timer */
    SpecVisitor                 _specVisitor;                   /** Spec visitor */
    const std::int32_t          _updateInterval;                /** Update interval */
};