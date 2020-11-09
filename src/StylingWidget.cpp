#include "StylingWidget.h"

#include "ui_StylingWidget.h"

#include <QDebug>

StylingWidget::StylingWidget(QWidget* parent) :
    ModelItemWidget(parent),
    _ui { std::make_unique<Ui::StylingWidget>() }
{
    setWindowFlags(Qt::Popup);

    setObjectName("StylingWidget");
    setStyleSheet("QWidget#StylingWidget { border: 1px solid grey; }");

    _ui->setupUi(this);

    move(parent->mapToGlobal(parent->rect().bottomRight()) - QPoint(width(), 0));
}

void StylingWidget::updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles /*= QVector<int>()*/)
{
    /*QVector<QSharedPointer<QSignalBlocker>> signalBlockers;

    signalBlockers << QSharedPointer<QSignalBlocker>::create(_ui->groupBox);
    signalBlockers << QSharedPointer<QSignalBlocker>::create(_ui->profileTypeComboBox);
    signalBlockers << QSharedPointer<QSignalBlocker>::create(_ui->rangeTypeComboBox);

    if (begin == QModelIndex() && end == QModelIndex()) {
        _ui->groupBox->setEnabled(false);
        _ui->groupBox->setChecked(false);

        return;
    }

    for (int column = begin.column(); column <= end.column(); column++) {
        const auto index = begin.siblingAtColumn(column);

        if (column == to_ul(Global::Column::Enabled)) {
            _ui->groupBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->groupBox->setChecked(index.data(Qt::EditRole).toBool());
            _ui->groupBox->setToolTip(index.data(Qt::ToolTipRole).toString());
        }

        if (column == to_ul(Global::Column::ProfileTypes)) {
            _ui->profileTypeComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
        }

        if (column == to_ul(Global::Column::ProfileType)) {
            _ui->profileTypeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->profileTypeComboBox->setCurrentText(index.data(Qt::DisplayRole).toString());
            _ui->profileTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
        }

        if (column == to_ul(Global::Column::RangeTypes)) {
            _ui->rangeTypeComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
        }

        if (column == to_ul(Global::Column::RangeType)) {
            _ui->rangeTypeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->rangeTypeComboBox->setCurrentText(index.data(Qt::DisplayRole).toString());
            _ui->rangeTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
        }
    }*/
}

void StylingWidget::setModelIndex(const QPersistentModelIndex& modelIndex)
{
    ModelItemWidget::setModelIndex(modelIndex);
}