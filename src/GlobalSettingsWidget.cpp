#include "GlobalSettingsWidget.h"
#include "DimensionsViewerPlugin.h"

#include "ui_GlobalSettingsWidget.h"

#include <QDebug>
#include <QStringListModel>

DimensionsViewerPlugin* GlobalSettingsWidget::dimensionsViewerPlugin = nullptr;

GlobalSettingsWidget::GlobalSettingsWidget(QWidget* parent) :
	QWidget(parent),
	_ui{ std::make_unique<Ui::GlobalSettingsWidget>() }
{
	_ui->setupUi(this);

    /*auto& configurationsModel = dimensionsViewerPlugin->getConfigurationsModel();

    QObject::connect(&configurationsModel, &ConfigurationsModel::dataChanged, this, &GlobalSettingsWidget::updateData);

    QObject::connect(&configurationsModel.getSelectionModel(), &QItemSelectionModel::selectionChanged, [this, &configurationsModel](const QItemSelection& selected, const QItemSelection& deselected) {
        const auto selectedRows = configurationsModel.getSelectionModel().selectedRows();

        if (selectedRows.isEmpty())
            updateData(QModelIndex(), QModelIndex());
        else {
            const auto first = selectedRows.first();

            updateData(first.siblingAtColumn(static_cast<int>(Configuration::Column::Start)), first.siblingAtColumn(static_cast<int>(Configuration::Column::End)));
        }
    });

    QObject::connect(_ui->groupBox, &QGroupBox::toggled, [this, &configurationsModel](bool state) {
        configurationsModel.setData(Configuration::Column::GlobalSettings, state);
    });

    QObject::connect(_ui->profileTypeComboBox, &QComboBox::currentTextChanged, [this, &configurationsModel](QString currentText) {
        configurationsModel.setData(Configuration::Column::GlobalProfileType, currentText);
    });

    QObject::connect(_ui->rangeTypeComboBox, &QComboBox::currentTextChanged, [this, &configurationsModel](QString currentText) {
        configurationsModel.setData(Configuration::Column::GlobalRangeType, currentText);
    });

	updateData(QModelIndex(), QModelIndex());*/
}

void GlobalSettingsWidget::updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles /*= QVector<int>()*/)
{
	/*const auto selectedRows = dimensionsViewerPlugin->getConfigurationsModel().getSelectionModel().selectedRows();

	if (selectedRows.isEmpty()) {
        _ui->groupBox->setEnabled(false);
        _ui->groupBox->setChecked(false);

		return;
	}

	if (begin.row() != selectedRows.first().row())
		return;

	for (int column = begin.column(); column <= end.column(); column++) {
		const auto index = begin.siblingAtColumn(column);

        if (column == Configuration::Column::GlobalSettings) {
            _ui->groupBox->blockSignals(true);
            _ui->groupBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->groupBox->setChecked(index.data(Qt::EditRole).toBool());
            _ui->groupBox->setToolTip(index.data(Qt::ToolTipRole).toString());
            _ui->groupBox->blockSignals(false);
        }

        if (column == Configuration::Column::GlobalProfileTypes) {
            _ui->profileTypeComboBox->blockSignals(true);
            _ui->profileTypeComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
            _ui->profileTypeComboBox->blockSignals(false);
        }

        if (column == Configuration::Column::GlobalProfileType) {
            _ui->profileTypeComboBox->blockSignals(true);
            _ui->profileTypeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->profileTypeComboBox->setCurrentText(index.data(Qt::DisplayRole).toString());
            _ui->profileTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
            _ui->profileTypeComboBox->blockSignals(false);
        }

        if (column == Configuration::Column::GlobalRangeTypes) {
            _ui->rangeTypeComboBox->blockSignals(true);
            _ui->rangeTypeComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
            _ui->rangeTypeComboBox->blockSignals(false);
        }

        if (column == Configuration::Column::GlobalRangeType) {
            _ui->rangeTypeComboBox->blockSignals(true);
            _ui->rangeTypeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->rangeTypeComboBox->setCurrentText(index.data(Qt::DisplayRole).toString());
            _ui->rangeTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
            _ui->rangeTypeComboBox->blockSignals(false);
        }
	}*/
}