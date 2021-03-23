#include "ChannelWidget.h"
#include "DimensionsViewerPlugin.h"
#include "Styling.h"
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
        getModel().setData(getChild(to_ul(Channel::Child::Enabled), to_ul(Channel::Column::Value)), static_cast<bool>(state));
    });
    
    QObject::connect(_ui->datasetNameComboBox, qOverload<int>(&QComboBox::currentIndexChanged), [this](int currentIndex) {
        getModel().setData(getChild(to_ul(Channel::Child::DatasetName), to_ul(Channel::Column::Value)), _ui->datasetNameComboBox->currentText());
    });

    /*
    QObject::connect(_ui->colorPushButton, &ColorPickerPushButton::colorChanged, [this](const QColor& color) {
        const auto stylingIndex = getChild(to_ul(Channel::Row::Styling));

        getModel().setData(stylingIndex.si)
        setData(to_ul(Channel::Column::Color), color);
    });
    
    */

    QObject::connect(_ui->linkedPushButton, &QPushButton::toggled, [this](bool checked) {
        getModel().setData(getChild(to_ul(Channel::Child::Linked), to_ul(Channel::Column::Value)), checked);
    });
    
    /*addWidgetMapper("DatasetNameFlags", QSharedPointer<WidgetMapper>::create(this, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize)
            return;
    }));*/

    addWidgetMapper("Enabled", QSharedPointer<WidgetMapper>::create(_ui->enabledCheckBox, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->enabledCheckBox->setText("Channel");
            return;
        }

        _ui->enabledCheckBox->setChecked(index.data(Qt::EditRole).toBool());
        _ui->enabledCheckBox->setToolTip(index.data(Qt::ToolTipRole).toString());
    }));

    addWidgetMapper("Name", QSharedPointer<WidgetMapper>::create(_ui->enabledCheckBox, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
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
        if (initialize)
            return;

        _ui->datasetNameComboBox->setCurrentText(index.data(Qt::EditRole).toString());
        _ui->datasetNameComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
    }));

    
    addWidgetMapper("Differential", QSharedPointer<WidgetMapper>::create(_ui->differentialWidget, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize)
            return;

        _ui->differentialWidget->setToolTip(index.data(Qt::ToolTipRole).toString());
    }));

    addWidgetMapper("Profile", QSharedPointer<WidgetMapper>::create(_ui->profileWidget, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize)
            return;

        _ui->profileWidget->setToolTip(index.data(Qt::ToolTipRole).toString());
    }));

    addWidgetMapper("Styling", QSharedPointer<WidgetMapper>::create(_ui->stylingPushButton, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize)
            return;

        _ui->stylingPushButton->setToolTip(index.data(Qt::ToolTipRole).toString());
    }));

    addWidgetMapper("Color", QSharedPointer<WidgetMapper>::create(_ui->colorPushButton, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            _ui->colorPushButton->setColor(Qt::gray);
            _ui->colorPushButton->setShowText(false);

            return;
        }

        _ui->colorPushButton->setColor(index.data(Qt::EditRole).value<QColor>());
        _ui->colorPushButton->setToolTip(index.data(Qt::ToolTipRole).toString());
    }));

    addWidgetMapper("Linked", QSharedPointer<WidgetMapper>::create(_ui->linkedPushButton, [this](const QPersistentModelIndex& index, const bool& initialize) {
        if (initialize) {
            QSizePolicy sizePolicyRetain = _ui->linkedPushButton->sizePolicy();
            sizePolicyRetain.setRetainSizeWhenHidden(true);
            
            _ui->linkedPushButton->setSizePolicy(sizePolicyRetain);

            return;
        }

        _ui->linkedPushButton->setToolTip(index.data(Qt::ToolTipRole).toString());
        _ui->linkedPushButton->setFont(index.data(to_ul(ConfigurationsModel::Role::IconFontRole)).value<QFont>());
        _ui->linkedPushButton->setChecked(index.data(Qt::EditRole).toBool());
        _ui->linkedPushButton->setText(index.data(to_ul(ConfigurationsModel::Role::IconFontCharacterRole)).toString());
    }));
}

void ChannelWidget::setModelIndex(const QPersistentModelIndex& modelIndex)
{
    TreeItemWidget::setModelIndex(modelIndex);

    
    _ui->differentialWidget->setModelIndex(getChild(static_cast<int>(Channel::Child::Differential)));
    _ui->profileWidget->setModelIndex(getChild(static_cast<int>(Channel::Child::Profile)));
    _ui->stylingPushButton->setModelIndex(getChild(static_cast<int>(Channel::Child::Styling)));

    //getWidgetMapper("Flags")->setModelIndex(getSiblingAtColumn(to_ul(tree::Item::Column::Flags)));

    getWidgetMapper("Enabled")->setModelIndex(getChild(to_ul(Channel::Child::Enabled), to_ul(Channel::Column::Value)));
    getWidgetMapper("EnabledFlags")->setModelIndex(getChild(to_ul(Channel::Child::Enabled), to_ul(Channel::Column::Value)));
    getWidgetMapper("Name")->setModelIndex(getSiblingAtColumn(to_ul(Channel::Item::Column::Name)));
    getWidgetMapper("DatasetNames")->setModelIndex(getChild(to_ul(Channel::Child::DatasetNames), to_ul(Channel::Column::Value)));
    getWidgetMapper("DatasetName")->setModelIndex(getChild(to_ul(Channel::Child::DatasetName)));
    getWidgetMapper("Differential")->setModelIndex(getChild(to_ul(Channel::Child::Differential)));
    getWidgetMapper("Profile")->setModelIndex(getChild(to_ul(Channel::Child::Profile)));
    getWidgetMapper("Styling")->setModelIndex(getChild(to_ul(Channel::Child::Styling)));
    
    //const auto stylingIndex = getChild(to_ul(Channel::Row::Styling));

    getWidgetMapper("Color")->setModelIndex(getChild(to_ul(Styling::Child::Color)));
    getWidgetMapper("Linked")->setModelIndex(getChild(to_ul(Channel::Child::Linked), to_ul(Channel::Column::Value)));
}