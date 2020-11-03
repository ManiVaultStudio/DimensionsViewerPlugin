#include "SettingsWidget.h"
#include "DimensionsViewerPlugin.h"
#include "Application.h"

#include "ui_SettingsWidget.h"

SettingsWidget::SettingsWidget(QWidget* parent) :
	QWidget(parent),
	_ui{ std::make_unique<Ui::SettingsWidget>() }
{
	_ui->setupUi(this);

    _dimensionsViewerPlugin = dynamic_cast<DimensionsViewerPlugin*>(parent);
}