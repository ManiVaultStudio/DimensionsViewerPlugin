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
        Widget(QWidget* parent, StylingAction* stylingAction);

    public:
        QGridLayout* getLayout() { return _layout; }

    protected:
        QGridLayout*    _layout;

        friend class StylingAction;
    };

    QWidget* getWidget(QWidget* parent, const Widget::State& state = Widget::State::Standard) override {
        return new StylingAction::Widget(parent, this);
    };

public:
    StylingAction(ChannelAction* channelAction);

    hdps::gui::ToggleAction& getShowRangeAction() { return _showRangeAction; }
    hdps::gui::ToggleAction& getShowPointsAction() { return _showPointsAction; }
    hdps::gui::ColorAction& getColorAction() { return _colorAction; }
    hdps::gui::DecimalAction& getOpacityAction() { return _opacityAction; }
    hdps::gui::OptionAction& getPrimaryLineTypeAction() { return _primaryLineTypeAction; }
    hdps::gui::DecimalAction& getPrimaryLineThicknessAction() { return _primaryLineThicknessAction; }
    hdps::gui::OptionAction& getSecondaryLineTypeAction() { return _secondaryLineTypeAction; }
    hdps::gui::DecimalAction& getSecondaryLineThicknessAction() { return _secondaryLineThicknessAction; }

    /*
    StylingAction& operator= (const StylingAction& other)
    {
        _showRangeAction                = other._showRangeAction;
        _showPointsAction               = other._showPointsAction;
        _colorAction                    = other._colorAction;
        _opacityAction                  = other._opacityAction;
        _primaryLineTypeAction          = other._primaryLineTypeAction;
        _primaryLineThicknessAction     = other._primaryLineThicknessAction;
        _secondaryLineTypeAction        = other._secondaryLineTypeAction;
        _secondaryLineThicknessAction   = other._secondaryLineThicknessAction;

        return *this;
    }
    */

protected:
    hdps::gui::ToggleAction     _showRangeAction;
    hdps::gui::ToggleAction     _showPointsAction;
    hdps::gui::ColorAction      _colorAction;
    hdps::gui::DecimalAction    _opacityAction;
    hdps::gui::OptionAction     _primaryLineTypeAction;
    hdps::gui::DecimalAction    _primaryLineThicknessAction;
    hdps::gui::OptionAction     _secondaryLineTypeAction;
    hdps::gui::DecimalAction    _secondaryLineThicknessAction;
};