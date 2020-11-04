#include "ChannelSettingsWidget.h"
#include "DimensionsViewerPlugin.h"
#include "Application.h"

#include "ui_ChannelSettingsWidget.h"

#include <QDebug>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QStringListModel>

ChannelSettingsWidget::ChannelSettingsWidget(QWidget* parent, const std::uint32_t& channelIndex) :
    ModelItemWidget(parent),
	_ui{ std::make_unique<Ui::ChannelSettingsWidget>() },
    _channelIndex(channelIndex)
{
	_ui->setupUi(this);

 //   const auto fontAwesome = QFont("Font Awesome 5 Free Solid", 8);

 //   auto& configurationsModel = dimensionsViewerPlugin->getConfigurationsModel();

 //   if (_channelIndex == 0) {
 //       const auto updateDatasetCombobox = [this]() {
 //           auto& configurationsModel = dimensionsViewerPlugin->getConfigurationsModel();

 //           _ui->datasetNameComboBox->blockSignals(true);
 //           _ui->datasetNameComboBox->setModel(new QStringListModel(configurationsModel.getConfigurationNames()));
 //           _ui->datasetNameComboBox->setEnabled(configurationsModel.rowCount() > 1);
 //           _ui->datasetNameComboBox->blockSignals(false);
 //       };

 //       QObject::connect(&configurationsModel, &ConfigurationsModel::rowsInserted, [this, updateDatasetCombobox](const QModelIndex& parent, int first, int last) {
 //           updateDatasetCombobox();
 //       });

 //       QObject::connect(&configurationsModel, &ConfigurationsModel::rowsRemoved, [this, updateDatasetCombobox](const QModelIndex& parent, int first, int last) {
 //           updateDatasetCombobox();
 //       });
 //   }

 //   QObject::connect(&configurationsModel, &ConfigurationsModel::dataChanged, this, &ChannelSettingsWidget::updateData);

 //   QObject::connect(&configurationsModel.getSelectionModel(), &QItemSelectionModel::selectionChanged, [this, &configurationsModel](const QItemSelection& selected, const QItemSelection& deselected) {
 //       const auto selectedRows = configurationsModel.getSelectionModel().selectedRows();

 //       if (selectedRows.isEmpty())
 //           updateData(QModelIndex(), QModelIndex());
 //       else {
 //           const auto first = selectedRows.first();

 //           updateData(first.siblingAtColumn(static_cast<int>(Configuration::Column::Start)), first.siblingAtColumn(static_cast<int>(Configuration::Column::End)));
 //       }
 //   });

 //   QObject::connect(_ui->enabledCheckBox, &QCheckBox::stateChanged, [this, &configurationsModel](int state) {
 //       configurationsModel.setData(Configuration::Column::ChannelEnabledStart + _channelIndex, state == Qt::Checked);
 //   });

 //   QObject::connect(_ui->datasetNameComboBox, qOverload<int>(&QComboBox::currentIndexChanged), [this, &configurationsModel](int currentIndex) {
 //       if (_channelIndex == 0)
 //           configurationsModel.selectRow(currentIndex);
 //       else
 //           configurationsModel.setData(Configuration::Column::ChannelDatasetNameStart + _channelIndex, _ui->datasetNameComboBox->currentText());
 //   });

 //   _ui->colorPushButton->setShowText(false);
 //   _ui->colorPushButton->setColor(Qt::gray);

 //   QObject::connect(_ui->colorPushButton, &ColorPickerPushButton::colorChanged, [this, &configurationsModel](const QColor& color) {
 //       configurationsModel.setData(Configuration::Column::ChannelColorStart + _channelIndex, color);
 //   });

 //   _ui->settingsPushButton->setFont(fontAwesome);
 //   _ui->settingsPushButton->setStyleSheet("text-align: center");
 //   _ui->settingsPushButton->setText(hdps::Application::getIconFont("FontAwesome").getIconCharacter("cog"));

 //   //_ui->profileTypeComboBox->setModel(new QStringListModel(Profile::getProfileTypeNames()));

 //   QObject::connect(_ui->profileTypeComboBox, qOverload<int>(&QComboBox::currentIndexChanged), [this, &configurationsModel](int currentIndex) {
 //       configurationsModel.setData(Configuration::Column::ChannelProfileTypeStart + _channelIndex, currentIndex);
 //   });

 //   //_ui->rangeTypeComboBox->setModel(new QStringListModel(Profile::getRangeTypeNames()));

 //   QObject::connect(_ui->rangeTypeComboBox, qOverload<int>(&QComboBox::currentIndexChanged), [this, &configurationsModel](int currentIndex) {
 //       configurationsModel.setData(Configuration::Column::ChannelRangeTypeStart + _channelIndex, currentIndex);
 //   });

	//updateData(QModelIndex(), QModelIndex());
}

