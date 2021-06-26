#pragma once

#include <QObject>
#include <QVariantMap>

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

    /** Get the spec of the current configuration */
    QVariantMap getSpec();

    /**
     * Function called by JS web channel to let the spec synchronizer know that the dimension selection has changed
     * @param dimensionIndex Selected dimension index
     */
    void setDimensionSelection(const QVariant& dimensionIndex);

signals:

    /** 
     * Signals that the selected dimension has changed
     * @param dimensionIndex Selected dimension index
     */
    void dimensionSelectionChanged(const std::uint32_t& dimensionIndex);

private:
	DimensionsViewerPlugin*     _dimensionsViewerPlugin;        /** Pointer to an instance of the dimensions viewer plugin */
};