#pragma once

#include <QObject>

class Points;

class DimensionsViewerPlugin;

/**
 * Dimensions class
 *
 * Computes dimension statistics
 */
class Dimensions : public QObject
{
	Q_OBJECT

public: // Construction/destruction

	/**
	 * Constructor
	 * @param dimensionsViewerPlugin Pointer to the parent dimensions viewer plugin
	 */
	Dimensions(DimensionsViewerPlugin* dimensionsViewerPlugin);

public:

	/**
	 * Computes the dimension statistics on \p points
	 * @param points Pointer to points dataset
	 */
	void update(Points* points, const std::vector<std::uint32_t>& selectedIndices = std::vector<std::uint32_t>());

signals:

	/**
	 * 
	 */
	void changed(const QString& data);

private:
	DimensionsViewerPlugin*		_dimensionsViewerPlugin;
};