void ChannelSettingsWidget::updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles /*= QVector<int>()*/)
{
	/*const auto selectedRows = dimensionsViewerPlugin->getConfigurationsModel().getSelectionModel().selectedRows();

	if (selectedRows.isEmpty()) {
        _ui->enabledCheckBox->setEnabled(false);
        _ui->enabledCheckBox->setChecked(false);
        _ui->enabledCheckBox->setText(QString("Channel %1").arg(QString::number(_channelIndex + 1)));
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

		if (_channelIndex > 0 && column == static_cast<int>(Configuration::Column::Subsets)) {
			const auto subsets = index.data(Qt::EditRole).toStringList();

            _ui->datasetNameComboBox->blockSignals(true);
            _ui->datasetNameComboBox->setModel(new QStringListModel(subsets));
            _ui->datasetNameComboBox->blockSignals(false);
		}
        
		if (column >= Configuration::Column::ChannelEnabledStart && column < Configuration::Column::ChannelEnabledEnd) {
			const auto channelIndex = index.column() - Configuration::Column::ChannelEnabledStart;

            if (channelIndex == _channelIndex) {
                _ui->enabledCheckBox->blockSignals(true);
                _ui->enabledCheckBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
                _ui->enabledCheckBox->setChecked(index.data(Qt::EditRole).toBool());
                _ui->enabledCheckBox->setToolTip(index.data(Qt::ToolTipRole).toString());
                _ui->enabledCheckBox->blockSignals(false);
            }
		}

        if (column >= Configuration::Column::ChannelNameStart && column < Configuration::Column::ChannelNameEnd) {
            const auto channelIndex = index.column() - Configuration::Column::ChannelNameStart;

            if (channelIndex == _channelIndex) {
                _ui->enabledCheckBox->blockSignals(true);
                _ui->enabledCheckBox->setText(index.data(Qt::EditRole).toString());
                _ui->enabledCheckBox->blockSignals(false);
            }
        }
        
		if (column >= Configuration::Column::ChannelDatasetNameStart && column < Configuration::Column::ChannelDatasetNameEnd) {
			const auto channelIndex = index.column() - Configuration::Column::ChannelDatasetNameStart;

            if (channelIndex == _channelIndex) {
                _ui->datasetNameComboBox->blockSignals(true);

                if (_channelIndex > 0)
                    _ui->datasetNameComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);

                _ui->datasetNameComboBox->setCurrentText(index.data(Qt::EditRole).toString());
                _ui->datasetNameComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
                _ui->datasetNameComboBox->blockSignals(false);
            }
		}
        
		if (column >= Configuration::Column::ChannelColorStart && column < Configuration::Column::ChannelColorEnd) {
			const auto channelIndex = index.column() - Configuration::Column::ChannelColorStart;

            if (channelIndex == _channelIndex) {
                _ui->colorPushButton->blockSignals(true);
                _ui->colorPushButton->setEnabled(index.flags() & Qt::ItemIsEnabled);
                _ui->colorPushButton->setColor(index.data(Qt::EditRole).value<QColor>());
                _ui->colorPushButton->setToolTip(index.data(Qt::ToolTipRole).toString());
                _ui->colorPushButton->blockSignals(false);
            }
		}
        
		if (column >= Configuration::Column::ChannelProfileTypeStart && column < Configuration::Column::ChannelProfileTypeEnd) {
			const auto channelIndex = index.column() - Configuration::Column::ChannelProfileTypeStart;

            if (channelIndex == _channelIndex) {
                _ui->profileTypeComboBox->blockSignals(true);
                _ui->profileTypeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
                _ui->profileTypeComboBox->setCurrentIndex(index.data(Qt::EditRole).toInt());
                _ui->profileTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
                _ui->profileTypeComboBox->blockSignals(false);
            }
		}
        
		if (column >= Configuration::Column::ChannelRangeTypeStart && column < Configuration::Column::ChannelRangeTypeEnd) {
			const auto channelIndex = index.column() - Configuration::Column::ChannelRangeTypeStart;

            if (channelIndex == _channelIndex) {
                _ui->rangeTypeComboBox->blockSignals(true);
                _ui->rangeTypeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
                _ui->rangeTypeComboBox->setCurrentIndex(index.data(Qt::EditRole).toInt());
                _ui->rangeTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
                _ui->rangeTypeComboBox->blockSignals(false);
            }
		}

        if (column >= Configuration::Column::ChannelSettingsStart && column < Configuration::Column::ChannelSettingsEnd) {
            const auto channelIndex = index.column() - Configuration::Column::ChannelSettingsStart;

            if (channelIndex == _channelIndex) {
                _ui->settingsPushButton->blockSignals(true);
                _ui->settingsPushButton->setEnabled(index.flags() & Qt::ItemIsEnabled);
                _ui->settingsPushButton->setToolTip(index.data(Qt::ToolTipRole).toString());
                _ui->settingsPushButton->blockSignals(false);
            }
        }
	}*/
}