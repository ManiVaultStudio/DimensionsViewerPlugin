#pragma once

#include "PluginAction.h"

#include <QAbstractListModel>

class ChannelAction;

class StylingAction : public PluginAction
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

    static QIcon getLineTypeIcon(const LineType& lineType, const QSize& iconSize = QSize(24, 12));

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

public:
    class Widget : public PluginAction::Widget {
    public:
        Widget(QWidget* parent, StylingAction* stylingAction);

    private:
        QHBoxLayout     _layout;
    };

    class PopupWidget : public PluginAction::PopupWidget {
    public:
        PopupWidget(QWidget* parent, StylingAction* stylingAction);

    private:
        QGridLayout     _layout;
    };

public:
    StylingAction(ChannelAction* channelAction);

    QWidget* createWidget(QWidget* parent) override {
        return new Widget(parent, this);
    }

    hdps::gui::StandardAction& getShowRangeAction() { return _showRangeAction; }
    hdps::gui::StandardAction& getShowPointsAction() { return _showPointsAction; }
    hdps::gui::ColorAction& getColorAction() { return _colorAction; }
    hdps::gui::DecimalAction& getOpacityAction() { return _opacityAction; }
    hdps::gui::OptionAction& getPrimaryLineTypeAction() { return _primaryLineTypeAction; }
    hdps::gui::DecimalAction& getPrimaryLineThicknessAction() { return _primaryLineThicknessAction; }
    hdps::gui::OptionAction& getSecondaryLineTypeAction() { return _secondaryLineTypeAction; }
    hdps::gui::DecimalAction& getSecondaryLineThicknessAction() { return _secondaryLineThicknessAction; }

protected:
    hdps::gui::StandardAction   _showRangeAction;
    hdps::gui::StandardAction   _showPointsAction;
    hdps::gui::ColorAction      _colorAction;
    hdps::gui::DecimalAction    _opacityAction;
    hdps::gui::OptionAction     _primaryLineTypeAction;
    hdps::gui::DecimalAction    _primaryLineThicknessAction;
    hdps::gui::OptionAction     _secondaryLineTypeAction;
    hdps::gui::DecimalAction    _secondaryLineThicknessAction;
};