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
	_ui{ std::make_unique<Ui::SettingsWidget>() },
	_enabledCheckBoxes(),
	_datasetNameComboBoxes(),
	_colorPushButtons(),
	_opacitySliders(),
	_profileTypeComboBoxes(),
	_bandTypeComboBoxes(),
	_showRangeCheckBoxes(),
	_lockedPushButtons()
{
	_ui->setupUi(this);

	_enabledCheckBoxes << _ui->channel1EnabledCheckBox<< _ui->channel2EnabledCheckBox << _ui->channel3EnabledCheckBox;
	_datasetNameComboBoxes << _ui->channel1DatasetNameComboBox<< _ui->channel2DatasetNameComboBox << _ui->channel3DatasetNameComboBox;
	_colorPushButtons << _ui->channel1ColorPushButton<< _ui->channel2ColorPushButton << _ui->channel3ColorPushButton;
	_opacitySliders << _ui->channel1OpacitySlider<< _ui->channel2OpacitySlider << _ui->channel3OpacitySlider;
	_profileTypeComboBoxes << _ui->channel1ProfileTypeComboBox<< _ui->channel2ProfileTypeComboBox << _ui->channel3ProfileTypeComboBox;
	_bandTypeComboBoxes << _ui->channel1BandTypeComboBox<< _ui->channel2BandTypeComboBox << _ui->channel3BandTypeComboBox;
	_lockedPushButtons << _ui->channel1LockedPushButton << _ui->channel2LockedPushButton << _ui->channel3LockedPushButton;
	_showRangeCheckBoxes << _ui->channel1ShowRangeCheckBox<< _ui->channel2ShowRangeCheckBox << _ui->channel3ShowRangeCheckBox;

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

	for (auto enabledCheckBox : _enabledCheckBoxes) {
		QObject::connect(enabledCheckBox, &QCheckBox::stateChanged, [this, &configurationsModel, enabledCheckBox](int state) {
			configurationsModel.setData(Configuration::Column::ChannelEnabledStart + _enabledCheckBoxes.indexOf(enabledCheckBox), state == Qt::Checked);
		});
	}

	for (auto datasetNameComboBox : _datasetNameComboBoxes) {
		QObject::connect(datasetNameComboBox, qOverload<int>(&QComboBox::currentIndexChanged), [this, &configurationsModel, datasetNameComboBox](int currentIndex) {
			const auto channelIndex = _datasetNameComboBoxes.indexOf(datasetNameComboBox);

			if (channelIndex == 0) {
				configurationsModel.selectRow(currentIndex);
			}
			else {
				configurationsModel.setData(Configuration::Column::ChannelDatasetNameStart + channelIndex, datasetNameComboBox->currentText());
			}
		});
	}

	for (auto colorPushButton : _colorPushButtons) {
		colorPushButton->setShowText(false);
		colorPushButton->setColor(Qt::gray);

		QObject::connect(colorPushButton, &ColorPickerPushButton::colorChanged, [this, &configurationsModel, colorPushButton](const QColor& color) {
			configurationsModel.setData(Configuration::Column::ChannelColorStart + _colorPushButtons.indexOf(colorPushButton), color);
		});
	}

	for (auto opacitySlider : _opacitySliders) {
		opacitySlider->setVisible(false);

		QObject::connect(opacitySlider, &QSlider::valueChanged, [this, &configurationsModel, opacitySlider](int value) {
			configurationsModel.setData(Configuration::Column::ChannelOpacityStart + _opacitySliders.indexOf(opacitySlider), value / 100.0f);
		});
	}

	for (auto profileTypeComboBox : _profileTypeComboBoxes) {
		profileTypeComboBox->setModel(new QStringListModel(Channel::getProfileTypeNames()));

		QObject::connect(profileTypeComboBox, qOverload<int>(&QComboBox::currentIndexChanged), [this, &configurationsModel, profileTypeComboBox](int currentIndex) {
			configurationsModel.setData(Configuration::Column::ChannelProfileTypeStart + _profileTypeComboBoxes.indexOf(profileTypeComboBox), currentIndex);
		});
	}

	for (auto bandTypeComboBox : _bandTypeComboBoxes) {
		bandTypeComboBox->setModel(new QStringListModel(Channel::getBandTypeNames()));
		//bandTypeComboBox->setVisible(false);

		QObject::connect(bandTypeComboBox, qOverload<int>(&QComboBox::currentIndexChanged), [this, &configurationsModel, bandTypeComboBox](int currentIndex) {
			configurationsModel.setData(Configuration::Column::ChannelBandTypeStart + _bandTypeComboBoxes.indexOf(bandTypeComboBox), currentIndex);
		});
	}

	for (auto showRangeCheckBox : _showRangeCheckBoxes) {
		QObject::connect(showRangeCheckBox, &QCheckBox::stateChanged, [this, &configurationsModel, showRangeCheckBox](int state) {
			configurationsModel.setData(Configuration::Column::ChannelShowRangeStart + _showRangeCheckBoxes.indexOf(showRangeCheckBox), state == Qt::Checked);
		});
	}

	for (auto lockedPushButton : _lockedPushButtons) {
		QFont font = QFont("Font Awesome 5 Free Solid", 8);

		lockedPushButton->setFont(font);
		lockedPushButton->setStyleSheet("text-align: center");
		lockedPushButton->setText(hdps::Application::getIconFont("FontAwesome").getIconCharacter("lock"));
		lockedPushButton->setVisible(_lockedPushButtons.indexOf(lockedPushButton) > 0);

		QObject::connect(lockedPushButton, &QPushButton::toggled, [this, &configurationsModel, lockedPushButton](bool checked) {
			configurationsModel.setData(Configuration::Column::ChannelLockedStart + _lockedPushButtons.indexOf(lockedPushButton), checked);
		});
	}

	updateData(QModelIndex(), QModelIndex());
}

