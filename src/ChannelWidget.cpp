#include "ChannelWidget.h"
#include "DimensionsViewerPlugin.h"
#include "Application.h"

#include "ui_ChannelWidget.h"

#include <QDebug>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QStringListModel>

ChannelWidget::ChannelWidget(QWidget* parent) :
    ModelItemWidget(parent),
	_ui{ std::make_unique<Ui::ChannelWidget>() }
{
	_ui->setupUi(this);

    QObject::connect(_ui->enabledCheckBox, &QCheckBox::stateChanged, [this](int state) {
        setData(to_ul(Channel::Column::Enabled), state);
    });
    
    QObject::connect(_ui->datasetNameComboBox, qOverload<int>(&QComboBox::currentIndexChanged), [this](int currentIndex) {
        if (_modelIndex.row() == 0)
            getModel().selectRow(currentIndex);
        else
            setData(to_ul(Channel::Column::DatasetName), _ui->datasetNameComboBox->currentText());
    });

    QObject::connect(_ui->colorPushButton, &ColorPickerPushButton::colorChanged, [this](const QColor& color) {
        const auto stylingIndex = getChild(to_ul(Channel::Row::Styling));
        getModel().setData(getSiblingAtColumn(to_ul(Styling::Column::Color), stylingIndex), color);
    });

    QObject::connect(_ui->profileTypeComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        const auto profileIndex = getChild(to_ul(Channel::Row::Profile));
        getModel().setData(getSiblingAtColumn(to_ul(Profile::Column::ProfileType), profileIndex), currentText);
    });

    QObject::connect(_ui->rangeTypeComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        const auto profileIndex = getChild(to_ul(Channel::Row::Profile));
        getModel().setData(getSiblingAtColumn(to_ul(Profile::Column::RangeType), profileIndex), currentText);
    });

    addWidgetMapper("Enabled", QSharedPointer<WidgetMapper>::create(_ui->enabledCheckBox, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->enabledCheckBox->setEnabled(false);
            _ui->enabledCheckBox->setChecked(false);

            return;
        }

        _ui->enabledCheckBox->setVisible(index.flags() & Qt::ItemIsEditable);
        _ui->enabledCheckBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
        _ui->enabledCheckBox->setChecked(index.data(Qt::EditRole).toBool());
        _ui->enabledCheckBox->setToolTip(index.data(Qt::ToolTipRole).toString());
    }));

    addWidgetMapper("DisplayName", QSharedPointer<WidgetMapper>::create(_ui->enabledCheckBox, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->enabledCheckBox->setEnabled(false);
            _ui->enabledCheckBox->setChecked(false);

            return;
        }

        _ui->enabledCheckBox->setText(index.data(Qt::EditRole).toString());
    }));

    addWidgetMapper("DatasetName", QSharedPointer<WidgetMapper>::create(_ui->datasetNameComboBox, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->datasetNameComboBox->setEnabled(false);

            return;
        }

        _ui->datasetNameComboBox->setVisible(index.flags() & Qt::ItemIsEditable);
        _ui->datasetNameComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
        _ui->datasetNameComboBox->setCurrentText(index.data(Qt::EditRole).toString());
        _ui->datasetNameComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
    }));

    addWidgetMapper("DatasetNames", QSharedPointer<WidgetMapper>::create(_ui->datasetNameComboBox, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize)
            return;

        qDebug() << index.data(Qt::EditRole).toStringList();

        _ui->datasetNameComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
    }));

    addWidgetMapper("Color", QSharedPointer<WidgetMapper>::create(_ui->colorPushButton, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->colorPushButton->setEnabled(false);
            _ui->colorPushButton->setColor(Qt::gray);
            _ui->colorPushButton->setShowText(false);

            return;
        }

        _ui->colorPushButton->setVisible(index.flags() & Qt::ItemIsEditable);
        _ui->colorPushButton->setEnabled(index.flags() & Qt::ItemIsEnabled);
        _ui->colorPushButton->setColor(index.data(Qt::EditRole).value<QColor>());
        _ui->colorPushButton->setToolTip(index.data(Qt::ToolTipRole).toString());
    }));

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

        _ui->profileTypeComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
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

    addWidgetMapper("Styling", QSharedPointer<WidgetMapper>::create(_ui->stylingPushButton, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->stylingPushButton->setEnabled(false);

            return;
        }

        _ui->stylingPushButton->setVisible(index.flags() & Qt::ItemIsEditable);
        _ui->stylingPushButton->setEnabled(index.flags() & Qt::ItemIsEnabled);
        _ui->stylingPushButton->setToolTip(index.data(Qt::ToolTipRole).toString());
    }));
}

void ChannelWidget::setModelIndex(const QPersistentModelIndex& modelIndex)
{
    ModelItemWidget::setModelIndex(modelIndex);

    const auto profileIndex = getChild(to_ul(Channel::Row::Profile));
    const auto stylingIndex = getChild(to_ul(Channel::Row::Styling));

    _ui->stylingPushButton->setModelIndex(stylingIndex);

    getWidgetMapper("Enabled")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::Enabled)));
    getWidgetMapper("DisplayName")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::DisplayName)));
    getWidgetMapper("DatasetName")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::DatasetName)));
    getWidgetMapper("DatasetNames")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::DatasetNames)));
    getWidgetMapper("Color")->setModelIndex(getSiblingAtColumn(to_ul(Styling::Column::Color), stylingIndex));
    getWidgetMapper("ProfileTypes")->setModelIndex(getSiblingAtColumn(to_ul(Profile::Column::ProfileTypes), profileIndex));
    getWidgetMapper("ProfileType")->setModelIndex(getSiblingAtColumn(to_ul(Profile::Column::ProfileType), profileIndex));
    getWidgetMapper("RangeTypes")->setModelIndex(getSiblingAtColumn(to_ul(Profile::Column::RangeTypes), profileIndex));
    getWidgetMapper("RangeType")->setModelIndex(getSiblingAtColumn(to_ul(Profile::Column::RangeType), profileIndex));
    getWidgetMapper("Styling")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::Styling)));
}