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
                setModelIndex(firstRowIndex);
        }
            
    });

    reset();
}

void ConfigurationWidget::updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles /*= QVector<int>()*/)
{
}

void ConfigurationWidget::setModelIndex(const QPersistentModelIndex& modelIndex)
{
    ModelItemWidget::setModelIndex(modelIndex);

    _ui->channelsWidget->setModelIndex(getSiblingModelIndex(static_cast<int>(Configuration::Row::Channels)));
    _ui->globalSettingsWidget->setModelIndex(getSiblingModelIndex(static_cast<int>(Configuration::Row::Global)));
    _ui->differentialProfileSettingsWidget->setModelIndex(getSiblingModelIndex(static_cast<int>(Configuration::Row::DifferentialProfile)));
    _ui->miscellaneousSettingsWidget->setModelIndex(getSiblingModelIndex(static_cast<int>(Configuration::Row::Miscellaneous)));
}