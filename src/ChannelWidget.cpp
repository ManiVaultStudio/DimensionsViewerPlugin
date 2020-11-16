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
    TreeItemWidget(parent),
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

    /*QObject::connect(_ui->colorPushButton, &ColorPickerPushButton::colorChanged, [this](const QColor& color) {
        setData(to_ul(Channel::Column::Color), color);
    });

    QObject::connect(_ui->profileTypeComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        setData(to_ul(Channel::Column::ProfileType), currentText, Qt::DisplayRole);
    });

    QObject::connect(_ui->rangeTypeComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        setData(to_ul(Channel::Column::RangeType), currentText, Qt::DisplayRole);
    });*/

    QObject::connect(_ui->linkedPushButton, &QPushButton::toggled, [this](bool checked) {
        setData(to_ul(Channel::Column::Linked), checked);
    });

    addWidgetMapper("Enabled", QSharedPointer<WidgetMapper>::create(_ui->enabledCheckBox, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->enabledCheckBox->setEnabled(false);
            _ui->enabledCheckBox->setChecked(false);
            _ui->enabledCheckBox->setText("Channel");

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
    TreeItemWidget::setModelIndex(modelIndex);

    _ui->stylingPushButton->setModelIndex(modelIndex);

    getWidgetMapper("Enabled")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::Enabled)));
    getWidgetMapper("DisplayName")->setModelIndex(getSiblingAtColumn(to_ul(TreeItem::Column::Name)));
    getWidgetMapper("DatasetNames")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::DatasetNames)));
    getWidgetMapper("DatasetName")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::DatasetName)));

    const auto differentialIndex = getChild(to_ul(Channel::Row::Styling));

    //getWidgetMapper("Color")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::Color)));
    //getWidgetMapper("Styling")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::Styling)));
    //getWidgetMapper("LinkedPushButton")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Column::Linked)));
}