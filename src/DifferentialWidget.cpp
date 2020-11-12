#include "DifferentialWidget.h"
#include "DimensionsViewerPlugin.h"

#include "ui_DifferentialWidget.h"

#include <QDebug>
#include <QStringListModel>

DifferentialWidget::DifferentialWidget(QWidget* parent) :
    ModelItemWidget(parent),
	_ui{ std::make_unique<Ui::DifferentialWidget>() }
{
    _ui->setupUi(this);

    /*QObject::connect(_ui->groupBox, &QGroupBox::toggled, [this](bool state) {
        setData(to_ul(Differential::Column::Enabled), state);
    });

    QObject::connect(_ui->profile1DatasetNameComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        setData(to_ul(Differential::Column::DatasetName1), currentText);
    });

    QObject::connect(_ui->profile2DatasetNameComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        setData(to_ul(Differential::Column::DatasetName2), currentText);
    });*/
}

//void DifferentialProfileWidget::updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles /*= QVector<int>()*/)
//{
//    QVector<QSharedPointer<QSignalBlocker>> signalBlockers;
//
//    signalBlockers << QSharedPointer<QSignalBlocker>::create(_ui->groupBox);
//    signalBlockers << QSharedPointer<QSignalBlocker>::create(_ui->profile1DatasetNameComboBox);
//    signalBlockers << QSharedPointer<QSignalBlocker>::create(_ui->profile2DatasetNameComboBox);
//
//	const auto selectedRows = dimensionsViewerPlugin->getModel().getSelectionModel().selectedRows();
//
//	if (selectedRows.isEmpty()) {
//        _ui->groupBox->setEnabled(false);
//        _ui->groupBox->setChecked(false);
//        _ui->profile1DatasetNameComboBox->setModel(new QStringListModel());
//        _ui->profile2DatasetNameComboBox->setModel(new QStringListModel());
//
//		return;
//	}
//
//	for (int column = begin.column(); column <= end.column(); column++) {
//		const auto index = begin.siblingAtColumn(column);
//
//        if (column == to_ul(DifferentialProfile::Column::Enabled)) {
//            _ui->groupBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
//            _ui->groupBox->setChecked(index.data(Qt::EditRole).toBool());
//            _ui->groupBox->setToolTip(index.data(Qt::ToolTipRole).toString());
//        }
//
//        if (column == to_ul(DifferentialProfile::Column::DatasetName1)) {
//            _ui->profile1DatasetNameComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
//            _ui->profile1DatasetNameComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
//        }
//
//        if (column == to_ul(DifferentialProfile::Column::DatasetName2)) {
//            _ui->profile2DatasetNameComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
//            _ui->profile2DatasetNameComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
//        }
//
//        if (column == to_ul(DifferentialProfile::Column::DatasetNames1)) {
//            _ui->profile1DatasetNameComboBox->setCurrentText(index.data(Qt::EditRole).toString());
//            _ui->profile1DatasetNameComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
//        }
//
//        if (column == to_ul(DifferentialProfile::Column::DatasetNames2)) {
//            _ui->profile2DatasetNameComboBox->setCurrentText(index.data(Qt::EditRole).toString());
//            _ui->profile2DatasetNameComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
//        }
//	}
//}

void DifferentialWidget::setModelIndex(const QPersistentModelIndex& modelIndex)
{
    ModelItemWidget::setModelIndex(modelIndex);
}