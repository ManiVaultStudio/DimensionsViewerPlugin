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

	const auto populateDatasetsCombobox = [this]() {
		_ui->dataSet1ComboBox->blockSignals(true);
		_ui->dataSet1ComboBox->setModel(new QStringListModel(_dimensionsViewerPlugin->getConfigurationsModel().getConfigurationNames()));
		_ui->dataSet1ComboBox->blockSignals(false);
	};
	
	QObject::connect(&configurationsModel, &ConfigurationsModel::rowsInserted, [this, populateDatasetsCombobox](const QModelIndex& parent, int first, int last) {
		populateDatasetsCombobox();
	});

	QObject::connect(&configurationsModel, &ConfigurationsModel::rowsRemoved, [this, populateDatasetsCombobox](const QModelIndex &parent, int first, int last) {
		populateDatasetsCombobox();
	});

	QObject::connect(&configurationsModel, &ConfigurationsModel::dataChanged, this, &SettingsWidget::updateData);

	QObject::connect(&configurationsModel.getSelectionModel(), &QItemSelectionModel::selectionChanged, [this, &configurationsModel](const QItemSelection& selected, const QItemSelection& deselected) {
		const auto selectedRows = configurationsModel.getSelectionModel().selectedRows();

		if (selectedRows.isEmpty())
			updateData(QModelIndex(), QModelIndex());
		else {
			const auto first = selectedRows.first();

			updateData(first.siblingAtColumn(static_cast<int>(Configuration::Column::Start)), first.siblingAtColumn(static_cast<int>(Configuration::Column::End)));
		}
	});
}

void SettingsWidget::updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles /*= QVector<int>()*/)
{
	const auto selectedRows = _dimensionsViewerPlugin->getConfigurationsModel().getSelectionModel().selectedRows();

	if (selectedRows.isEmpty())
		return;

	for (int column = begin.column(); column <= end.column(); column++) {
		const auto index = begin.siblingAtColumn(column);

		if (column == static_cast<int>(Configuration::Column::Channel1Dataset)) {

		}
	}
}