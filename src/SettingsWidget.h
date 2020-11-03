#pragma once

#include <memory>

#include <QWidget.h>

class DimensionsViewerPlugin;

namespace Ui {
	class SettingsWidget;
}

/**
 * Settings container widget class
 *
 * @author T. Kroes
 */
class SettingsWidget : public QWidget
{
	Q_OBJECT

public: // Construction

    /**
     * Constructor
     * @param parent Parent widget
     */
	SettingsWidget(QWidget* parent);

private:
	DimensionsViewerPlugin*					_dimensionsViewerPlugin;        /** Pointer to an instance of the dimensions viewer plugin */
	std::unique_ptr<Ui::SettingsWidget>		_ui;                            /** UI from creator */
};