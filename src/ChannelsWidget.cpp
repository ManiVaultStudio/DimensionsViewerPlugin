#include "ChannelsWidget.h"
#include "ChannelWidget.h"
#include "DimensionsViewerPlugin.h"
#include "Channels.h"

#include "ui_ChannelsWidget.h"

#include <QDebug>

ChannelsWidget::ChannelsWidget(QWidget* parent) :
    TreeItemWidget(parent),
	_ui{ std::make_unique<Ui::ChannelsWidget>() }
{
	_ui->setupUi(this);
}

void ChannelsWidget::setModelIndex(const QPersistentModelIndex& modelIndex)
{
    TreeItemWidget::setModelIndex(modelIndex);

    _ui->channel1Widget->setModelIndex(getChild(static_cast<int>(Channels::Row::Dataset)));
    _ui->channel2Widget->setModelIndex(getChild(static_cast<int>(Channels::Row::Subset1)));
    _ui->channel3Widget->setModelIndex(getChild(static_cast<int>(Channels::Row::Subset2)));
    _ui->channel4Widget->setModelIndex(getChild(static_cast<int>(Channels::Row::Differential)));
}