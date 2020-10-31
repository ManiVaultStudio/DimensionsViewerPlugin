#pragma once

#include <memory>

#include <QWidget.h>
#include <QVector.h>

class DimensionsViewerPlugin;
class ColorPickerPushButton;

class QCheckBox;
class QComboBox;
class QPushButton;
class QSlider;

namespace Ui {
	class SettingsWidget;
}

/**
 * Settings widget class
 *
 * UI widget class for interfacing with dataset configurations
 *
 * @author T. Kroes
 */
class SettingsWidget : public QWidget
{
	Q_OBJECT

public: // Construction

    /**
     * Constructor
     * @param dimensionsViewerPlugin Pointer to an instance of the dimensions viewer plugin
     */
	SettingsWidget(DimensionsViewerPlugin* dimensionsViewerPlugin);

public:

	/**
	 * Updates the UI with model indices ranging from \p begin to \p end
	 * @param begin Start of model index range
	 * @param end End of model index range
	 * @param roles Data roles
	 */
	void updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles = QVector<int>());

private:
	DimensionsViewerPlugin*					_dimensionsViewerPlugin;        /** Pointer to an instance of the dimensions viewer plugin */
	std::unique_ptr<Ui::SettingsWidget>		_ui;                            /** UI from creator */
    QVector<QCheckBox*>                     _enabledCheckBoxes;             /** Enabled check boxes */
    QVector<QComboBox*>                     _datasetNameComboBoxes;         /** Dataset name combo boxes */
    QVector<ColorPickerPushButton*>         _colorPushButtons;              /** Color push buttons */
    QVector<QSlider*>                       _opacitySliders;                /** Opacity sliders */
    QVector<QComboBox*>                     _profileTypeComboBoxes;         /** Color push buttons */
    QVector<QComboBox*>                     _bandTypeComboBoxes;            /** Profile type combo boxes */
    QVector<QCheckBox*>                     _showRangeCheckBoxes;           /** Band type combo boxes */
    QVector<QPushButton*>                   _lockedPushButtons;             /** Locked push buttons */
    QVector<QWidget*>                       _differentialProfileWidgets;    /** Widget for differential profile */
};