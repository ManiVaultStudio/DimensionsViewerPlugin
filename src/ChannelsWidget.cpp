#include "ChannelsWidget.h"
#include "ChannelSettingsWidget.h"
#include "DimensionsViewerPlugin.h"

#include "ui_ChannelsWidget.h"

#include <QDebug>

DimensionsViewerPlugin* ChannelsWidget::dimensionsViewerPlugin = nullptr;

ChannelsWidget::ChannelsWidget(QWidget* parent) :
    QWidget(parent),
	_ui{ std::make_unique<Ui::ChannelsWidget>() }
{
	_ui->setupUi(this);

    if (parent == nullptr)
        return;

    for (int channelIndex = 0; channelIndex < 3; channelIndex++)
        _ui->channelsLayout->addWidget(new ChannelSettingsWidget(this, channelIndex));
}

void ChannelsWidget::updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles /*= QVector<int>()*/)
{

}