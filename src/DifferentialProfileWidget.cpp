#include "DifferentialProfileWidget.h"
#include "DimensionsViewerPlugin.h"

#include "ui_DifferentialProfileWidget.h"

#include <QDebug>
#include <QStringListModel>

DifferentialProfileWidget::DifferentialProfileWidget(QWidget* parent) :
    ModelItemWidget(parent),
	_ui{ std::make_unique<Ui::DifferentialProfileWidget>() }
{
	_ui->setupUi(this);

    /*auto& configurationsModel = dimensionsViewerPlugin->getConfigurationsModel();

    QObject::connect(&configurationsModel, &ConfigurationsModel::dataChanged, this, &DifferentialProfileWidget::updateData);

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

	*/

    reset();
}

void DifferentialProfileWidget::updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles /*= QVector<int>()*/)
{
    QVector<QSharedPointer<QSignalBlocker>> signalBlockers;

    signalBlockers << QSharedPointer<QSignalBlocker>::create(_ui->groupBox);
    signalBlockers << QSharedPointer<QSignalBlocker>::create(_ui->profile1DatasetNameComboBox);
    signalBlockers << QSharedPointer<QSignalBlocker>::create(_ui->profile2DatasetNameComboBox);

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

        if (column == Configuration::Column::ShowDifferentialProfile) {
            _ui->groupBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->groupBox->setChecked(index.data(Qt::EditRole).toBool());
            _ui->groupBox->setToolTip(index.data(Qt::ToolTipRole).toString());
            
            _ui->compareLabel->setEnabled(index.data(Qt::EditRole).toBool());
            _ui->withLabel->setEnabled(index.data(Qt::EditRole).toBool());
            _ui->colorPushButton->setEnabled(index.data(Qt::EditRole).toBool());
        }

        if (column == Configuration::Column::Profile1DatasetNames) {
            _ui->profile1DatasetNameComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
        }

        if (column == Configuration::Column::Profile2DatasetNames) {
            _ui->profile2DatasetNameComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
        }

        if (column == Configuration::Column::Profile1DatasetName) {
            _ui->profile1DatasetNameComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->profile1DatasetNameComboBox->setCurrentText(index.data(Qt::EditRole).toString());
            _ui->profile1DatasetNameComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
        }

        if (column == Configuration::Column::Profile2DatasetName) {
            _ui->profile2DatasetNameComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->profile2DatasetNameComboBox->setCurrentText(index.data(Qt::EditRole).toString());
            _ui->profile2DatasetNameComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
        }
	}*/
}

void DifferentialProfileWidget::setPersistentModelIndex(const QPersistentModelIndex& modelIndex)
{
    ModelItemWidget::setPersistentModelIndex(modelIndex);
}