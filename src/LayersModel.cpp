#include "LayersModel.h"
#include "SettingsAction.h"

#include <Application.h>
#include <util/Exception.h>
#include <event/Event.h>
#include <PointData.h>

#include <QMessageBox>
#include <QPainter>

#include <stdexcept>

using namespace hdps;

LayersModel::LayersModel(QObject* parent) :
    QAbstractListModel(parent),
    _layers()
{
}

LayersModel::~LayersModel()
{
    // Remove all channels
    for (std::int32_t row = 0; rowCount(); row++)
        removeLayer(row);
}

int LayersModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
    return _layers.count();
}

int LayersModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
    return Column::Count;
}

QModelIndex LayersModel::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
{
    return createIndex(row, column, static_cast<void*>(_layers.at(row)));
}

QVariant LayersModel::data(const QModelIndex& index, int role) const
{
    auto layer = static_cast<Layer*>(index.internalPointer());

    switch (role) {
        case Qt::DisplayRole:
        {
            switch (static_cast<Column>(index.column())) {
                case Column::Visible:
                    break;

                case Column::Name:
                    return data(index, Qt::EditRole).toString();

                default:
                    break;
            }

            break;
        }

        case Qt::EditRole:
        {
            switch (static_cast<Column>(index.column())) {
                case Column::Visible:
                    return layer->getChannelAction().isChecked();

                case Column::Name:
                    return layer->getChannelAction().getDisplayName();

                default:
                    break;
            }

            break;
        }

        case Qt::CheckStateRole:
        {
            switch (static_cast<Column>(index.column())) {
                case Column::Visible:
                    return layer->getChannelAction().isChecked() ? Qt::Checked : Qt::Unchecked;

                case Column::Name:
                    break;

                default:
                    break;
            }

            break;
        }

        case Qt::DecorationRole:
        {
            switch (static_cast<Column>(index.column())) {
                case Column::Visible:
                case Column::Name:
                    break;

                default:
                    break;
            }

            break;
        }
        
        case Qt::ForegroundRole:
        {
            switch (static_cast<Column>(index.column())) {
                case Column::Visible:
                case Column::Name:
                    return QColor(layer->getChannelAction().isChecked() ? QApplication::palette().color(QPalette::Text) : QApplication::palette().color(QPalette::Disabled, QPalette::Text));

                default:
                    break;
            }
        }
    }

    return QVariant();
}

bool LayersModel::setData(const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole*/)
{
    auto layer = static_cast<Layer*>(index.internalPointer());

    switch (role) {
        case Qt::DisplayRole:
            break;

        case Qt::EditRole:
        {
            switch (static_cast<Column>(index.column())) {
                case Column::Visible:
                    break;

                case Column::Name:
                {
                    // TODO
                    break;
                }

                default:
                    break;
            }

            break;
        }

        case Qt::CheckStateRole:
        {
            switch (static_cast<Column>(index.column())) {
                case Column::Visible:
                {
                    layer->getChannelAction().setChecked(value.toBool());
                    break;
                }

                case Column::Name:
                    break;

                default:
                    break;
            }

            break;
        }
    }

    emit dataChanged(index, index);

    return true;
}

QVariant LayersModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
    const auto iconSize = QSize(14, 14);

    if (orientation == Qt::Horizontal) {
        switch (role)
        {
            case Qt::DisplayRole:
            {
                switch (static_cast<Column>(section))
                {
                    case Column::Visible:
                        return "";

                    case Column::Name:
                        return "Name";

                    default:
                        break;
                }

                break;
            }

            case Qt::ToolTipRole:
            {
                switch (static_cast<Column>(section))
                {
                    case Column::Visible:
                        return "Whether the layer is visible or not";

                    case Column::Name:
                        return "Name of the layer";

                    default:
                        break;
                }

                break;
            }

            case Qt::DecorationRole:
            {
                switch (static_cast<Column>(section))
                {
                    case Column::Visible:
                    case Column::Name:
                        break;

                    default:
                        break;
                }

                break;
            }

            case Qt::TextAlignmentRole:
            {
                switch (static_cast<Column>(section)) {
                    case Column::Visible:
                        return Qt::AlignCenter;

                    case Column::Name:
                        break;

                    default:
                        break;
                }

                break;
            }

            default:
                break;
        }
    }

    return QVariant();
}

Qt::ItemFlags LayersModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    auto itemFlags = Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | QAbstractItemModel::flags(index);

    auto layer = static_cast<Layer*>(index.internalPointer());

    if (index.column() == Column::Visible) {
        itemFlags |= Qt::ItemIsUserCheckable;
    }
    else {
        if (index.column() == Column::Name)
            itemFlags |= Qt::ItemIsEditable;
    }

    return itemFlags;
}

