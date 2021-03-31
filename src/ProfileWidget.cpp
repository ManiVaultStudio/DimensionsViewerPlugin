#include "ProfileWidget.h"
#include "Profile.h"
#include "StandardItems.h"

#include "ui_ProfileWidget.h"

#include <QDebug>
#include <QStringListModel>

ProfileWidget::ProfileWidget(QWidget* parent) :
    TreeItemWidget(parent),
    _ui{ std::make_unique<Ui::ProfileWidget>() }
{
    _ui->setupUi(this);

    QObject::connect(_ui->profileTypeComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        getModel().setData(getChild(to_ul(Profile::Child::ProfileType), to_ul(Profile::Column::Value)), currentText, Qt::DisplayRole);
    });

    QObject::connect(_ui->rangeTypeComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        getModel().setData(getChild(to_ul(Profile::Child::RangeType), to_ul(Profile::Column::Value)), currentText, Qt::DisplayRole);
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
        if (initialize)
            return;

        _ui->rangeTypeComboBox->setCurrentText(index.data(Qt::DisplayRole).toString());
        _ui->rangeTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
    }));
}

void ProfileWidget::setModelIndex(const QPersistentModelIndex& modelIndex)
{
    TreeItemWidget::setModelIndex(modelIndex);

    getWidgetMapper("ProfileTypes")->setModelIndex(getChild(to_ul(Profile::Child::ProfileTypes), to_ul(Profile::Column::Value)));
    getWidgetMapper("ProfileType")->setModelIndex(getChild(to_ul(Profile::Child::ProfileType), to_ul(Profile::Column::Value)));
    getWidgetMapper("RangeTypes")->setModelIndex(getChild(to_ul(Profile::Child::RangeTypes), to_ul(Profile::Column::Value)));
    getWidgetMapper("RangeType")->setModelIndex(getChild(to_ul(Profile::Child::RangeType), to_ul(Profile::Column::Value)));
}