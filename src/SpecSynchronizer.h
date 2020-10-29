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

    /**
     * Get the spec of the current configuration
     * @param modified Modification time stamp
     */
    QVariantMap getSpec(const int& modified);

private:
	DimensionsViewerPlugin*     _dimensionsViewerPlugin;        /** Pointer to an instance of the dimensions viewer plugin */
};