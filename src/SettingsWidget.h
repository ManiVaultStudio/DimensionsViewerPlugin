#pragma once

#include "Common.h"

#include <memory>

#include <QWidget.h>
#include <QVector.h>

class DimensionsViewerPlugin;

class QCheckBox;
class QPushButton;

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
    QVector<QCheckBox*>                     _enabled;
    QVector<QCheckBox*>                     _showRange;
    QVector<QPushButton*>                   _locked;
};