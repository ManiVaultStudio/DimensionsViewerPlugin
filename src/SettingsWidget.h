#pragma once

#include "Common.h"

#include <memory>

#include <QStackedWidget.h>

class DimensionsViewerPlugin;

namespace Ui {
	class SettingsWidget;
}

class SettingsWidget : public QWidget
{
	Q_OBJECT

public:
	SettingsWidget(DimensionsViewerPlugin* histogramViewerPlugin);

public:

	/**
	 * Updates the UI with model indices ranging from \p begin to \p end
	 * @param begin Start of model index range
	 * @param end End of model index range
	 * @param roles Data roles
	 */
	void updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles = QVector<int>());

signals:
	void datasetChanged(const QString& dataset);

private:
	DimensionsViewerPlugin*					_dimensionsViewerPlugin;
	std::unique_ptr<Ui::SettingsWidget>		_ui;
};
