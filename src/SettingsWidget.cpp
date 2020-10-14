#include "SettingsWidget.h"
#include "DimensionsViewerPlugin.h"

#include "ui_SettingsWidget.h"

#include <QDebug>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QStringListModel>

SettingsWidget::SettingsWidget(DimensionsViewerPlugin* dimensionsViewerPlugin) :
	_dimensionsViewerPlugin(dimensionsViewerPlugin),
	_ui{ std::make_unique<Ui::SettingsWidget>() }
{
	_ui->setupUi(this);

	auto& configurationsModel = _dimensionsViewerPlugin->getConfigurationsModel();

	_ui->tableView->setModel(&configurationsModel);
	_ui->tableView->setSelectionModel(&configurationsModel.getSelectionModel());

	const auto updateDataset1Combobox = [this]() {
		auto& configurationsModel = _dimensionsViewerPlugin->getConfigurationsModel();

		_ui->dataSet1ComboBox->blockSignals(true);
		_ui->dataSet1ComboBox->setModel(new QStringListModel(configurationsModel.getConfigurationNames()));
		_ui->dataSet1ComboBox->setEnabled(configurationsModel.rowCount() > 0);
		_ui->dataSet1ComboBox->blockSignals(false);
	};
	
	QObject::connect(&configurationsModel, &ConfigurationsModel::rowsInserted, [this, updateDataset1Combobox](const QModelIndex& parent, int first, int last) {
		updateDataset1Combobox();
	});

	QObject::connect(&configurationsModel, &ConfigurationsModel::rowsRemoved, [this, updateDataset1Combobox](const QModelIndex& parent, int first, int last) {
		updateDataset1Combobox();
	});

	QObject::connect(&configurationsModel, &ConfigurationsModel::dataChanged, this, &SettingsWidget::updateData);

	QObject::connect(&configurationsModel.getSelectionModel(), &QItemSelectionModel::selectionChanged, [this, &configurationsModel](const QItemSelection& selected, const QItemSelection& deselected) {
		const auto selectedRows = configurationsModel.getSelectionModel().selectedRows();

		/*
		if (selectedRows.isEmpty())
			_ui->dataSet1ComboBox->setCurrentIndex(-1);
		else
			_ui->dataSet1ComboBox->setCurrentIndex(selectedRows.first().row());
		*/

		if (selectedRows.isEmpty())
			updateData(QModelIndex(), QModelIndex());
		else {
			const auto first = selectedRows.first();

			updateData(first.siblingAtColumn(static_cast<int>(Configuration::Column::Start)), first.siblingAtColumn(static_cast<int>(Configuration::Column::End)));
		}
	});

	QObject::connect(_ui->dataSet1ComboBox, qOverload<int>(&QComboBox::currentIndexChanged), [this](int currentIndex) {
		_dimensionsViewerPlugin->getConfigurationsModel().selectRow(currentIndex);
	});

	QObject::connect(_ui->dataSet2EnabledCheckBox, &QCheckBox::stateChanged, [this, &configurationsModel](int state) {
		const auto index = configurationsModel.index(_ui->dataSet1ComboBox->currentIndex(), static_cast<int>(Configuration::Column::Channel2Enabled));

		configurationsModel.setData(index, state == Qt::Checked);
	});

	QObject::connect(_ui->dataSet3EnabledCheckBox, &QCheckBox::stateChanged, [this, &configurationsModel](int state) {
		const auto index = configurationsModel.index(_ui->dataSet1ComboBox->currentIndex(), static_cast<int>(Configuration::Column::Channel3Enabled));

		configurationsModel.setData(index, state == Qt::Checked);
	});
}

void SettingsWidget::updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles /*= QVector<int>()*/)
{
	const auto selectedRows = _dimensionsViewerPlugin->getConfigurationsModel().getSelectionModel().selectedRows();

	if (selectedRows.isEmpty())
		return;

	if (begin.row() != selectedRows.first().row())
		return;

	for (int column = begin.column(); column <= end.column(); column++) {
		const auto index = begin.siblingAtColumn(column);

		if (column == static_cast<int>(Configuration::Column::Channel2Enabled)) {
			_ui->dataSet2EnabledCheckBox->blockSignals(true);
			_ui->dataSet2EnabledCheckBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->dataSet2EnabledCheckBox->setChecked(index.data(Qt::EditRole).toBool());
			_ui->dataSet2EnabledCheckBox->blockSignals(false);
		}

		if (column == static_cast<int>(Configuration::Column::Channel3Enabled)) {
			_ui->dataSet3EnabledCheckBox->blockSignals(true);
			_ui->dataSet3EnabledCheckBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->dataSet3EnabledCheckBox->setChecked(index.data(Qt::EditRole).toBool());
			_ui->dataSet3EnabledCheckBox->blockSignals(false);
		}

		if (column == static_cast<int>(Configuration::Column::Channel1DatasetName)) {
			_ui->dataSet1ComboBox->blockSignals(true);
			_ui->dataSet1ComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->dataSet1ComboBox->blockSignals(false);
		}

		if (column == static_cast<int>(Configuration::Column::Channel2DatasetName)) {
			_ui->dataSet2ComboBox->blockSignals(true);
			_ui->dataSet2ComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->dataSet2ComboBox->blockSignals(false);
		}

		if (column == static_cast<int>(Configuration::Column::Channel3DatasetName)) {
			_ui->dataSet3ComboBox->blockSignals(true);
			_ui->dataSet3ComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->dataSet3ComboBox->blockSignals(false);
		}

		if (column == static_cast<int>(Configuration::Column::Subsets)) {
			const auto subsets = index.data(Qt::EditRole).toStringList();

			_ui->dataSet2ComboBox->blockSignals(true);
			_ui->dataSet2ComboBox->setModel(new QStringListModel(subsets));
			_ui->dataSet2ComboBox->blockSignals(false);

			_ui->dataSet3ComboBox->blockSignals(true);
			_ui->dataSet3ComboBox->setModel(new QStringListModel(subsets));
			_ui->dataSet3ComboBox->blockSignals(false);
		}
	}
}