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
}

QString SettingsWidget::getCurrentDataset() const
{
	return _ui->datasetsComboBox->currentText();
}