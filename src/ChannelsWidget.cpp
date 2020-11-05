#include "ChannelsWidget.h"
#include "ChannelSettingsWidget.h"
#include "DimensionsViewerPlugin.h"
#include "Channels.h"

#include "ui_ChannelsWidget.h"

#include <QDebug>

ChannelsWidget::ChannelsWidget(QWidget* parent) :
    ModelItemWidget(parent),
	_ui{ std::make_unique<Ui::ChannelsWidget>() }
{
	_ui->setupUi(this);

    reset();
}

void ChannelsWidget::updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles /*= QVector<int>()*/)
{
}

void ChannelsWidget::setPersistentModelIndex(const QPersistentModelIndex& modelIndex)
{
    ModelItemWidget::setPersistentModelIndex(modelIndex);

    _ui->channel1SettingsWidget->setPersistentModelIndex(getSiblingModelIndex(static_cast<int>(Channels::Child::Channel1)));
    _ui->channel2SettingsWidget->setPersistentModelIndex(getSiblingModelIndex(static_cast<int>(Channels::Child::Channel2)));
    _ui->channel3SettingsWidget->setPersistentModelIndex(getSiblingModelIndex(static_cast<int>(Channels::Child::Channel3)));
}