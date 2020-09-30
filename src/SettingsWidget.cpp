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
	
	const auto updateDataset1GUI = [this]() {
	};

	const auto updateDataset2GUI = [this]() {
		/*
		const auto enableControls = _ui->dataset2EnabledCheckBox->isChecked();

		_ui->dataSet2ComboBox->setEnabled(enableControls);
		*/
	};

	const auto updateDataset3GUI = [this]() {
		/*
		const auto enableControls = _ui->dataset3EnabledCheckBox->isChecked();

		_ui->dataSet3ComboBox->setEnabled(enableControls);
		*/
	};

	QObject::connect(_dimensionsViewerPlugin, &DimensionsViewerPlugin::datasetsChanged, [this](const DimensionsViewerPlugin::Datasets& datasets) {
		const auto currentIndex	= _ui->dataSet1ComboBox->currentIndex();
		const auto firstDataset	= datasets.size() == 1;

		_ui->dataSet1ComboBox->blockSignals(true);
		_ui->dataSet1ComboBox->setModel(new QStringListModel(datasets.keys()));
		_ui->dataSet1ComboBox->setCurrentIndex(firstDataset ? 0 : currentIndex);
		_ui->dataSet1ComboBox->blockSignals(false);

		/*
		const auto subsets = currentIndex >= 0 ? datasets[datasets.keys().at(currentIndex)] : QStringList();
		const auto noSubsets = subsets.size();

		_ui->dataset2EnabledCheckBox->setEnabled(noSubsets >= 1);
		_ui->dataset3EnabledCheckBox->setEnabled(noSubsets >= 2);

		_ui->dataSet2ComboBox->blockSignals(true);
		_ui->dataSet2ComboBox->setModel(new QStringListModel(subsets));
		_ui->dataSet2ComboBox->setCurrentIndex(0);
		_ui->dataSet2ComboBox->blockSignals(false);

		_ui->dataSet3ComboBox->blockSignals(true);
		_ui->dataSet3ComboBox->setModel(new QStringListModel(subsets));
		_ui->dataSet3ComboBox->setCurrentIndex(1);
		_ui->dataSet3ComboBox->blockSignals(false);
		*/

		if (firstDataset)
			emit datasetChanged(datasets.keys().last());
	});

	QObject::connect(_ui->dataSet1ComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
		emit datasetChanged(currentText);
	});
}

QString SettingsWidget::getCurrentDatasetName() const
{
	return _ui->dataSet1ComboBox->currentText();
}