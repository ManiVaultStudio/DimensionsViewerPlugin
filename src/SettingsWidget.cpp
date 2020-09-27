#include "SettingsWidget.h"
#include "DimensionsViewerPlugin.h"

#include "ui_SettingsWidget.h"

#include <QDebug>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>

SettingsWidget::SettingsWidget(DimensionsViewerPlugin* histogramViewerPlugin) :
	_dimensionsViewerPlugin(histogramViewerPlugin),
	_ui{ std::make_unique<Ui::SettingsWidget>() }
{
	_ui->setupUi(this);
	
	_ui->datasetsComboBox->setModel(&_dimensionsViewerPlugin->getDatasetsModel());

	QObject::connect(_ui->datasetsComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
		emit datasetChanged(currentText);
	});

	/*
	QObject::connect(&_dimensionsViewerPlugin->getDatasetsModel(), &QAbstractItemModel::rowsInserted, [this](const QModelIndex& parent, int first, int last) {
		if (_dimensionsViewerPlugin->getDatasetsModel().rowCount() == 1)
			_ui->datasetsComboBox->setCurrentIndex(0);
	});
	*/
}

QString SettingsWidget::getCurrentDatasetName() const
{
	return _ui->datasetsComboBox->currentText();
}