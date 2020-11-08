#include "ConfigurationWidget.h"
#include "DimensionsViewerPlugin.h"
#include "Configuration.h"

#include "ui_ConfigurationWidget.h"

#include <QDebug>

ConfigurationWidget::ConfigurationWidget(QWidget* parent) :
    ModelItemWidget(parent),
	_ui{ std::make_unique<Ui::ConfigurationWidget>() }
{
	_ui->setupUi(this);

    _ui->treeView->setModel(&getConfigurationsModel());
    _ui->treeView->setSelectionModel(&getSelectionModel());

    QObject::connect(&getSelectionModel(), &QItemSelectionModel::selectionChanged, [this](const QItemSelection& selected, const QItemSelection& deselected) {
        const auto selectedRows = getSelectionModel().selectedRows();

        if (!selectedRows.isEmpty()) {
            const auto firstRowIndex = selectedRows.first();

            if (firstRowIndex.parent() == QModelIndex())
                setPersistentModelIndex(firstRowIndex);
        }
            
    });

    reset();
}

void ConfigurationWidget::updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles /*= QVector<int>()*/)
{
}

void ConfigurationWidget::setPersistentModelIndex(const QPersistentModelIndex& modelIndex)
{
    ModelItemWidget::setPersistentModelIndex(modelIndex);

    _ui->channelsWidget->setPersistentModelIndex(getSiblingModelIndex(static_cast<int>(Configuration::Child::Channels)));
    _ui->globalSettingsWidget->setPersistentModelIndex(getSiblingModelIndex(static_cast<int>(Configuration::Child::Global)));
    _ui->differentialProfileSettingsWidget->setPersistentModelIndex(getSiblingModelIndex(static_cast<int>(Configuration::Child::DifferentialProfile)));
    _ui->miscellaneousSettingsWidget->setPersistentModelIndex(getSiblingModelIndex(static_cast<int>(Configuration::Child::Miscellaneous)));
}