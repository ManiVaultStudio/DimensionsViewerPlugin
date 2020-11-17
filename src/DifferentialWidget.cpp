#include "DifferentialWidget.h"
#include "Differential.h"

#include "ui_DifferentialWidget.h"

#include <QDebug>
#include <QStringListModel>

DifferentialWidget::DifferentialWidget(QWidget* parent) :
    TreeItemWidget(parent),
	_ui{ std::make_unique<Ui::DifferentialWidget>() }
{
	_ui->setupUi(this);

    QObject::connect(_ui->differentialOperandAComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        //const auto differentialIndex = getChild(to_ul(Channel::Row::Differential));
        //getModel().setData( to_ul(Channel::Column::DifferentialOperandA), currentText, Qt::EditRole);
    });

    QObject::connect(_ui->differentialOperandBComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        //setData(to_ul(Channel::Column::DifferentialOperandB), currentText, Qt::EditRole);
    });
    
    addWidgetMapper("DifferentialOperandNamesA", QSharedPointer<WidgetMapper>::create(_ui->differentialOperandAComboBox, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->differentialOperandAComboBox->setModel(new QStringListModel());

            return;
        }

        _ui->differentialOperandAComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
    }));

    addWidgetMapper("DifferentialOperandNamesB", QSharedPointer<WidgetMapper>::create(_ui->differentialOperandBComboBox, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->differentialOperandBComboBox->setModel(new QStringListModel());

            return;
        }

        _ui->differentialOperandBComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
    }));

    addWidgetMapper("DifferentialOperandA", QSharedPointer<WidgetMapper>::create(_ui->differentialOperandAComboBox, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->differentialOperandAComboBox->setEnabled(false);

            return;
        }

        _ui->differentialOperandAComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
        _ui->differentialOperandAComboBox->setCurrentText(index.data(Qt::DisplayRole).toString());
        _ui->differentialOperandAComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
    }));
    
    addWidgetMapper("DifferentialOperandB", QSharedPointer<WidgetMapper>::create(_ui->differentialOperandBComboBox, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->differentialOperandBComboBox->setEnabled(false);

            return;
        }

        _ui->differentialOperandBComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
        _ui->differentialOperandBComboBox->setCurrentText(index.data(Qt::DisplayRole).toString());
        _ui->differentialOperandBComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
    }));
}

void DifferentialWidget::setModelIndex(const QPersistentModelIndex& modelIndex)
{
    TreeItemWidget::setModelIndex(modelIndex);

    getWidgetMapper("DifferentialOperandNamesA")->setModelIndex(getSiblingAtColumn(to_ul(Differential::Column::DifferentialOperandNamesA)));
    getWidgetMapper("DifferentialOperandNamesB")->setModelIndex(getSiblingAtColumn(to_ul(Differential::Column::DifferentialOperandNamesB)));
    getWidgetMapper("DifferentialOperandA")->setModelIndex(getSiblingAtColumn(to_ul(Differential::Column::DifferentialOperandA)));
    getWidgetMapper("DifferentialOperandB")->setModelIndex(getSiblingAtColumn(to_ul(Differential::Column::DifferentialOperandB)));
}