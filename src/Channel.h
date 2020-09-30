#pragma once

#include <QObject>
#include <QVector>
#include <QVariantMap>

class Points;

class DimensionsViewerPlugin;

/**
 */
class Channel : public QObject
{
	Q_OBJECT

public: // Construction/destruction

	/**
	 * Constructor
	 * @param dimensionsViewerPlugin Pointer to the parent dimensions viewer plugin
	 */
	Channel(const QString& name, DimensionsViewerPlugin* dimensionsViewerPlugin);

public:

	/**
	 * Computes the dimension statistics on \p points
	 * @param points Pointer to points dataset
	 */
	void update(Points* points, const std::vector<std::uint32_t>& selectedIndices = std::vector<std::uint32_t>());

	void setDataSetName(const QString& name);
	QString getDataSetName()const;

	QString getName() { return _name; }

private:

	/** Returns the indices of the selected points */
	std::vector<std::uint32_t> selectedIndices() const;

signals:

	/**
	 * 
	 */
	void changed(const QVariantList& dimensions);

private:
	QString						_name;
	DimensionsViewerPlugin*		_dimensionsViewerPlugin;
	QString						_dataSetName;
	Points*						_points;
	
};