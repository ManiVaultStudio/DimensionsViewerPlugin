#include "SettingsWidget.h"
#include "DimensionsViewerPlugin.h"

#include "ui_SettingsWidget.h"

#include <QDebug>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QStringListModel>

SettingsWidget::SettingsWidget(DimensionsViewerPlugin* histogramViewerPlugin) :
	_dimensionsViewerPlugin(histogramViewerPlugin),
	_ui{ std::make_unique<Ui::SettingsWidget>() }
{
	_ui->setupUi(this);
	
	QObject::connect(_dimensionsViewerPlugin, &DimensionsViewerPlugin::datasetsChanged, [this](const QStringList& datasets) {
		const auto currentIndex = _ui->datasetsComboBox->currentIndex();

		_ui->datasetsComboBox->blockSignals(true);
		_ui->datasetsComboBox->setModel(new QStringListModel(datasets));
		_ui->datasetsComboBox->setCurrentIndex(currentIndex);
		_ui->datasetsComboBox->blockSignals(false);

		if (datasets.size() == 1)
			emit datasetChanged(datasets.first());
	});

	QObject::connect(_ui->datasetsComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
		emit datasetChanged(currentText);
	});
}

QString SettingsWidget::getCurrentDatasetName() const
{
	return _ui->datasetsComboBox->currentText();
}