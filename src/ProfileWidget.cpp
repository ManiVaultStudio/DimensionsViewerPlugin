#include "ProfileWidget.h"
#include "Profile.h"

#include "ui_ProfileWidget.h"

#include <QDebug>
#include <QStringListModel>

ProfileWidget::ProfileWidget(QWidget* parent) :
    TreeItemWidget(parent),
	_ui{ std::make_unique<Ui::ProfileWidget>() }
{
	_ui->setupUi(this);

    /*
    QObject::connect(_ui->profileTypeComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        setData(to_ul(Profile::Column::ProfileType), currentText, Qt::DisplayRole);
    });

    QObject::connect(_ui->rangeTypeComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        setData(to_ul(Profile::Column::RangeType), currentText, Qt::DisplayRole);
    });

    addWidgetMapper("ProfileTypes", QSharedPointer<WidgetMapper>::create(_ui->profileTypeComboBox, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->profileTypeComboBox->setModel(new QStringListModel());
            _ui->profileTypeComboBox->setCurrentIndex(-1);

            return;
        }

        _ui->profileTypeComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
    }));

    addWidgetMapper("ProfileType", QSharedPointer<WidgetMapper>::create(_ui->profileTypeComboBox, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            return;
        }

        _ui->profileTypeComboBox->setVisible(index.flags() & Qt::ItemIsEditable);
        _ui->profileTypeComboBox->setCurrentText(index.data(Qt::DisplayRole).toString());
        _ui->profileTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
    }));

    addWidgetMapper("RangeTypes", QSharedPointer<WidgetMapper>::create(_ui->rangeTypeComboBox, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->rangeTypeComboBox->setModel(new QStringListModel());
            _ui->rangeTypeComboBox->setCurrentIndex(-1);

            return;
        }

        _ui->rangeTypeComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
    }));

    addWidgetMapper("RangeType", QSharedPointer<WidgetMapper>::create(_ui->rangeTypeComboBox, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            return;
        }

        _ui->rangeTypeComboBox->setVisible(index.flags() & Qt::ItemIsEditable);
        _ui->rangeTypeComboBox->setCurrentText(index.data(Qt::DisplayRole).toString());
        _ui->rangeTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
    }));
    */
}

void ProfileWidget::setModelIndex(const QPersistentModelIndex& modelIndex)
{
    TreeItemWidget::setModelIndex(modelIndex);

    /*
    getWidgetMapper("ProfileTypes")->setModelIndex(getSiblingAtColumn(to_ul(Profile::Column::ProfileTypes)));
    getWidgetMapper("ProfileType")->setModelIndex(getSiblingAtColumn(to_ul(Profile::Column::ProfileType)));
    getWidgetMapper("RangeTypes")->setModelIndex(getSiblingAtColumn(to_ul(Profile::Column::RangeTypes)));
    getWidgetMapper("RangeType")->setModelIndex(getSiblingAtColumn(to_ul(Profile::Column::RangeType)));
    */
}