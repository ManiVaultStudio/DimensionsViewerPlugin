#include "DimensionsViewerWidget.h"
#include "DimensionsViewerPlugin.h"

#include <QFileInfo>
#include <QWebChannel>

DimensionsViewerWidget::DimensionsViewerWidget(DimensionsViewerPlugin* dimensionsViewerPlugin) :
	QWebEngineView(),
	_dimensionsViewerPlugin(dimensionsViewerPlugin)
{
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

	load(QUrl("qrc:DimensionsViewer.html"));

	QWebChannel* webChannel = new QWebChannel(this);

	auto& configurationsModel = _dimensionsViewerPlugin->getConfigurationsModel();

	QObject::connect(&configurationsModel.getSelectionModel(), &QItemSelectionModel::selectionChanged, [this, &configurationsModel, webChannel](const QItemSelection& selected, const QItemSelection& deselected) {
		const auto selectedRows = configurationsModel.getSelectionModel().selectedRows();

		if (!selectedRows.isEmpty()) {
			for (auto registeredObject : webChannel->registeredObjects())
				webChannel->deregisterObject(registeredObject);

			auto selectedConfiguration = configurationsModel.getSelectedConfiguration();

			for (auto& channel : selectedConfiguration->getChannels()) {
				webChannel->registerObject(channel->getName(), channel);
			}
		}
	});

	page()->setWebChannel(webChannel);
}