void LayersModel::addLayer(Layer* layer)
{
    try
    {
        // Insert the layer action at the beginning
        beginInsertRows(QModelIndex(), 0, 0);
        {
            // Insert the layer at the beginning (layer will be added on top of all other layers)
            _layers.insert(0, layer);

            connect(&layer->getChannelAction().getRemoveAction(), &TriggerAction::triggered, this, [this, layer]() {
                removeLayer(_layers.indexOf(layer));
            });
        }
        endInsertRows();

        if (_layers.count() == 1) {
            const auto numPoints        = layer->getDatasetReference()->getNumPoints();
            const auto numDimensions    = layer->getDatasetReference()->getNumDimensions();
            const auto updateDuringDrag = numPoints < 100000;

            auto& dimensionsAction = layer->getSettingsAction().getDimensionsAction();

            dimensionsAction.getSelectionCenterIndexAction().setUpdateDuringDrag(updateDuringDrag);
            dimensionsAction.getSelectionCenterIndexAction().setMaximum(numDimensions - 1);
            dimensionsAction.getSelectionCenterIndexAction().setValue(static_cast<std::int32_t>(floorf(static_cast<float>(numDimensions) / 2.0f)));
            dimensionsAction.getSelectionCenterNameAction().setOptions(layer->getChannelAction().getDimensionNames());
            dimensionsAction.getSelectionRadiusAction().setUpdateDuringDrag(updateDuringDrag);
        }
    }
    catch (std::exception& e)
    {
        exceptionMessageBox("Unable to add layer to the layers model", e);
    }
    catch (...) {
        exceptionMessageBox("Unable to add layer to the layers model");
    }
}

void LayersModel::removeLayer(const std::uint32_t& row)
{
    try
    {
        // Get pointer to layer which needs to be removed
        auto removeLayer = _layers[row];

#ifdef LAYERS_MODEL_VERBOSE
        qDebug() << "Remove layer:" << removeLayer->getChannelAction().getDisplayName();
#endif

        // Remove the row from the model
        beginRemoveRows(QModelIndex(), row, row);
        {
            // Remove the layer from the list
            _layers.removeAt(row);

            // Remove the layer physically
            delete removeLayer;
        }
        endRemoveRows();
    }
    catch (std::exception& e)
    {
        exceptionMessageBox("Unable to remove layer from the layers model", e);
    }
    catch (...) {
        exceptionMessageBox("Unable to remove layer from the layers model");
    }
}

void LayersModel::removeLayer(const QModelIndex& layerModelIndex)
{
    removeLayer(layerModelIndex.row());
}

void LayersModel::moveLayer(const QModelIndex& layerModelIndex, const std::int32_t& amount /*= 1*/)
{
    try
    {
        // Establish source and target row index
        const auto sourceRowIndex = layerModelIndex.row();
        const auto targetRowIndex = std::clamp(sourceRowIndex + amount, 0, rowCount() - 1);

        QVector<std::int32_t> rowIndices{ sourceRowIndex, targetRowIndex };

        if (sourceRowIndex < targetRowIndex) {
            
            // Begin moving the row in the model
            if (beginMoveRows(QModelIndex(), rowIndices.first(), rowIndices.first(), QModelIndex(), rowIndices.last() + 1)) {

                // Re-arrange internal layers data
                _layers.insert(rowIndices.last() + 1, _layers[rowIndices.first()]);
                _layers.removeAt(rowIndices.first());

                // Finished moving row around
                endMoveRows();
            }
            else {
                throw std::runtime_error("Unable to begin moving rows");
            }
        }
        else {

            // Begin moving the row in the model
            if (beginMoveRows(QModelIndex(), rowIndices.first(), rowIndices.first(), QModelIndex(), rowIndices.last())) {

                auto cache = _layers[rowIndices.first()];

                // Re-arrange internal layers data
                _layers.removeAt(rowIndices.first());
                _layers.insert(rowIndices.last(), cache);

                // Finished moving row around
                endMoveRows();
            }
            else {
                throw std::runtime_error("Unable to begin moving rows");
            }
        }
    }
    catch (std::exception& e)
    {
        exceptionMessageBox("Unable to move layer in the layers model", e);
    }
    catch (...) {
        exceptionMessageBox("Unable to move layer in the layers model");
    }
}

QVector<Layer*>& LayersModel::getLayers()
{
    return _layers;
}

QIcon LayersModel::getColorIcon(const QColor& color) const
{
    QPixmap pixmap(QSize(13, 13));

    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);

    const auto radius = 3;

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(color));
    painter.drawRoundedRect(0, 0, pixmap.width(), pixmap.height(), radius, radius);

    return QIcon(pixmap);
}
