#include "SettingsWidget.h"
#include "ChannelWidget.h"
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
	_rangeTypeComboBoxes(),
    _settingsPushButtons(),
    _differentialProfileWidgets()
{
	_ui->setupUi(this);

    for (int channelIndex = 0; channelIndex < 3; channelIndex++)
        _ui->channelsLayout->addWidget(new ChannelWidget(_dimensionsViewerPlugin, channelIndex));

    auto& configurationsModel = _dimensionsViewerPlugin->getConfigurationsModel();

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

    QObject::connect(_ui->globalProfileTypeComboBox, qOverload<int>(&QComboBox::currentIndexChanged), [this, &configurationsModel](int currentIndex) {
        configurationsModel.setData(Configuration::Column::GlobalProfileType, currentIndex);
    });

    QObject::connect(_ui->globalRangeTypeComboBox, qOverload<int>(&QComboBox::currentIndexChanged), [this, &configurationsModel](int currentIndex) {
        configurationsModel.setData(Configuration::Column::GlobalRangeType, currentIndex);
    });

    _ui->globalProfileTypeComboBox->setModel(new QStringListModel(Channel::getProfileTypeNames()));
    _ui->globalRangeTypeComboBox->setModel(new QStringListModel(Channel::getRangeTypeNames()));

	updateData(QModelIndex(), QModelIndex());
}

void SettingsWidget::updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles /*= QVector<int>()*/)
{
	const auto selectedRows = _dimensionsViewerPlugin->getConfigurationsModel().getSelectionModel().selectedRows();

	if (selectedRows.isEmpty()) {
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

	for (int column = begin.column(); column <= end.column(); column++) {
		const auto index = begin.siblingAtColumn(column);

		if (column == Configuration::Column::GlobalSettings) {
			_ui->globalSettingsGroupBox->blockSignals(true);
			_ui->globalSettingsGroupBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->globalSettingsGroupBox->setChecked(index.data(Qt::EditRole).toBool());
			_ui->globalSettingsGroupBox->setToolTip(index.data(Qt::ToolTipRole).toString());
			_ui->globalSettingsGroupBox->blockSignals(false);
		}

        if (column == Configuration::Column::GlobalProfileType) {
            _ui->globalProfileTypeComboBox->blockSignals(true);
            _ui->globalProfileTypeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->globalProfileTypeComboBox->setCurrentIndex(index.data(Qt::EditRole).toInt());
            _ui->globalProfileTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
            _ui->globalProfileTypeComboBox->blockSignals(false);
        }

        if (column == Configuration::Column::GlobalRangeType) {
            _ui->globalRangeTypeComboBox->blockSignals(true);
            _ui->globalRangeTypeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->globalRangeTypeComboBox->setCurrentIndex(index.data(Qt::EditRole).toInt());
            _ui->globalRangeTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
            _ui->globalRangeTypeComboBox->blockSignals(false);
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