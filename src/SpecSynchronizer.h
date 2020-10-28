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

public:

	/**
	 * Responds to changes in the data of the configurations model and synchronizes the spec with the Vega JS client
	 * @param begin Start of model index range
	 * @param end End of model index range
	 * @param roles Data roles
	 */
	void updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles = QVector<int>());

public slots: // Functions called from the Vega JS client

    /** Get the spec of the selected configuration */
    QVariantMap getSpec();

    /** Invoked when the Vega JS client starts with the embedding process */
    void beginVegaEmbed();

    /** Invoked when the Vega JS client has finished the embedding process */
    void endVegaEmbed();

signals: // Callbacks for the Vega JS client

    /**
     * Signals that the configuration spec has changed
     * @param spec Configuration spec
     */
    void specChanged(const QVariantMap& spec);

private:
	DimensionsViewerPlugin*     _dimensionsViewerPlugin;        /** Pointer to an instance of the dimensions viewer plugin */
    QVariantMap                 _spec;                          /** The current spec */
    bool                        _vegaIsBusy;                    /** Determines whether the Vega JS client is busy or not */
};