void SettingsWidget::updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles /*= QVector<int>()*/)
{
	const auto selectedRows = _dimensionsViewerPlugin->getConfigurationsModel().getSelectionModel().selectedRows();

	if (selectedRows.isEmpty()) {
		_ui->settingsGroupBox->setEnabled(false);

		for (auto enabledCheckBox : _enabledCheckBoxes) {
			enabledCheckBox->setEnabled(false);
			enabledCheckBox->setChecked(false);
		}

		for (auto datasetNameComboBox : _datasetNameComboBoxes) {
			datasetNameComboBox->setEnabled(false);
			datasetNameComboBox->setCurrentIndex(-1);
		}

		for (auto colorPushButton : _colorPushButtons) {
			colorPushButton->setEnabled(false);
			colorPushButton->setColor(Qt::gray);
		}

		for (auto opacitySlider : _opacitySliders) {
			opacitySlider->setEnabled(false);
			opacitySlider->setValue(0);
		}

		for (auto profileTypeComboBox : _profileTypeComboBoxes) {
			profileTypeComboBox->setEnabled(false);
			profileTypeComboBox->setCurrentIndex(-1);
		}

		for (auto bandTypeComboBox : _bandTypeComboBoxes) {
			bandTypeComboBox->setEnabled(false);
			bandTypeComboBox->setCurrentIndex(-1);
		}
		
		for (auto showRangeCheckBox : _showRangeCheckBoxes) {
			showRangeCheckBox->setEnabled(false);
			showRangeCheckBox->setChecked(false);
		}

		for (auto lockedPushButton : _lockedPushButtons) {
			lockedPushButton->setEnabled(false);
			lockedPushButton->setChecked(false);
		}

		return;
	}

	if (begin.row() != selectedRows.first().row())
		return;

	_ui->settingsGroupBox->setEnabled(true);

	for (int column = begin.column(); column <= end.column(); column++) {
		const auto index = begin.siblingAtColumn(column);

		if (column == static_cast<int>(Configuration::Column::Subsets)) {
			const auto subsets = index.data(Qt::EditRole).toStringList();

			_ui->channel2DatasetNameComboBox->blockSignals(true);
			_ui->channel2DatasetNameComboBox->setModel(new QStringListModel(subsets));
			_ui->channel2DatasetNameComboBox->blockSignals(false);

			_ui->channel3DatasetNameComboBox->blockSignals(true);
			_ui->channel3DatasetNameComboBox->setModel(new QStringListModel(subsets));
			_ui->channel3DatasetNameComboBox->blockSignals(false);
		}

		if (column >= Configuration::Column::ChannelEnabledStart && column < Configuration::Column::ChannelEnabledEnd) {
			const auto channelIndex = index.column() - Configuration::Column::ChannelEnabledStart;

			auto enabledCheckBox = _enabledCheckBoxes[channelIndex];

			enabledCheckBox->blockSignals(true);
			enabledCheckBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
			enabledCheckBox->setChecked(index.data(Qt::EditRole).toBool());
			enabledCheckBox->setToolTip(index.data(Qt::ToolTipRole).toString());
			enabledCheckBox->blockSignals(false);
		}

		if (column >= Configuration::Column::ChannelDatasetNameStart && column < Configuration::Column::ChannelDatasetNameEnd) {
			const auto channelIndex = index.column() - Configuration::Column::ChannelDatasetNameStart;

			auto datasetNameComboBox = _datasetNameComboBoxes[channelIndex];

			datasetNameComboBox->blockSignals(true);
			datasetNameComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
			datasetNameComboBox->setCurrentText(index.data(Qt::EditRole).toString());
			datasetNameComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
			datasetNameComboBox->blockSignals(false);
		}

		if (column >= Configuration::Column::ChannelColorStart && column < Configuration::Column::ChannelColorEnd) {
			const auto channelIndex = index.column() - Configuration::Column::ChannelColorStart;

			auto colorPushButton = _colorPushButtons[channelIndex];

			colorPushButton->blockSignals(true);
			colorPushButton->setEnabled(index.flags() & Qt::ItemIsEnabled);
			colorPushButton->setColor(index.data(Qt::EditRole).value<QColor>());
			colorPushButton->setToolTip(index.data(Qt::ToolTipRole).toString());
			colorPushButton->blockSignals(false);
		}

		if (column >= Configuration::Column::ChannelOpacityStart && column < Configuration::Column::ChannelOpacityEnd) {
			const auto channelIndex = index.column() - Configuration::Column::ChannelOpacityStart;

			auto opacitySlider = _opacitySliders[channelIndex];

			opacitySlider->blockSignals(true);
			opacitySlider->setEnabled(index.flags() & Qt::ItemIsEnabled);
			opacitySlider->setValue(static_cast<int>(100.0f * index.data(Qt::EditRole).toFloat()));
			opacitySlider->setToolTip(index.data(Qt::ToolTipRole).toString());
			opacitySlider->blockSignals(false);
		}

		if (column >= Configuration::Column::ChannelProfileTypeStart && column < Configuration::Column::ChannelProfileTypeEnd) {
			const auto channelIndex = index.column() - Configuration::Column::ChannelProfileTypeStart;

			auto profileTypeComboBox = _profileTypeComboBoxes[channelIndex];

			profileTypeComboBox->blockSignals(true);
			profileTypeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
			profileTypeComboBox->setCurrentIndex(index.data(Qt::EditRole).toInt());
			profileTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
			profileTypeComboBox->blockSignals(false);
		}

		if (column >= Configuration::Column::ChannelBandTypeStart && column < Configuration::Column::ChannelBandTypeEnd) {
			const auto channelIndex = index.column() - Configuration::Column::ChannelBandTypeStart;

			auto bandTypeComboBox = _bandTypeComboBoxes[channelIndex];

			bandTypeComboBox->blockSignals(true);
			bandTypeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
			bandTypeComboBox->setCurrentIndex(index.data(Qt::EditRole).toInt());
			bandTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
			bandTypeComboBox->blockSignals(false);
		}

		if (column >= Configuration::Column::ChannelShowRangeStart && column < Configuration::Column::ChannelShowRangeEnd) {
			const auto channelIndex = index.column() - Configuration::Column::ChannelShowRangeStart;

			auto showRangeCheckBox = _showRangeCheckBoxes[channelIndex];

			showRangeCheckBox->blockSignals(true);
			showRangeCheckBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
			showRangeCheckBox->setChecked(index.data(Qt::EditRole).toBool());
			showRangeCheckBox->setToolTip(index.data(Qt::ToolTipRole).toString());
			showRangeCheckBox->blockSignals(false);
		}

		if (column >= Configuration::Column::ChannelLockedStart && column < Configuration::Column::ChannelLockedEnd) {
			const auto channelIndex = index.column() - Configuration::Column::ChannelLockedStart;

			auto lockedPushButton = _lockedPushButtons[channelIndex];

			lockedPushButton->blockSignals(true);
			lockedPushButton->setEnabled(index.flags() & Qt::ItemIsEnabled);
			lockedPushButton->setChecked(index.data(Qt::EditRole).toBool());
			lockedPushButton->setText(hdps::Application::getIconFont("FontAwesome").getIconCharacter(index.data(Qt::EditRole).toBool() ? "lock" : "unlock"));
			lockedPushButton->setToolTip(index.data(Qt::ToolTipRole).toString());
			lockedPushButton->blockSignals(false);
		}
	}
}