#include "StylingWidget.h"
#include "Channel.h"

#include "ui_StylingWidget.h"

#include <QDebug>
#include <QStringListModel>

StylingWidget::StylingWidget(QWidget* parent) :
    TreeItemWidget(parent),
    _ui { std::make_unique<Ui::StylingWidget>() }
{
    setWindowFlags(Qt::Popup);

    setObjectName("StylingWidget");
    setStyleSheet("QWidget#StylingWidget { border: 1px solid grey; }");

    _ui->setupUi(this);

    move(parent->mapToGlobal(parent->rect().bottomRight()) - QPoint(width(), 0));

    QObject::connect(_ui->lineTypeProfileComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        setData(to_ul(Channel::Column::LineTypeProfile), currentText, Qt::DisplayRole);
    });

    QObject::connect(_ui->lineTypeRangeComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        setData(to_ul(Channel::Column::LineTypeRange), currentText, Qt::DisplayRole);
    });

    QObject::connect(_ui->renderPointsCheckBox, &QCheckBox::stateChanged, [this](int state) {
        setData(to_ul(Channel::Column::RenderPoints), state);
    });

    QObject::connect(_ui->opacitySpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), [this](double value) {
        setData(to_ul(Channel::Column::Opacity), static_cast<float>(value));
    });

    QObject::connect(_ui->opacitySlider, &QSlider::valueChanged, [this](int value) {
        setData(to_ul(Channel::Column::Opacity), 0.01f * static_cast<float>(value));
    });

    QObject::connect(_ui->colorPushButton, &ColorPickerPushButton::colorChanged, [this](const QColor& color) {
        setData(to_ul(Channel::Column::Color), color);
    });

    addWidgetMapper("LineTypesProfile", QSharedPointer<WidgetMapper>::create(_ui->lineTypeProfileComboBox, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->lineTypeProfileComboBox->setIconSize(Styling::LineTypesModel::iconSize);
            _ui->lineTypeProfileComboBox->setModel(new Styling::LineTypesModel());

            return;
        }
    }));

    addWidgetMapper("LineTypesRange", QSharedPointer<WidgetMapper>::create(_ui->lineTypeRangeComboBox, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->lineTypeRangeComboBox->setIconSize(Styling::LineTypesModel::iconSize);
            _ui->lineTypeRangeComboBox->setModel(new Styling::LineTypesModel());

            return;
        }
    }));

    addWidgetMapper("LineTypeProfileComboBox", QSharedPointer<WidgetMapper>::create(_ui->lineTypeProfileComboBox, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->lineTypeProfileComboBox->setEnabled(false);

            return;
        }

        _ui->lineTypeProfileComboBox->setVisible(index.flags() & Qt::ItemIsEditable);
        _ui->lineTypeProfileComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
        _ui->lineTypeProfileComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
        _ui->lineTypeProfileComboBox->setCurrentText(index.data(Qt::DisplayRole).toString());
    }));

    addWidgetMapper("LineTypeRangeComboBox", QSharedPointer<WidgetMapper>::create(_ui->lineTypeRangeComboBox, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->lineTypeRangeComboBox->setEnabled(false);

            return;
        }

        _ui->lineTypeRangeComboBox->setVisible(index.flags() & Qt::ItemIsEditable);
        _ui->lineTypeRangeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
        _ui->lineTypeRangeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
        _ui->lineTypeRangeComboBox->setCurrentText(index.data(Qt::DisplayRole).toString());
    }));

    addWidgetMapper("RenderPointsCheckBox", QSharedPointer<WidgetMapper>::create(_ui->renderPointsCheckBox, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->renderPointsCheckBox->setEnabled(false);

            return;
        }

        _ui->renderPointsCheckBox->setVisible(index.flags() & Qt::ItemIsEditable);
        _ui->renderPointsCheckBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
        _ui->renderPointsCheckBox->setToolTip(index.data(Qt::ToolTipRole).toString());
        _ui->renderPointsCheckBox->setChecked(index.data(Qt::EditRole).toBool());
    }));

    addWidgetMapper("OpacitySpinBox", QSharedPointer<WidgetMapper>::create(_ui->opacitySpinBox, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->opacitySpinBox->setEnabled(false);
            _ui->opacitySpinBox->setValue(0.0f);

            return;
        }

        _ui->opacitySpinBox->setVisible(index.flags() & Qt::ItemIsEditable);
        _ui->opacitySpinBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
        _ui->opacitySpinBox->setToolTip(index.data(Qt::ToolTipRole).toString());
        _ui->opacitySpinBox->setValue(index.data(Qt::EditRole).toFloat());
    }));

    addWidgetMapper("OpacitySlider", QSharedPointer<WidgetMapper>::create(_ui->opacitySlider, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->opacitySpinBox->setEnabled(false);
            _ui->opacitySpinBox->setValue(0.0f);

            return;
        }

        _ui->opacitySlider->setVisible(index.flags() & Qt::ItemIsEditable);
        _ui->opacitySlider->setEnabled(index.flags() & Qt::ItemIsEnabled);
        _ui->opacitySlider->setToolTip(index.data(Qt::ToolTipRole).toString());
        _ui->opacitySlider->setValue(index.data(Qt::EditRole).toFloat() * 100.0f);
    }));

    addWidgetMapper("ColorPushButton", QSharedPointer<WidgetMapper>::create(_ui->colorPushButton, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->colorPushButton->setEnabled(false);
            _ui->colorPushButton->setColor(Qt::gray);

            return;
        }

        _ui->colorPushButton->setVisible(index.flags() & Qt::ItemIsEditable);
        _ui->colorPushButton->setEnabled(index.flags() & Qt::ItemIsEnabled);
        _ui->colorPushButton->setToolTip(index.data(Qt::ToolTipRole).toString());
        _ui->colorPushButton->setColor(index.data(Qt::EditRole).value<QColor>());
    }));
}

void StylingWidget::setModelIndex(const QPersistentModelIndex& modelIndex)
{
    TreeItemWidget::setModelIndex(modelIndex);
    
    getWidgetMapper("LineTypesProfile")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::LineTypes)));
    getWidgetMapper("LineTypesRange")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::LineTypes)));
    getWidgetMapper("LineTypeProfileComboBox")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::LineTypeProfile)));
    getWidgetMapper("LineTypeRangeComboBox")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::LineTypeRange)));
    getWidgetMapper("RenderPointsCheckBox")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::RenderPoints)));
    getWidgetMapper("OpacitySpinBox")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::Opacity)));
    getWidgetMapper("OpacitySlider")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::Opacity)));
    getWidgetMapper("ColorPushButton")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::Color)));
}