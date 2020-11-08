#include "MiscellaneousWidget.h"
#include "DimensionsViewerPlugin.h"

#include "ui_MiscellaneousWidget.h"

#include <QDebug>

MiscellaneousWidget::MiscellaneousWidget(QWidget* parent) :
    ModelItemWidget(parent),
	_ui{ std::make_unique<Ui::MiscellaneousWidget>() }
{
	_ui->setupUi(this);

    /*auto& configurationsModel = dimensionsViewerPlugin->getConfigurationsModel();

    QObject::connect(&configurationsModel, &ConfigurationsModel::dataChanged, this, &MiscellaneousWidget::updateData);

    QObject::connect(&configurationsModel.getSelectionModel(), &QItemSelectionModel::selectionChanged, [this, &configurationsModel](const QItemSelection& selected, const QItemSelection& deselected) {
        const auto selectedRows = configurationsModel.getSelectionModel().selectedRows();

        if (selectedRows.isEmpty())
            updateData(QModelIndex(), QModelIndex());
        else {
            const auto first = selectedRows.first();

            updateData(first.siblingAtColumn(static_cast<int>(Configuration::Column::Start)), first.siblingAtColumn(static_cast<int>(Configuration::Column::End)));
        }
    });

    QObject::connect(_ui->showDimensionNamesCheckBox, &QCheckBox::stateChanged, [this, &configurationsModel](int state) {
        configurationsModel.setData(Configuration::Column::ShowDimensionNames, state == Qt::Checked);
    });

	*/

    reset();
}

void MiscellaneousWidget::updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles /*= QVector<int>()*/)
{
    QVector<QSharedPointer<QSignalBlocker>> signalBlockers;

    signalBlockers << QSharedPointer<QSignalBlocker>::create(_ui->groupBox);
    signalBlockers << QSharedPointer<QSignalBlocker>::create(_ui->showDimensionNamesCheckBox);

    /*const auto selectedRows = dimensionsViewerPlugin->getConfigurationsModel().getSelectionModel().selectedRows();

    if (selectedRows.isEmpty()) {
        _ui->showDimensionNamesCheckBox->setEnabled(false);

        return;
    }

    if (begin.row() != selectedRows.first().row())
        return;

    for (int column = begin.column(); column <= end.column(); column++) {
        const auto index = begin.siblingAtColumn(column);

        if (column == Configuration::Column::ShowDimensionNames) {
            _ui->showDimensionNamesCheckBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->showDimensionNamesCheckBox->setChecked(index.data(Qt::EditRole).toBool());
            _ui->showDimensionNamesCheckBox->setToolTip(index.data(Qt::ToolTipRole).toString());
        }
    }*/
}

void MiscellaneousWidget::setPersistentModelIndex(const QPersistentModelIndex& modelIndex)
{
    ModelItemWidget::setPersistentModelIndex(modelIndex);
}