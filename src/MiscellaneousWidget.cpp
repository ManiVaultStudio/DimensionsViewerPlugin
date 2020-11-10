#include "MiscellaneousWidget.h"
#include "Miscellaneous.h"

#include "ui_MiscellaneousWidget.h"

#include <QDebug>

MiscellaneousWidget::MiscellaneousWidget(QWidget* parent) :
    ModelItemWidget(parent),
	_ui{ std::make_unique<Ui::MiscellaneousWidget>() }
{
    _ui->setupUi(this);

    QObject::connect(_ui->showDimensionNamesCheckBox, &QCheckBox::stateChanged, [this](int state) {
        setData(to_ul(Miscellaneous::Column::ShowDimensionNames), state == Qt::Checked);
    });
}

void MiscellaneousWidget::updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles /*= QVector<int>()*/)
{
    QVector<QSharedPointer<QSignalBlocker>> signalBlockers;

    signalBlockers << QSharedPointer<QSignalBlocker>::create(_ui->groupBox);
    signalBlockers << QSharedPointer<QSignalBlocker>::create(_ui->showDimensionNamesCheckBox);

    if (begin == QModelIndex() && end == QModelIndex()) {
        _ui->groupBox->setEnabled(false);
        _ui->showDimensionNamesCheckBox->setEnabled(false);

        return;
    }

    for (int column = begin.column(); column <= end.column(); column++) {
        const auto index = begin.siblingAtColumn(column);

        if (column == to_ul(Miscellaneous::Column::ShowDimensionNames)) {
            _ui->showDimensionNamesCheckBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->showDimensionNamesCheckBox->setChecked(index.data(Qt::EditRole).toBool());
            _ui->showDimensionNamesCheckBox->setToolTip(index.data(Qt::ToolTipRole).toString());
        }
    }
}

void MiscellaneousWidget::setModelIndex(const QPersistentModelIndex& modelIndex)
{
    ModelItemWidget::setModelIndex(modelIndex);
}