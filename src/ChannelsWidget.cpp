#include "ChannelsWidget.h"
#include "ChannelWidget.h"
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

void ChannelsWidget::setModelIndex(const QPersistentModelIndex& modelIndex)
{
    ModelItemWidget::setModelIndex(modelIndex);

    _ui->channel1SettingsWidget->setModelIndex(getSiblingModelIndex(static_cast<int>(Channels::Row::Channel1)));
    _ui->channel2SettingsWidget->setModelIndex(getSiblingModelIndex(static_cast<int>(Channels::Row::Channel2)));
    _ui->channel3SettingsWidget->setModelIndex(getSiblingModelIndex(static_cast<int>(Channels::Row::Channel3)));
}