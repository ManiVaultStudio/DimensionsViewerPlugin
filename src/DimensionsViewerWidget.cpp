#include "DimensionsViewerWidget.h"
#include "DimensionsViewerPlugin.h"

#include <QFileInfo>
#include <QWebChannel>

DimensionsViewerWidget::DimensionsViewerWidget(DimensionsViewerPlugin* dimensionsViewerPlugin) :
	QWebEngineView(),
	_dimensionsViewerPlugin(dimensionsViewerPlugin)
{
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

	auto& configurationsModel = _dimensionsViewerPlugin->getConfigurationsModel();

	QObject::connect(&configurationsModel.getSelectionModel(), &QItemSelectionModel::selectionChanged, [this, &configurationsModel](const QItemSelection& selected, const QItemSelection& deselected) {
		const auto selectedRows = configurationsModel.getSelectionModel().selectedRows();

		if (!selectedRows.isEmpty()) {
			QWebChannel* webChannel = new QWebChannel(this);

			for (auto registeredObject : webChannel->registeredObjects())
				webChannel->deregisterObject(registeredObject);

			auto selectedConfiguration = configurationsModel.getSelectedConfiguration();

			webChannel->registerObject("configuration", selectedConfiguration);
			
			page()->setWebChannel(webChannel);

			load(QUrl("qrc:DimensionsViewer.html"));
		}
	});
}