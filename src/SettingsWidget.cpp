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
	_profileTypeComboBoxes(),
	_bandTypeComboBoxes(),
    _settingsPushButtons(),
    _differentialProfileWidgets()
{
	_ui->setupUi(this);

	_enabledCheckBoxes << _ui->channel1EnabledCheckBox<< _ui->channel2EnabledCheckBox << _ui->channel3EnabledCheckBox;
	_datasetNameComboBoxes << _ui->channel1DatasetNameComboBox<< _ui->channel2DatasetNameComboBox << _ui->channel3DatasetNameComboBox;
	_colorPushButtons << _ui->channel1ColorPushButton<< _ui->channel2ColorPushButton << _ui->channel3ColorPushButton;
	_profileTypeComboBoxes << _ui->channel1ProfileTypeComboBox<< _ui->channel2ProfileTypeComboBox << _ui->channel3ProfileTypeComboBox;
	_bandTypeComboBoxes << _ui->channel1BandTypeComboBox << _ui->channel2BandTypeComboBox << _ui->channel3BandTypeComboBox;
    _settingsPushButtons << _ui->channel1SettingsPushButton << _ui->channel2SettingsPushButton << _ui->channel3SettingsPushButton;
    _differentialProfileWidgets << _ui->differentialProfileCompareLabel << _ui->profile1DatasetNameComboBox << _ui->differentialProfileWithLabel << _ui->profile2DatasetNameComboBox << _ui->differentialProfileColorPushButton;

	auto& configurationsModel = _dimensionsViewerPlugin->getConfigurationsModel();

	const auto updateDataset1Combobox = [this]() {
		auto& configurationsModel = _dimensionsViewerPlugin->getConfigurationsModel();

		_ui->channel1DatasetNameComboBox->blockSignals(true);
		_ui->channel1DatasetNameComboBox->setModel(new QStringListModel(configurationsModel.getConfigurationNames()));
		_ui->channel1DatasetNameComboBox->setEnabled(configurationsModel.rowCount() > 1);
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

    for (auto settingsPushButton : _settingsPushButtons) {
        QFont font = QFont("Font Awesome 5 Free Solid", 8);

        settingsPushButton->setFont(font);
        settingsPushButton->setStyleSheet("text-align: center");
        settingsPushButton->setText(hdps::Application::getIconFont("FontAwesome").getIconCharacter("cog"));
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

    QObject::connect(_ui->showDimensionNamesCheckBox, &QCheckBox::stateChanged, [this, &configurationsModel](int state) {
        configurationsModel.setData(Configuration::Column::ShowDimensionNames, state == Qt::Checked);
    });

    QObject::connect(_ui->differentialProfileGroupBox, &QGroupBox::toggled, [this, &configurationsModel](bool checked) {
        configurationsModel.setData(Configuration::Column::ShowDifferentialProfile, checked);
    });

    QObject::connect(_ui->profile1DatasetNameComboBox, &QComboBox::currentTextChanged, [this, &configurationsModel](QString currentText) {
        configurationsModel.setData(Configuration::Column::Profile1DatasetName, currentText);
    });

    QObject::connect(_ui->profile2DatasetNameComboBox, &QComboBox::currentTextChanged, [this, &configurationsModel](QString currentText) {
        configurationsModel.setData(Configuration::Column::Profile2DatasetName, currentText);
    });

    QObject::connect(_ui->globalSettingsGroupBox, &QGroupBox::toggled, [this, &configurationsModel](bool state) {
        configurationsModel.setData(Configuration::Column::GlobalSettings, state);
    });

	updateData(QModelIndex(), QModelIndex());
}

void SettingsWidget::updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles /*= QVector<int>()*/)
{
	const auto selectedRows = _dimensionsViewerPlugin->getConfigurationsModel().getSelectionModel().selectedRows();

	if (selectedRows.isEmpty()) {
		_ui->channelsGroupBox->setEnabled(false);

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

		for (auto profileTypeComboBox : _profileTypeComboBoxes) {
			profileTypeComboBox->setEnabled(false);
			profileTypeComboBox->setCurrentIndex(-1);
		}

		for (auto bandTypeComboBox : _bandTypeComboBoxes) {
			bandTypeComboBox->setEnabled(false);
			bandTypeComboBox->setCurrentIndex(-1);
		}
		
        for (auto settingsPushButton : _settingsPushButtons) {
            settingsPushButton->setEnabled(false);
        }

        _ui->showDimensionNamesCheckBox->setEnabled(false);
        _ui->globalSettingsGroupBox->setEnabled(false);
        _ui->globalSettingsGroupBox->setChecked(false);
        _ui->differentialProfileGroupBox->setEnabled(false);
        _ui->differentialProfileGroupBox->setChecked(false);

        for (auto differentialProfileWidget : _differentialProfileWidgets) {
            differentialProfileWidget->setEnabled(false);
        }

		return;
	}

	if (begin.row() != selectedRows.first().row())
		return;

	_ui->channelsGroupBox->setEnabled(true);

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

            _settingsPushButtons[channelIndex]->setEnabled(index.data(Qt::EditRole).toBool());
		}

		if (column >= Configuration::Column::ChannelDatasetNameStart && column < Configuration::Column::ChannelDatasetNameEnd) {
			const auto channelIndex = index.column() - Configuration::Column::ChannelDatasetNameStart;

			auto datasetNameComboBox = _datasetNameComboBoxes[channelIndex];

			datasetNameComboBox->blockSignals(true);

            if (channelIndex > 0)
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

		if (column == Configuration::Column::GlobalSettings) {
			_ui->globalSettingsGroupBox->blockSignals(true);
			_ui->globalSettingsGroupBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->globalSettingsGroupBox->setChecked(index.data(Qt::EditRole).toBool());
			_ui->globalSettingsGroupBox->setToolTip(index.data(Qt::ToolTipRole).toString());
			_ui->globalSettingsGroupBox->blockSignals(false);
		}

        if (column == Configuration::Column::ShowDimensionNames) {
            _ui->showDimensionNamesCheckBox->blockSignals(true);
            _ui->showDimensionNamesCheckBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->showDimensionNamesCheckBox->setChecked(index.data(Qt::EditRole).toBool());
            _ui->showDimensionNamesCheckBox->setToolTip(index.data(Qt::ToolTipRole).toString());
            _ui->showDimensionNamesCheckBox->blockSignals(false);
        }

        if (column == Configuration::Column::ShowDifferentialProfile) {
            _ui->differentialProfileGroupBox->blockSignals(true);
            _ui->differentialProfileGroupBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->differentialProfileGroupBox->setChecked(index.data(Qt::EditRole).toBool());
            _ui->differentialProfileGroupBox->setToolTip(index.data(Qt::ToolTipRole).toString());
            _ui->differentialProfileGroupBox->blockSignals(false);
            
            _ui->differentialProfileCompareLabel->setEnabled(index.data(Qt::EditRole).toBool());
            _ui->differentialProfileWithLabel->setEnabled(index.data(Qt::EditRole).toBool());
            _ui->differentialProfileColorPushButton->setEnabled(index.data(Qt::EditRole).toBool());
        }

        if (column == Configuration::Column::Profile1DatasetNames) {
            _ui->profile1DatasetNameComboBox->blockSignals(true);
            _ui->profile1DatasetNameComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
            _ui->profile1DatasetNameComboBox->blockSignals(false);
        }

        if (column == Configuration::Column::Profile2DatasetNames) {
            _ui->profile2DatasetNameComboBox->blockSignals(true);
            _ui->profile2DatasetNameComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
            _ui->profile2DatasetNameComboBox->blockSignals(false);
        }

        if (column == Configuration::Column::Profile1DatasetName) {
            _ui->profile1DatasetNameComboBox->blockSignals(true);
            _ui->profile1DatasetNameComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->profile1DatasetNameComboBox->setCurrentText(index.data(Qt::EditRole).toString());
            _ui->profile1DatasetNameComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
            _ui->profile1DatasetNameComboBox->blockSignals(false);
        }

        if (column == Configuration::Column::Profile2DatasetName) {
            _ui->profile2DatasetNameComboBox->blockSignals(true);
            _ui->profile2DatasetNameComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->profile2DatasetNameComboBox->setCurrentText(index.data(Qt::EditRole).toString());
            _ui->profile2DatasetNameComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
            _ui->profile2DatasetNameComboBox->blockSignals(false);
        }
	}
}