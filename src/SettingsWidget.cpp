#include "SettingsWidget.h"
#include "DimensionsViewerPlugin.h"
#include "Application.h"

#include "ui_SettingsWidget.h"

#include <QDebug>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QStringListModel>

SettingsWidget::SettingsWidget(DimensionsViewerPlugin* dimensionsViewerPlugin) :
	_dimensionsViewerPlugin(dimensionsViewerPlugin),
	_ui{ std::make_unique<Ui::SettingsWidget>() }
{
	_ui->setupUi(this);

	_locked << _ui->channel1LockedPushButton << _ui->channel2LockedPushButton << _ui->channel3LockedPushButton;
	_showRange << _ui->channel1ShowRangeCheckBox<< _ui->channel2ShowRangeCheckBox << _ui->channel3ShowRangeCheckBox;

	auto& configurationsModel = _dimensionsViewerPlugin->getConfigurationsModel();

	const auto updateDataset1Combobox = [this]() {
		auto& configurationsModel = _dimensionsViewerPlugin->getConfigurationsModel();

		_ui->channel1DatasetNameComboBox->blockSignals(true);
		_ui->channel1DatasetNameComboBox->setModel(new QStringListModel(configurationsModel.getConfigurationNames()));
		_ui->channel1DatasetNameComboBox->setEnabled(configurationsModel.rowCount() > 0);
		_ui->channel1DatasetNameComboBox->blockSignals(false);
	};
	
	QObject::connect(&configurationsModel, &ConfigurationsModel::rowsInserted, [this, updateDataset1Combobox](const QModelIndex& parent, int first, int last) {
		updateDataset1Combobox();
	});

	QObject::connect(&configurationsModel, &ConfigurationsModel::rowsRemoved, [this, updateDataset1Combobox](const QModelIndex& parent, int first, int last) {
		updateDataset1Combobox();
	});

	QObject::connect(&configurationsModel, &ConfigurationsModel::dataChanged, this, &SettingsWidget::updateData);

	QObject::connect(&configurationsModel.getSelectionModel(), &QItemSelectionModel::selectionChanged, [this, &configurationsModel](const QItemSelection& selected, const QItemSelection& deselected) {
		const auto selectedRows = configurationsModel.getSelectionModel().selectedRows();

		if (selectedRows.isEmpty())
			updateData(QModelIndex(), QModelIndex());
		else {
			const auto first = selectedRows.first();

			updateData(first.siblingAtColumn(static_cast<int>(Configuration::Column::Start)), first.siblingAtColumn(static_cast<int>(Configuration::Column::End)));
		}
	});

	_ui->channel1ColorPushButton->setShowText(false);
	_ui->channel2ColorPushButton->setShowText(false);
	_ui->channel3ColorPushButton->setShowText(false);

	_ui->channel1ColorPushButton->setColor(Qt::gray);
	_ui->channel2ColorPushButton->setColor(Qt::gray);
	_ui->channel3ColorPushButton->setColor(Qt::gray);

	_ui->channel1OpacitySlider->setVisible(false);
	_ui->channel2OpacitySlider->setVisible(false);
	_ui->channel3OpacitySlider->setVisible(false);

	_ui->channel1ProfileTypeComboBox->setModel(new QStringListModel(Channel::getProfileTypeNames()));
	_ui->channel2ProfileTypeComboBox->setModel(new QStringListModel(Channel::getProfileTypeNames()));
	_ui->channel3ProfileTypeComboBox->setModel(new QStringListModel(Channel::getProfileTypeNames()));

	_ui->channel1BandTypeComboBox->setModel(new QStringListModel(Channel::getBandTypeNames()));
	_ui->channel2BandTypeComboBox->setModel(new QStringListModel(Channel::getBandTypeNames()));
	_ui->channel3BandTypeComboBox->setModel(new QStringListModel(Channel::getBandTypeNames()));

	_ui->channel1BandTypeComboBox->setVisible(false);
	_ui->channel2BandTypeComboBox->setVisible(false);
	_ui->channel3BandTypeComboBox->setVisible(false);

	QFont font = QFont("Font Awesome 5 Free Solid", 8);

	QObject::connect(_ui->channel1DatasetNameComboBox, qOverload<int>(&QComboBox::currentIndexChanged), [this, &configurationsModel](int currentIndex) {
		configurationsModel.selectRow(currentIndex);
	});

	QObject::connect(_ui->channel2DatasetNameComboBox, &QComboBox::currentTextChanged, [this, &configurationsModel](const QString& currentText) {
		configurationsModel.setData(Configuration::Column::Channel2DatasetName, currentText);
	});

	QObject::connect(_ui->channel3DatasetNameComboBox, &QComboBox::currentTextChanged, [this, &configurationsModel](const QString& currentText) {
		configurationsModel.setData(Configuration::Column::Channel3DatasetName, currentText);
	});

	QObject::connect(_ui->channel1EnabledCheckBox, &QCheckBox::stateChanged, [this, &configurationsModel](int state) {
		configurationsModel.setData(Configuration::Column::Channel1Enabled, state == Qt::Checked);
	});

	QObject::connect(_ui->channel2EnabledCheckBox, &QCheckBox::stateChanged, [this, &configurationsModel](int state) {
		configurationsModel.setData(Configuration::Column::Channel2Enabled, state == Qt::Checked);
	});

	QObject::connect(_ui->channel3EnabledCheckBox, &QCheckBox::stateChanged, [this, &configurationsModel](int state) {
		configurationsModel.setData(Configuration::Column::Channel3Enabled, state == Qt::Checked);
	});

	QObject::connect(_ui->channel1ColorPushButton, &ColorPickerPushButton::colorChanged, [this, &configurationsModel](const QColor& color) {
		configurationsModel.setData(Configuration::Column::Channel1Color, color);
	});

	QObject::connect(_ui->channel2ColorPushButton, &ColorPickerPushButton::colorChanged, [this, &configurationsModel](const QColor& color) {
		configurationsModel.setData(Configuration::Column::Channel2Color, color);
	});

	QObject::connect(_ui->channel3ColorPushButton, &ColorPickerPushButton::colorChanged, [this, &configurationsModel](const QColor& color) {
		configurationsModel.setData(Configuration::Column::Channel3Color, color);
	});

	QObject::connect(_ui->channel1OpacitySlider, &QSlider::valueChanged, [this, &configurationsModel](int value) {
		configurationsModel.setData(Configuration::Column::Channel1Opacity, value / 100.0f);
	});

	QObject::connect(_ui->channel2OpacitySlider, &QSlider::valueChanged, [this, &configurationsModel](int value) {
		configurationsModel.setData(Configuration::Column::Channel2Opacity, value / 100.0f);
	});

	QObject::connect(_ui->channel3OpacitySlider, &QSlider::valueChanged, [this, &configurationsModel](int value) {
		configurationsModel.setData(Configuration::Column::Channel3Opacity, value / 100.0f);
	});

	QObject::connect(_ui->channel1ProfileTypeComboBox, qOverload<int>(&QComboBox::currentIndexChanged), [this, &configurationsModel](int currentIndex) {
		configurationsModel.setData(Configuration::Column::Channel1ProfileType, currentIndex);
	});

	QObject::connect(_ui->channel2ProfileTypeComboBox, qOverload<int>(&QComboBox::currentIndexChanged), [this, &configurationsModel](int currentIndex) {
		configurationsModel.setData(Configuration::Column::Channel2ProfileType, currentIndex);
	});

	QObject::connect(_ui->channel3ProfileTypeComboBox, qOverload<int>(&QComboBox::currentIndexChanged), [this, &configurationsModel](int currentIndex) {
		configurationsModel.setData(Configuration::Column::Channel3ProfileType, currentIndex);
	});

	QObject::connect(_ui->channel1BandTypeComboBox, qOverload<int>(&QComboBox::currentIndexChanged), [this, &configurationsModel](int currentIndex) {
		configurationsModel.setData(Configuration::Column::Channel1BandType, currentIndex);
	});

	QObject::connect(_ui->channel2BandTypeComboBox, qOverload<int>(&QComboBox::currentIndexChanged), [this, &configurationsModel](int currentIndex) {
		configurationsModel.setData(Configuration::Column::Channel2BandType, currentIndex);
	});

	QObject::connect(_ui->channel3BandTypeComboBox, qOverload<int>(&QComboBox::currentIndexChanged), [this, &configurationsModel](int currentIndex) {
		configurationsModel.setData(Configuration::Column::Channel3BandType, currentIndex);
	});

	for (auto showRange : _showRange) {
		QObject::connect(showRange, &QCheckBox::stateChanged, [this, &configurationsModel, showRange](int state) {
			configurationsModel.setData(Configuration::Column::ChannelShowRangeStart + _showRange.indexOf(showRange), state == Qt::Checked);
		});
	}

	for (auto locked : _locked) {
		locked->setFont(font);
		locked->setStyleSheet("text-align: center");
		locked->setText(hdps::Application::getIconFont("FontAwesome").getIconCharacter("lock"));

		QObject::connect(locked, &QPushButton::toggled, [this, &configurationsModel, locked](bool checked) {
			configurationsModel.setData(Configuration::Column::ChannelLockedStart + _locked.indexOf(locked), checked);
		});
	}

	updateData(QModelIndex(), QModelIndex());
}

