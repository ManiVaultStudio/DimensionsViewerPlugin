#include "GlobalSettingsWidget.h"
#include "DimensionsViewerPlugin.h"
#include "Configuration.h"

#include "ui_GlobalSettingsWidget.h"

#include <QDebug>
#include <QStringListModel>

GlobalSettingsWidget::GlobalSettingsWidget(QWidget* parent) :
    ModelItemWidget(parent),
	_ui{ std::make_unique<Ui::GlobalSettingsWidget>() }
{
	_ui->setupUi(this);

    QObject::connect(_ui->groupBox, &QGroupBox::toggled, [this](bool state) {
        setData(static_cast<int>(GlobalSettings::Column::Enabled), state);
    });

    QObject::connect(_ui->profileTypeComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        setData(static_cast<int>(GlobalSettings::Column::ProfileType), currentText);
    });

    QObject::connect(_ui->rangeTypeComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        setData(static_cast<int>(GlobalSettings::Column::RangeType), currentText);
    });

    reset();
}

void GlobalSettingsWidget::updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles /*= QVector<int>()*/)
{
	const auto selectedRows = dimensionsViewerPlugin->getConfigurationsModel().getSelectionModel().selectedRows();

    if (begin == QModelIndex() && end == QModelIndex()) {
        _ui->groupBox->blockSignals(true);
        _ui->groupBox->setEnabled(false);
        _ui->groupBox->setChecked(false);
        _ui->groupBox->blockSignals(false);
		return;
	}

	for (int column = begin.column(); column <= end.column(); column++) {
		const auto index = begin.siblingAtColumn(column);

        if (column == GlobalSettings::Column::Enabled) {
            _ui->groupBox->blockSignals(true);
            _ui->groupBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->groupBox->setChecked(index.data(Qt::EditRole).toBool());
            _ui->groupBox->setToolTip(index.data(Qt::ToolTipRole).toString());
            _ui->groupBox->blockSignals(false);
        }

        if (column == GlobalSettings::Column::ProfileTypes) {
            _ui->profileTypeComboBox->blockSignals(true);
            _ui->profileTypeComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
            _ui->profileTypeComboBox->blockSignals(false);
        }

        if (column == GlobalSettings::Column::ProfileType) {
            _ui->profileTypeComboBox->blockSignals(true);
            _ui->profileTypeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->profileTypeComboBox->setCurrentText(index.data(Qt::DisplayRole).toString());
            _ui->profileTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
            _ui->profileTypeComboBox->blockSignals(false);
        }

        if (column == GlobalSettings::Column::RangeTypes) {
            _ui->rangeTypeComboBox->blockSignals(true);
            _ui->rangeTypeComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
            _ui->rangeTypeComboBox->blockSignals(false);
        }

        if (column == GlobalSettings::Column::RangeType) {
            _ui->rangeTypeComboBox->blockSignals(true);
            _ui->rangeTypeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->rangeTypeComboBox->setCurrentText(index.data(Qt::DisplayRole).toString());
            _ui->rangeTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
            _ui->rangeTypeComboBox->blockSignals(false);
        }
	}
}

void GlobalSettingsWidget::setModelIndex(const QPersistentModelIndex& modelIndex)
{
    ModelItemWidget::setModelIndex(modelIndex);
}