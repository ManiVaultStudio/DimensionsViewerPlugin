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

	auto& configurationsModel = _dimensionsViewerPlugin->getConfigurationsModel();

	_ui->tableView->setModel(&configurationsModel);
	_ui->tableView->setSelectionModel(&configurationsModel.getSelectionModel());

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

	_ui->channel1ProfileTypeComboBox->setModel(new QStringListModel(Channel::getProfileTypeNames()));
	_ui->channel2ProfileTypeComboBox->setModel(new QStringListModel(Channel::getProfileTypeNames()));
	_ui->channel3ProfileTypeComboBox->setModel(new QStringListModel(Channel::getProfileTypeNames()));

	_ui->channel1BandTypeComboBox->setModel(new QStringListModel(Channel::getBandTypeNames()));
	_ui->channel2BandTypeComboBox->setModel(new QStringListModel(Channel::getBandTypeNames()));
	_ui->channel3BandTypeComboBox->setModel(new QStringListModel(Channel::getBandTypeNames()));

	QFont font = QFont("Font Awesome 5 Free Solid", 8);

	_ui->globalSettingsPushButton->setFont(font);
	_ui->globalSettingsPushButton->setStyleSheet("text-align: center");
	_ui->globalSettingsPushButton->setText(hdps::Application::getIconFont("FontAwesome").getIconCharacter("lock"));

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

	QObject::connect(_ui->channel1ShowRangeCheckBox, &QCheckBox::stateChanged, [this, &configurationsModel](int state) {
		configurationsModel.setData(Configuration::Column::Channel1ShowRange, state == Qt::Checked);
	});

	QObject::connect(_ui->channel2ShowRangeCheckBox, &QCheckBox::stateChanged, [this, &configurationsModel](int state) {
		configurationsModel.setData(Configuration::Column::Channel2ShowRange, state == Qt::Checked);
	});

	QObject::connect(_ui->channel3ShowRangeCheckBox, &QCheckBox::stateChanged, [this, &configurationsModel](int state) {
		configurationsModel.setData(Configuration::Column::Channel3ShowRange, state == Qt::Checked);
	});

	QObject::connect(_ui->globalSettingsPushButton, &QPushButton::toggled, [this, &configurationsModel](bool checked) {
		configurationsModel.setData(Configuration::Column::GlobalSettings, checked);
	});

	updateData(QModelIndex(), QModelIndex());
}

SettingsWidget::~SettingsWidget() {}

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
		_ui->globalSettingsPushButton->setChecked(false);

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

		if (column == static_cast<int>(Configuration::Column::Channel1ShowRange)) {
			_ui->channel1ShowRangeCheckBox->blockSignals(true);
			_ui->channel1ShowRangeCheckBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->channel1ShowRangeCheckBox->setChecked(index.data(Qt::EditRole).toBool());
			_ui->channel1ShowRangeCheckBox->setToolTip(index.data(Qt::ToolTipRole).toString());
			_ui->channel1ShowRangeCheckBox->blockSignals(false);
		}

		if (column == static_cast<int>(Configuration::Column::Channel2ShowRange)) {
			_ui->channel2ShowRangeCheckBox->blockSignals(true);
			_ui->channel2ShowRangeCheckBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->channel2ShowRangeCheckBox->setChecked(index.data(Qt::EditRole).toBool());
			_ui->channel2ShowRangeCheckBox->setToolTip(index.data(Qt::ToolTipRole).toString());
			_ui->channel2ShowRangeCheckBox->blockSignals(false);
		}

		if (column == static_cast<int>(Configuration::Column::Channel3ShowRange)) {
			_ui->channel3ShowRangeCheckBox->blockSignals(true);
			_ui->channel3ShowRangeCheckBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->channel3ShowRangeCheckBox->setChecked(index.data(Qt::EditRole).toBool());
			_ui->channel3ShowRangeCheckBox->setToolTip(index.data(Qt::ToolTipRole).toString());
			_ui->channel3ShowRangeCheckBox->blockSignals(false);
		}

		if (column == static_cast<int>(Configuration::Column::GlobalSettings)) {
			const auto globalSettings = index.data(Qt::EditRole).toBool();

			_ui->globalSettingsPushButton->blockSignals(true);
			_ui->globalSettingsPushButton->setEnabled(index.flags() & Qt::ItemIsEnabled);
			_ui->globalSettingsPushButton->setChecked(globalSettings);
			_ui->globalSettingsPushButton->setText(hdps::Application::getIconFont("FontAwesome").getIconCharacter(globalSettings ? "lock" : "unlock"));
			_ui->globalSettingsPushButton->setToolTip(index.data(Qt::ToolTipRole).toString());
			_ui->globalSettingsPushButton->blockSignals(false);
		}
	}
}