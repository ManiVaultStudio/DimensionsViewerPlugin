#include "DifferentialProfileSettingsWidget.h"
#include "DimensionsViewerPlugin.h"

#include "ui_DifferentialProfileSettingsWidget.h"

#include <QDebug>
#include <QStringListModel>

DimensionsViewerPlugin* DifferentialProfileSettingsWidget::dimensionsViewerPlugin = nullptr;

DifferentialProfileSettingsWidget::DifferentialProfileSettingsWidget(QWidget* parent) :
    QWidget(parent),
	_ui{ std::make_unique<Ui::DifferentialProfileSettingsWidget>() }
{
	_ui->setupUi(this);

    auto& configurationsModel = dimensionsViewerPlugin->getConfigurationsModel();

    QObject::connect(&configurationsModel, &ConfigurationsModel::dataChanged, this, &DifferentialProfileSettingsWidget::updateData);

    QObject::connect(&configurationsModel.getSelectionModel(), &QItemSelectionModel::selectionChanged, [this, &configurationsModel](const QItemSelection& selected, const QItemSelection& deselected) {
        const auto selectedRows = configurationsModel.getSelectionModel().selectedRows();

        if (selectedRows.isEmpty())
            updateData(QModelIndex(), QModelIndex());
        else {
            const auto first = selectedRows.first();

            updateData(first.siblingAtColumn(static_cast<int>(Configuration::Column::Start)), first.siblingAtColumn(static_cast<int>(Configuration::Column::End)));
        }
    });

    QObject::connect(_ui->groupBox, &QGroupBox::toggled, [this, &configurationsModel](bool checked) {
        configurationsModel.setData(Configuration::Column::ShowDifferentialProfile, checked);
    });

    QObject::connect(_ui->profile1DatasetNameComboBox, &QComboBox::currentTextChanged, [this, &configurationsModel](QString currentText) {
        configurationsModel.setData(Configuration::Column::Profile1DatasetName, currentText);
    });

    QObject::connect(_ui->profile2DatasetNameComboBox, &QComboBox::currentTextChanged, [this, &configurationsModel](QString currentText) {
        configurationsModel.setData(Configuration::Column::Profile2DatasetName, currentText);
    });

	updateData(QModelIndex(), QModelIndex());
}

void DifferentialProfileSettingsWidget::updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles /*= QVector<int>()*/)
{
	const auto selectedRows = dimensionsViewerPlugin->getConfigurationsModel().getSelectionModel().selectedRows();

	if (selectedRows.isEmpty()) {
        _ui->groupBox->setEnabled(false);
        _ui->groupBox->setChecked(false);

		return;
	}

	if (begin.row() != selectedRows.first().row())
		return;

	for (int column = begin.column(); column <= end.column(); column++) {
		const auto index = begin.siblingAtColumn(column);

        if (column == Configuration::Column::ShowDifferentialProfile) {
            _ui->groupBox->blockSignals(true);
            _ui->groupBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->groupBox->setChecked(index.data(Qt::EditRole).toBool());
            _ui->groupBox->setToolTip(index.data(Qt::ToolTipRole).toString());
            _ui->groupBox->blockSignals(false);
            
            _ui->compareLabel->setEnabled(index.data(Qt::EditRole).toBool());
            _ui->withLabel->setEnabled(index.data(Qt::EditRole).toBool());
            _ui->colorPushButton->setEnabled(index.data(Qt::EditRole).toBool());
        }

        if (column == Configuration::Column::Profile1DatasetNames) {
            _ui->profile1DatasetNameComboBox->blockSignals(true);
            _ui->profile1DatasetNameComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
            _ui->profile1DatasetNameComboBox->blockSignals(false);
        }

        if (column == Configuration::Column::Profile2DatasetNames) {
            _ui->profile2DatasetNameComboBox->blockSignals(true);
            _ui->profile2DatasetNameComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
            _ui->profile2DatasetNameComboBox->blockSignals(false);
        }

        if (column == Configuration::Column::Profile1DatasetName) {
            _ui->profile1DatasetNameComboBox->blockSignals(true);
            _ui->profile1DatasetNameComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->profile1DatasetNameComboBox->setCurrentText(index.data(Qt::EditRole).toString());
            _ui->profile1DatasetNameComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
            _ui->profile1DatasetNameComboBox->blockSignals(false);
        }

        if (column == Configuration::Column::Profile2DatasetName) {
            _ui->profile2DatasetNameComboBox->blockSignals(true);
            _ui->profile2DatasetNameComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->profile2DatasetNameComboBox->setCurrentText(index.data(Qt::EditRole).toString());
            _ui->profile2DatasetNameComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
            _ui->profile2DatasetNameComboBox->blockSignals(false);
        }
	}
}