#include "Dimensions.h"

#include "PointData.h"

#include <QDebug>
#include <QList>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

Dimensions::Dimensions(DimensionsViewerPlugin* dimensionsViewerPlugin) :
	QObject(),
	_dimensionsViewerPlugin(dimensionsViewerPlugin)
{
}

void Dimensions::update(Points* points, const std::vector<std::uint32_t>& selectedIndices /*= std::vector<std::uint32_t>()*/)
{
	qDebug() << "Updating dimensions for" << points->getName() << "with selection" << selectedIndices.size();

	/*
	auto jsonValues = QJsonArray();

	auto binStart = 0.f;
	auto binId = 0;

	for (const auto& bin : _bins) {
		auto jsonObject = QJsonObject();

		jsonObject.insert("key", static_cast<int>(binId));
		jsonObject.insert("count", static_cast<int>(bin));
		jsonObject.insert("bin0", binStart);
		jsonObject.insert("bin1", binStart + binSize);

		jsonValues.push_back(jsonObject);

		binStart += binSize;
		binId++;
	}

	const auto data = QString(QJsonDocument(jsonValues).toJson(QJsonDocument::Indented));

	//qDebug() << data;

	emit changed(data);
	*/
}