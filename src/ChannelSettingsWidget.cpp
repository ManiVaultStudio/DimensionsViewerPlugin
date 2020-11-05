#include "ChannelSettingsWidget.h"
#include "DimensionsViewerPlugin.h"
#include "Application.h"

#include "ui_ChannelSettingsWidget.h"

#include <QDebug>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QStringListModel>

ChannelSettingsWidget::ChannelSettingsWidget(QWidget* parent) :
    ModelItemWidget(parent),
	_ui{ std::make_unique<Ui::ChannelSettingsWidget>() }
{
	_ui->setupUi(this);

    const auto fontAwesome = QFont("Font Awesome 5 Free Solid", 8);

    QObject::connect(_ui->enabledCheckBox, &QCheckBox::stateChanged, [this](int state) {
        setData(Channel::Column::Enabled, state);
    });

    QObject::connect(_ui->datasetNameComboBox, qOverload<int>(&QComboBox::currentIndexChanged), [this](int currentIndex) {
        if (_persistentModelIndex.row() == 0)
            getConfigurationsModel().selectRow(currentIndex);
        else
            setData(Channel::Column::DatasetName, _ui->datasetNameComboBox->currentText());
    });

    _ui->colorPushButton->setShowText(false);
    _ui->colorPushButton->setColor(Qt::gray);

    QObject::connect(_ui->colorPushButton, &ColorPickerPushButton::colorChanged, [this](const QColor& color) {
        setData(Channel::Column::Color, color);
    });

    _ui->settingsPushButton->setFont(fontAwesome);
    _ui->settingsPushButton->setStyleSheet("text-align: center");
    _ui->settingsPushButton->setText(hdps::Application::getIconFont("FontAwesome").getIconCharacter("cog"));

    QObject::connect(_ui->profileTypeComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        setData(Channel::Column::ProfileType, currentText);
    });

    QObject::connect(_ui->rangeTypeComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        setData(Channel::Column::RangeType, currentText);
    });

    reset();
}

void ChannelSettingsWidget::updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles /*= QVector<int>()*/)
{
	const auto selectedRows = dimensionsViewerPlugin->getConfigurationsModel().getSelectionModel().selectedRows();

	if (begin == QModelIndex() && end == QModelIndex()) {
        _ui->enabledCheckBox->setEnabled(false);
        _ui->enabledCheckBox->setChecked(false);
        _ui->datasetNameComboBox->setEnabled(false);
        _ui->datasetNameComboBox->setCurrentIndex(-1);
        _ui->colorPushButton->setEnabled(false);
        _ui->colorPushButton->setColor(Qt::gray);
        _ui->profileTypeComboBox->setEnabled(false);
        _ui->profileTypeComboBox->setCurrentIndex(-1);
        _ui->rangeTypeComboBox->setEnabled(false);
        _ui->rangeTypeComboBox->setCurrentIndex(-1);
        _ui->settingsPushButton->setEnabled(false);

		return;
	}

	if (begin.row() != selectedRows.first().row())
		return;

	for (int column = begin.column(); column <= end.column(); column++) {
		const auto index = begin.siblingAtColumn(column);

		if (column == Channel::Column::Enabled) {
            _ui->enabledCheckBox->blockSignals(true);
            _ui->enabledCheckBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->enabledCheckBox->setChecked(index.data(Qt::EditRole).toBool());
            _ui->enabledCheckBox->setToolTip(index.data(Qt::ToolTipRole).toString());
            _ui->enabledCheckBox->blockSignals(false);
		}

        if (column == Channel::Column::DisplayName) {
            _ui->enabledCheckBox->blockSignals(true);
            _ui->enabledCheckBox->setText(index.data(Qt::EditRole).toString());
            _ui->enabledCheckBox->blockSignals(false);
        }
        
        if (column == Channel::Column::DatasetNames) {
            _ui->datasetNameComboBox->blockSignals(true);
            _ui->datasetNameComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
            _ui->datasetNameComboBox->blockSignals(false);
        }

		if (column == Channel::Column::DatasetName) {
            _ui->datasetNameComboBox->blockSignals(true);
            _ui->datasetNameComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->datasetNameComboBox->setCurrentText(index.data(Qt::EditRole).toString());
            _ui->datasetNameComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
            _ui->datasetNameComboBox->blockSignals(false);
		}
        
		if (column == Channel::Column::Color) {
            _ui->colorPushButton->blockSignals(true);
            _ui->colorPushButton->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->colorPushButton->setColor(index.data(Qt::EditRole).value<QColor>());
            _ui->colorPushButton->setToolTip(index.data(Qt::ToolTipRole).toString());
            _ui->colorPushButton->blockSignals(false);
		}
        
        if (column == Channel::Column::ProfileTypes) {
            _ui->profileTypeComboBox->blockSignals(true);
            _ui->profileTypeComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
            _ui->profileTypeComboBox->blockSignals(false);
        }

        if (column == Channel::Column::ProfileType) {
            _ui->profileTypeComboBox->blockSignals(true);
            _ui->profileTypeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->profileTypeComboBox->setCurrentText(index.data(Qt::DisplayRole).toString());
            _ui->profileTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
            _ui->profileTypeComboBox->blockSignals(false);
        }

        if (column == Channel::Column::RangeTypes) {
            _ui->rangeTypeComboBox->blockSignals(true);
            _ui->rangeTypeComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
            _ui->rangeTypeComboBox->blockSignals(false);
        }
        
		if (column == Channel::Column::RangeType) {
            _ui->rangeTypeComboBox->blockSignals(true);
            _ui->rangeTypeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->rangeTypeComboBox->setCurrentText(index.data(Qt::DisplayRole).toString());
            _ui->rangeTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
            _ui->rangeTypeComboBox->blockSignals(false);
		}

        if (column == Channel::Column::Settings) {
            _ui->settingsPushButton->blockSignals(true);
            _ui->settingsPushButton->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->settingsPushButton->setToolTip(index.data(Qt::ToolTipRole).toString());
            _ui->settingsPushButton->blockSignals(false);
        }
	}
}

void ChannelSettingsWidget::setPersistentModelIndex(const QPersistentModelIndex& modelIndex)
{
    ModelItemWidget::setPersistentModelIndex(modelIndex);
}