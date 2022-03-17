#pragma once

#include <actions/WidgetAction.h>
#include <actions/ToggleAction.h>
#include <actions/ColorAction.h>
#include <actions/DecimalAction.h>
#include <actions/OptionAction.h>

#include <QAbstractListModel>

using namespace hdps::gui;

class Layer;
class ChannelAction;

class ChannelStylingAction : public WidgetAction
{
public:
    
    enum class LineType {
        Solid,          /** Display solid line */
        Dash,           /** Display dash line */
        Dot,            /** Display dot line */
        DashDot,        /** Display dash-dot line */
        DashDotDot,     /** Display dash-dot-dot line */
    };

    static const QMap<LineType, QString> lineTypes;

    class LineTypesModel : public QAbstractListModel {
    public:
        int rowCount(const QModelIndex& parent = QModelIndex()) const {
            return lineTypes.size();
        }

        int columnCount(const QModelIndex& parent = QModelIndex()) const {
            return 1;
        }

        QVariant data(const QModelIndex& index, int role) const {
            const auto column   = index.column();
            const auto lineType = static_cast<LineType>(index.row());

            switch (role)
            {
                case Qt::DecorationRole:
                    return getDecorationRole(lineType);

                case Qt::DisplayRole:
                    return lineTypes[lineType];

                default:
                    break;
            }

            return QVariant();
        }

    private:
        QIcon getDecorationRole(const LineType& lineType) const;

    public:
        static const QSize iconSize;
    };

    static const QVector<QColor> defaultColors;

protected: // Widget

    class Widget : public hdps::gui::WidgetActionWidget {
    protected:
        Widget(QWidget* parent, ChannelStylingAction* channelStylingAction);

    public:
        QGridLayout* getLayout() { return _layout; }

    protected:
        QGridLayout*    _layout;

        friend class ChannelStylingAction;
    };

    QWidget* getWidget(QWidget* parent, const std::int32_t& widgetFlags) override {
        return new Widget(parent, this);
    };

protected:
    ChannelStylingAction(Layer& layer, ChannelAction* channelConfigurationAction);

public:
    ToggleAction& getShowRangeAction() { return _showRangeAction; }
    ToggleAction& getShowPointsAction() { return _showPointsAction; }
    ColorAction& getColorAction() { return _colorAction; }
    DecimalAction& getOpacityAction() { return _opacityAction; }
    OptionAction& getPrimaryLineTypeAction() { return _primaryLineTypeAction; }
    DecimalAction& getPrimaryLineThicknessAction() { return _primaryLineThicknessAction; }
    OptionAction& getSecondaryLineTypeAction() { return _secondaryLineTypeAction; }
    DecimalAction& getSecondaryLineThicknessAction() { return _secondaryLineThicknessAction; }

protected:
    Layer&                          _layer;                                 /** Reference to owning layer */
    ToggleAction                    _showRangeAction;
    ToggleAction                    _showPointsAction;
    ColorAction                     _colorAction;
    DecimalAction                   _opacityAction;
    OptionAction                    _primaryLineTypeAction;
    DecimalAction                   _primaryLineThicknessAction;
    OptionAction                    _secondaryLineTypeAction;
    DecimalAction                   _secondaryLineThicknessAction;

    friend class ChannelAction;
};