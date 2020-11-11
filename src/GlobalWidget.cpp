#include "GlobalWidget.h"
#include "DimensionsViewerPlugin.h"
#include "Configuration.h"

#include "ui_GlobalWidget.h"

#include <QDebug>
#include <QStringListModel>

GlobalWidget::GlobalWidget(QWidget* parent) :
    ModelItemWidget(parent),
	_ui{ std::make_unique<Ui::GlobalWidget>() }
{
	_ui->setupUi(this);

    QObject::connect(_ui->groupBox, &QGroupBox::toggled, [this](bool state) {
        getModel().setData(getSiblingAtColumn(to_ul(Global::Column::Enabled)), state);
    });

    QObject::connect(_ui->profileTypeComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        const auto profileIndex = getChild(to_ul(Channel::Row::Profile));
        getModel().setData(getSiblingAtColumn(to_ul(Profile::Column::ProfileType), profileIndex), currentText);
    });

    QObject::connect(_ui->rangeTypeComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        const auto profileIndex = getChild(to_ul(Channel::Row::Profile));
        getModel().setData(getSiblingAtColumn(to_ul(Profile::Column::RangeType), profileIndex), currentText);
    });

    addWidgetMapper("EnabledCheckBox", QSharedPointer<WidgetMapper>::create(_ui->groupBox, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->groupBox->setEnabled(false);
            _ui->groupBox->setChecked(false);

            return;
        }

        _ui->groupBox->setVisible(index.flags() & Qt::ItemIsEditable);
        _ui->groupBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
        _ui->groupBox->setChecked(index.data(Qt::EditRole).toBool());
        _ui->groupBox->setToolTip(index.data(Qt::ToolTipRole).toString());
    }));

    addWidgetMapper("ProfileTypesComboBox", QSharedPointer<WidgetMapper>::create(_ui->profileTypeComboBox, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->profileTypeComboBox->setModel(new QStringListModel());
            _ui->profileTypeComboBox->setCurrentIndex(-1);

            return;
        }

        _ui->profileTypeComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
    }));

    addWidgetMapper("ProfileType", QSharedPointer<WidgetMapper>::create(_ui->profileTypeComboBox, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->profileTypeComboBox->setEnabled(false);

            return;
        }

        _ui->profileTypeComboBox->setVisible(index.flags() & Qt::ItemIsEditable);
        _ui->profileTypeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
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
            _ui->rangeTypeComboBox->setEnabled(false);

            return;
        }

        _ui->rangeTypeComboBox->setVisible(index.flags() & Qt::ItemIsEditable);
        _ui->rangeTypeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
        _ui->rangeTypeComboBox->setCurrentText(index.data(Qt::DisplayRole).toString());
        _ui->rangeTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
    }));
}

void GlobalWidget::setModelIndex(const QPersistentModelIndex& modelIndex)
{
    ModelItemWidget::setModelIndex(modelIndex);

    const auto profileIndex = getChild(to_ul(Channel::Row::Profile));

    getWidgetMapper("EnabledCheckBox")->setModelIndex(getSiblingAtColumn(to_ul(Profile::Column::Enabled), profileIndex));
    getWidgetMapper("ProfileTypesComboBox")->setModelIndex(getSiblingAtColumn(to_ul(Profile::Column::ProfileTypes), profileIndex));
    getWidgetMapper("ProfileType")->setModelIndex(getSiblingAtColumn(to_ul(Profile::Column::ProfileType), profileIndex));
    getWidgetMapper("RangeTypes")->setModelIndex(getSiblingAtColumn(to_ul(Profile::Column::RangeTypes), profileIndex));
    getWidgetMapper("RangeType")->setModelIndex(getSiblingAtColumn(to_ul(Profile::Column::RangeType), profileIndex));
}