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

	QString getCurrentDatasetName() const;

signals:
	void datasetChanged(const QString& dataset);

private:
	DimensionsViewerPlugin*					_dimensionsViewerPlugin;
	std::unique_ptr<Ui::SettingsWidget>		_ui;
};