void SettingsWidget::updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles /*= QVector<int>()*/)
{
	const auto selectedRows = _dimensionsViewerPlugin->getConfigurationsModel().getSelectionModel().selectedRows();

	if (selectedRows.isEmpty()) {
		_ui->settingsGroupBox->setEnabled(false);
		_ui->channel1EnabledCheckBox->setChecked(false);
		_ui->channel2EnabledCheckBox->setChecked(false);
		_ui->channel3EnabledCheckBox->setChecked(false);
		_ui->channel1DatasetNameComboBox->setCurrentIndex(-1);
		_ui->channel2DatasetNameComboBox->setCurrentIndex(-1);
		_ui->channel3DatasetNameComboBox->setCurrentIndex(-1);
		_ui->channel1ProfileTypeComboBox->setCurrentIndex(-1);
		_ui->channel2ProfileTypeComboBox->setCurrentIndex(-1);
		_ui->channel3ProfileTypeComboBox->setCurrentIndex(-1);
		_ui->channel1BandTypeComboBox->setCurrentIndex(-1);
		_ui->channel2BandTypeComboBox->setCurrentIndex(-1);
		_ui->channel3BandTypeComboBox->setCurrentIndex(-1);
		
		for (auto showRange : _showRange) {
			showRange->setEnabled(false);
			showRange->setChecked(false);
		}

		for (auto locked : _locked) {
			locked->setEnabled(false);
			locked->setChecked(false);
		}

		return;
	}

	if (begin.row() != selectedRows.first().row())
		return;

	_ui->settingsGroupBox->setEnabled(true);

	for (int column = begin.column(); column <= end.column(); column++) {
		const auto index = begin.siblingAtColumn(column);

		if (column == static_cast<int>(Configuration::Column::Channel1Enabled)) {
			_ui->channel1EnabledCheckBox->blockSignals(true);
			_ui->channel1EnabledCheckBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->channel1EnabledCheckBox->setChecked(index.data(Qt::EditRole).toBool());
			_ui->channel1EnabledCheckBox->setToolTip(index.data(Qt::ToolTipRole).toString());
			_ui->channel1EnabledCheckBox->blockSignals(false);
		}

		if (column == static_cast<int>(Configuration::Column::Channel2Enabled)) {
			_ui->channel2EnabledCheckBox->blockSignals(true);
			_ui->channel2EnabledCheckBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->channel2EnabledCheckBox->setChecked(index.data(Qt::EditRole).toBool());
			_ui->channel2EnabledCheckBox->setToolTip(index.data(Qt::ToolTipRole).toString());
			_ui->channel2EnabledCheckBox->blockSignals(false);
		}

		if (column == static_cast<int>(Configuration::Column::Channel3Enabled)) {
			_ui->channel3EnabledCheckBox->blockSignals(true);
			_ui->channel3EnabledCheckBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->channel3EnabledCheckBox->setChecked(index.data(Qt::EditRole).toBool());
			_ui->channel3EnabledCheckBox->setToolTip(index.data(Qt::ToolTipRole).toString());
			_ui->channel3EnabledCheckBox->blockSignals(false);
		}

		if (column == static_cast<int>(Configuration::Column::Subsets)) {
			const auto subsets = index.data(Qt::EditRole).toStringList();

			_ui->channel2DatasetNameComboBox->blockSignals(true);
			_ui->channel2DatasetNameComboBox->setModel(new QStringListModel(subsets));
			_ui->channel2DatasetNameComboBox->blockSignals(false);

			_ui->channel3DatasetNameComboBox->blockSignals(true);
			_ui->channel3DatasetNameComboBox->setModel(new QStringListModel(subsets));
			_ui->channel3DatasetNameComboBox->blockSignals(false);
		}

		if (column == static_cast<int>(Configuration::Column::Channel1DatasetName)) {
			_ui->channel1DatasetNameComboBox->blockSignals(true);
			_ui->channel1DatasetNameComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->channel1DatasetNameComboBox->setCurrentText(index.data(Qt::EditRole).toString());
			_ui->channel1DatasetNameComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
			_ui->channel1DatasetNameComboBox->blockSignals(false);
		}

		if (column == static_cast<int>(Configuration::Column::Channel2DatasetName)) {
			_ui->channel2DatasetNameComboBox->blockSignals(true);
			_ui->channel2DatasetNameComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->channel2DatasetNameComboBox->setCurrentText(index.data(Qt::EditRole).toString());
			_ui->channel2DatasetNameComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
			_ui->channel2DatasetNameComboBox->blockSignals(false);
		}

		if (column == static_cast<int>(Configuration::Column::Channel3DatasetName)) {
			_ui->channel3DatasetNameComboBox->blockSignals(true);
			_ui->channel3DatasetNameComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->channel3DatasetNameComboBox->setCurrentText(index.data(Qt::EditRole).toString());
			_ui->channel3DatasetNameComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
			_ui->channel3DatasetNameComboBox->blockSignals(false);
		}

		if (column == static_cast<int>(Configuration::Column::Channel1Color)) {
			_ui->channel1ColorPushButton->blockSignals(true);
			_ui->channel1ColorPushButton->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->channel1ColorPushButton->setColor(index.data(Qt::EditRole).value<QColor>());
			_ui->channel1ColorPushButton->setToolTip(index.data(Qt::ToolTipRole).toString());
			_ui->channel1ColorPushButton->blockSignals(false);
		}

		if (column == static_cast<int>(Configuration::Column::Channel2Color)) {
			_ui->channel2ColorPushButton->blockSignals(true);
			_ui->channel2ColorPushButton->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->channel2ColorPushButton->setColor(index.data(Qt::EditRole).value<QColor>());
			_ui->channel2ColorPushButton->setToolTip(index.data(Qt::ToolTipRole).toString());
			_ui->channel2ColorPushButton->blockSignals(false);
		}

		if (column == static_cast<int>(Configuration::Column::Channel3Color)) {
			_ui->channel3ColorPushButton->blockSignals(true);
			_ui->channel3ColorPushButton->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->channel3ColorPushButton->setColor(index.data(Qt::EditRole).value<QColor>());
			_ui->channel3ColorPushButton->setToolTip(index.data(Qt::ToolTipRole).toString());
			_ui->channel3ColorPushButton->blockSignals(false);
		}

		if (column == static_cast<int>(Configuration::Column::Channel1Opacity)) {
			_ui->channel1OpacitySlider->blockSignals(true);
			_ui->channel1OpacitySlider->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->channel1OpacitySlider->setValue(static_cast<int>(100.0f * index.data(Qt::EditRole).toFloat()));
			_ui->channel1OpacitySlider->setToolTip(index.data(Qt::ToolTipRole).toString());
			_ui->channel1OpacitySlider->blockSignals(false);
		}

		if (column == static_cast<int>(Configuration::Column::Channel2Opacity)) {
			_ui->channel2OpacitySlider->blockSignals(true);
			_ui->channel2OpacitySlider->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->channel2OpacitySlider->setValue(static_cast<int>(100.0f * index.data(Qt::EditRole).toFloat()));
			_ui->channel2OpacitySlider->setToolTip(index.data(Qt::ToolTipRole).toString());
			_ui->channel2OpacitySlider->blockSignals(false);
		}

		if (column == static_cast<int>(Configuration::Column::Channel3Opacity)) {
			_ui->channel3OpacitySlider->blockSignals(true);
			_ui->channel3OpacitySlider->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->channel3OpacitySlider->setValue(static_cast<int>(100.0f * index.data(Qt::EditRole).toFloat()));
			_ui->channel3OpacitySlider->setToolTip(index.data(Qt::ToolTipRole).toString());
			_ui->channel3OpacitySlider->blockSignals(false);
		}

		if (column == static_cast<int>(Configuration::Column::Channel1ProfileType)) {
			_ui->channel1ProfileTypeComboBox->blockSignals(true);
			_ui->channel1ProfileTypeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->channel1ProfileTypeComboBox->setCurrentIndex(index.data(Qt::EditRole).toInt());
			_ui->channel1ProfileTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
			_ui->channel1ProfileTypeComboBox->blockSignals(false);
		}

		if (column == static_cast<int>(Configuration::Column::Channel2ProfileType)) {
			_ui->channel2ProfileTypeComboBox->blockSignals(true);
			_ui->channel2ProfileTypeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->channel2ProfileTypeComboBox->setCurrentIndex(index.data(Qt::EditRole).toInt());
			_ui->channel2ProfileTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
			_ui->channel2ProfileTypeComboBox->blockSignals(false);
		}

		if (column == static_cast<int>(Configuration::Column::Channel3ProfileType)) {
			_ui->channel3ProfileTypeComboBox->blockSignals(true);
			_ui->channel3ProfileTypeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->channel3ProfileTypeComboBox->setCurrentIndex(index.data(Qt::EditRole).toInt());
			_ui->channel3ProfileTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
			_ui->channel3ProfileTypeComboBox->blockSignals(false);
		}

		if (column == static_cast<int>(Configuration::Column::Channel1BandType)) {
			_ui->channel1BandTypeComboBox->blockSignals(true);
			_ui->channel1BandTypeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->channel1BandTypeComboBox->setCurrentIndex(index.data(Qt::EditRole).toInt());
			_ui->channel1BandTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
			_ui->channel1BandTypeComboBox->blockSignals(false);
		}

		if (column == static_cast<int>(Configuration::Column::Channel2BandType)) {
			_ui->channel2BandTypeComboBox->blockSignals(true);
			_ui->channel2BandTypeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->channel2BandTypeComboBox->setCurrentIndex(index.data(Qt::EditRole).toInt());
			_ui->channel2BandTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
			_ui->channel2BandTypeComboBox->blockSignals(false);
		}

		if (column == static_cast<int>(Configuration::Column::Channel3BandType)) {
			_ui->channel3BandTypeComboBox->blockSignals(true);
			_ui->channel3BandTypeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->channel3BandTypeComboBox->setCurrentIndex(index.data(Qt::EditRole).toInt());
			_ui->channel3BandTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
			_ui->channel3BandTypeComboBox->blockSignals(false);
		}

		if (column >= Configuration::Column::ChannelShowRangeStart&& column < Configuration::Column::ChannelShowRangeEnd) {
			const auto channelIndex = index.column() - Configuration::Column::ChannelShowRangeStart;

			auto showRange = _showRange[channelIndex];

			showRange->blockSignals(true);
			showRange->setEnabled(index.flags() & Qt::ItemIsEnabled);
			showRange->setChecked(index.data(Qt::EditRole).toBool());
			showRange->setToolTip(index.data(Qt::ToolTipRole).toString());
			showRange->blockSignals(false);
		}

		if (column >= Configuration::Column::ChannelLockedStart && column < Configuration::Column::ChannelLockedEnd) {
			const auto channelIndex = index.column() - Configuration::Column::ChannelLockedStart;

			auto locked = _locked[channelIndex];

			locked->blockSignals(true);
			locked->setEnabled(index.flags() & Qt::ItemIsEnabled);
			locked->setChecked(index.data(Qt::EditRole).toBool());
			locked->setText(hdps::Application::getIconFont("FontAwesome").getIconCharacter(index.data(Qt::EditRole).toBool() ? "lock" : "unlock"));
			locked->setToolTip(index.data(Qt::ToolTipRole).toString());
			locked->blockSignals(false);
		}
	}
}