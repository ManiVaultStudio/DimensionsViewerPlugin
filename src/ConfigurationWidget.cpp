#include "ConfigurationWidget.h"
#include "DimensionsViewerPlugin.h"
#include "Configuration.h"

#include "ui_ConfigurationWidget.h"

#include <QDebug>

ConfigurationWidget::ConfigurationWidget(QWidget* parent) :
    TreeItemWidget(parent),
    _ui{ std::make_unique<Ui::ConfigurationWidget>() }
{
    _ui->setupUi(this);

    _ui->treeView->setModel(&getModel());
    _ui->treeView->setSelectionModel(&getSelectionModel());

    //_ui->treeView->header()->hideSection(0);
    _ui->treeView->header()->hideSection(2);
    _ui->treeView->header()->hideSection(3);
    _ui->treeView->header()->hideSection(4);
    _ui->treeView->header()->hideSection(5);

    QObject::connect(&getSelectionModel(), &QItemSelectionModel::selectionChanged, [this](const QItemSelection& selected, const QItemSelection& deselected) {
        const auto selectedRows = getSelectionModel().selectedRows();

        if (!selectedRows.isEmpty()) {
            const auto firstRowIndex = selectedRows.first();

            if (firstRowIndex.siblingAtColumn(to_ul(tree::Item::Column::Type)).data(Qt::EditRole).toString() == "Configuration")
                setModelIndex(firstRowIndex);
        }
    });
}

void ConfigurationWidget::setModelIndex(const QPersistentModelIndex& modelIndex)
{
    TreeItemWidget::setModelIndex(modelIndex);

    _ui->channelsWidget->setModelIndex(getChild(static_cast<int>(Configuration::Child::Channels)));
}