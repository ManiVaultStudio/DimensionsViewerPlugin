#include "ConfigurationWidget.h"
#include "DimensionsViewerPlugin.h"
#include "Configuration.h"

#include "ui_ConfigurationWidget.h"

ConfigurationWidget::ConfigurationWidget(QWidget* parent) :
    ModelItemWidget(parent),
	_ui{ std::make_unique<Ui::ConfigurationWidget>() }
{
	_ui->setupUi(this);

    _ui->treeView->setModel(&dimensionsViewerPlugin->getConfigurationsModel());

    QObject::connect(&getSelectionModel(), &QItemSelectionModel::selectionChanged, [this](const QItemSelection& selected, const QItemSelection& deselected) {
        const auto selectedRows = getSelectionModel().selectedRows();

        if (!selectedRows.isEmpty())
            setModelIndex(selectedRows.first());
    });
}

void ConfigurationWidget::updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles /*= QVector<int>()*/)
{
}

void ConfigurationWidget::setModelIndex(const QPersistentModelIndex& modelIndex)
{
    ModelItemWidget::setModelIndex(modelIndex);

    _ui->channelsWidget->setModelIndex(modelIndex.sibling(0, static_cast<int>(Configuration::Column::Channels)));
    _ui->globalSettingsWidget->setModelIndex(modelIndex.sibling(0, static_cast<int>(Configuration::Column::Global)));
    _ui->differentialProfileSettingsWidget->setModelIndex(modelIndex.sibling(0, static_cast<int>(Configuration::Column::DifferentialProfile)));
    _ui->miscellaneousSettingsWidget->setModelIndex(modelIndex.sibling(0, static_cast<int>(Configuration::Column::Miscellaneous)));
}