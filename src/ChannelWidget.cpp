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
        setData(to_ul(Channel::Column::Color), color);
    });

    QObject::connect(_ui->profileTypeComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        setData(to_ul(Channel::Column::ProfileType), currentText, Qt::DisplayRole);
    });

    QObject::connect(_ui->rangeTypeComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        setData(to_ul(Channel::Column::RangeType), currentText, Qt::DisplayRole);
    });

    QObject::connect(_ui->linkedPushButton, &QPushButton::toggled, [this](bool checked) {
        setData(to_ul(Channel::Column::Linked), checked);
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

    addWidgetMapper("DatasetNames", QSharedPointer<WidgetMapper>::create(_ui->datasetNameComboBox, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize)
            return;

        _ui->datasetNameComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
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

    addWidgetMapper("Differential", QSharedPointer<WidgetMapper>::create(_ui->differentialWidget, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->differentialWidget->setEnabled(false);

            return;
        }

        _ui->differentialWidget->setVisible(index.flags() & Qt::ItemIsEditable);
        _ui->differentialWidget->setEnabled(index.flags() & Qt::ItemIsEnabled);
        _ui->differentialWidget->setToolTip(index.data(Qt::ToolTipRole).toString());
    }));

    addWidgetMapper("DifferentialDatasetName1", QSharedPointer<WidgetMapper>::create(_ui->differentialDatasetName1ComboBox, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->differentialDatasetName1ComboBox->setEnabled(false);

            return;
        }

        _ui->differentialDatasetName1ComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
        _ui->differentialDatasetName1ComboBox->setCurrentText(index.data(Qt::EditRole).toString());
        _ui->differentialDatasetName1ComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
    }));

    addWidgetMapper("DifferentialDatasetName2", QSharedPointer<WidgetMapper>::create(_ui->differentialDatasetName2ComboBox, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->differentialDatasetName2ComboBox->setEnabled(false);

            return;
        }

        _ui->differentialDatasetName2ComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
        _ui->differentialDatasetName2ComboBox->setCurrentText(index.data(Qt::EditRole).toString());
        _ui->differentialDatasetName2ComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
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

    addWidgetMapper("Styling", QSharedPointer<WidgetMapper>::create(_ui->stylingPushButton, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->stylingPushButton->setEnabled(false);

            return;
        }

        _ui->stylingPushButton->setVisible(index.flags() & Qt::ItemIsEditable);
        _ui->stylingPushButton->setEnabled(index.flags() & Qt::ItemIsEnabled);
        _ui->stylingPushButton->setToolTip(index.data(Qt::ToolTipRole).toString());
    }));

    addWidgetMapper("LinkedPushButton", QSharedPointer<WidgetMapper>::create(_ui->linkedPushButton, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->linkedPushButton->setEnabled(false);

            QSizePolicy sizePolicyRetain = _ui->linkedPushButton->sizePolicy();
            sizePolicyRetain.setRetainSizeWhenHidden(true);
            
            _ui->linkedPushButton->setSizePolicy(sizePolicyRetain);

            return;
        }

        _ui->linkedPushButton->setVisible(index.flags() & Qt::ItemIsEditable);
        _ui->linkedPushButton->setEnabled(index.flags() & Qt::ItemIsEnabled);
        _ui->linkedPushButton->setToolTip(index.data(Qt::ToolTipRole).toString());
        _ui->linkedPushButton->setFont(index.data(Qt::FontRole).value<QFont>());
        _ui->linkedPushButton->setChecked(index.data(Qt::EditRole).toBool());
        _ui->linkedPushButton->setText(index.data(Qt::DisplayRole).toString());
    }));
}

void ChannelWidget::setModelIndex(const QPersistentModelIndex& modelIndex)
{
    ModelItemWidget::setModelIndex(modelIndex);

    _ui->stylingPushButton->setModelIndex(modelIndex);

    getWidgetMapper("Enabled")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::Enabled)));
    getWidgetMapper("DisplayName")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::DisplayName)));
    getWidgetMapper("DatasetNames")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::DatasetNames)));
    getWidgetMapper("DatasetName")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::DatasetName)));
    getWidgetMapper("Differential")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::Differential)));
    getWidgetMapper("DifferentialDatasetName1")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::DifferentialDatasetName1)));
    getWidgetMapper("DifferentialDatasetName2")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::DifferentialDatasetName2)));
    getWidgetMapper("Color")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::Color)));
    getWidgetMapper("ProfileTypes")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::ProfileTypes)));
    getWidgetMapper("ProfileType")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::ProfileType)));
    getWidgetMapper("RangeTypes")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::RangeTypes)));
    getWidgetMapper("RangeType")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::RangeType)));
    getWidgetMapper("Styling")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::Styling)));
    getWidgetMapper("LinkedPushButton")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::Linked)));
}