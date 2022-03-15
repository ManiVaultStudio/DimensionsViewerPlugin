#include "StylingAction.h"
#include "ChannelAction.h"
#include "DimensionsViewerPlugin.h"
#include "Application.h"

#include <QDebug>
#include <QLabel>
#include <QPainter>
#include <QToolButton>

using namespace hdps;
using namespace hdps::gui;

const QMap<StylingAction::LineType, QString> StylingAction::lineTypes = QMap<StylingAction::LineType, QString>({
    { StylingAction::LineType::Solid, "Solid" },
    { StylingAction::LineType::Dash, "Dash" },
    { StylingAction::LineType::Dot, "Dot" },
    { StylingAction::LineType::DashDot, "DashDot" },
    { StylingAction::LineType::DashDotDot, "DashDotDot" }
});

const QVector<QColor> StylingAction::defaultColors = QVector<QColor>({
    QColor::fromHsv(109, 182, 200),
    QColor::fromHsv(204, 179, 200),
    QColor::fromHsv(300, 150, 200),
    QColor::fromHsv(40, 240, 200)
});

QIcon StylingAction::LineTypesModel::getDecorationRole(const LineType& lineType) const
{
    QPixmap pixmap(iconSize);

    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);

    painter.setRenderHint(QPainter::Antialiasing);

    const auto lineWidth    = 1.6;
    const auto color        = QColor(0, 0, 0);
    const auto margin       = 2;

    switch (lineType)
    {
        case LineType::Solid:
            painter.setPen(QPen(color, lineWidth, Qt::SolidLine));
            break;

        case LineType::Dash:
            painter.setPen(QPen(color, lineWidth, Qt::DashLine));
            break;

        case LineType::Dot:
            painter.setPen(QPen(color, lineWidth, Qt::DotLine));
            break;

        case LineType::DashDot:
            painter.setPen(QPen(color, lineWidth, Qt::DashDotLine));
            break;

        case LineType::DashDotDot:
            painter.setPen(QPen(color, lineWidth, Qt::DashDotDotLine));
            break;
    }

    const auto y = iconSize.height() / 2.0;

    painter.drawLine(margin, y, iconSize.width() - margin, y);

    return QIcon(pixmap);
}

const QSize StylingAction::LineTypesModel::iconSize = QSize(24, 12);

StylingAction::StylingAction(ChannelAction* channelAction) :
    PluginAction(channelAction->getDimensionsViewerPlugin(), "Styling"),
    _showRangeAction(this, "Show min/max range"),
    _showPointsAction(this, "Show points"),
    _colorAction(this, "Color"),
    _opacityAction(this, "Opacity"),
    _primaryLineTypeAction(this, "Line type"),
    _primaryLineThicknessAction(this, "Thickness"),
    _secondaryLineTypeAction(this, "Line type"),
    _secondaryLineThicknessAction(this, "Thickness")
{
    setIcon(Application::getIconFont("FontAwesome").getIcon("paint-brush"));

    _showRangeAction.setCheckable(true);
    _showRangeAction.setChecked(true);

    _showPointsAction.setCheckable(true);
    _showPointsAction.setChecked(true);

    _opacityAction.setMinimum(1.0);
    _opacityAction.setMaximum(100.0);
    _opacityAction.setSuffix("%");
    _opacityAction.setValue(100.0);

    const auto color = defaultColors[channelAction->getIndex()];

    _colorAction.setColor(color);
    _colorAction.setDefaultColor(color);

    auto lineTypesModel = new LineTypesModel();

    _primaryLineTypeAction.setCustomModel(lineTypesModel);
    _primaryLineTypeAction.setCurrentIndex(0);
    _primaryLineTypeAction.setDefaultIndex(0);

    _primaryLineThicknessAction.setSuffix("px");
    _primaryLineThicknessAction.setMinimum(0.1f);
    _primaryLineThicknessAction.setMaximum(10.0f);
    _primaryLineThicknessAction.setValue(2.0);
    _primaryLineThicknessAction.setDefaultValue(2.0);

    _secondaryLineTypeAction.setCustomModel(lineTypesModel);
    _secondaryLineTypeAction.setCurrentIndex(1);
    _secondaryLineTypeAction.setDefaultIndex(1);

    _secondaryLineThicknessAction.setSuffix("px");
    _secondaryLineThicknessAction.setMinimum(0.1f);
    _secondaryLineThicknessAction.setMaximum(10.0f);
    _secondaryLineThicknessAction.setValue(1.0f);
    _secondaryLineThicknessAction.setDefaultValue(1.0f);

    const auto updateShowRange = [this, channelAction]() {
        _showRangeAction.setEnabled(!channelAction->isDifferential());
    };

    connect(&channelAction->getProfileTypeAction(), &OptionAction::currentIndexChanged, [this, updateShowRange](const std::int32_t& currentIndex) {
        updateShowRange();
    });

    updateShowRange();
}

StylingAction::Widget::Widget(QWidget* parent, StylingAction* stylingAction) :
    WidgetActionWidget(parent, stylingAction, State::Standard),
    _layout(new QGridLayout())
{
    auto showRangeWidget                = stylingAction->_showRangeAction.createWidget(this);
    auto showPointsWidget               = stylingAction->_showPointsAction.createWidget(this);
    auto colorWidget                    = stylingAction->_colorAction.createWidget(this);
    auto opacityWidget                  = stylingAction->_opacityAction.createWidget(this);
    auto primaryLineTypeWidget          = stylingAction->_primaryLineTypeAction.createWidget(this);
    auto primaryLineThicknessWidget     = stylingAction->_primaryLineThicknessAction.createWidget(this);
    auto secondaryLineTypeWidget        = stylingAction->_secondaryLineTypeAction.createWidget(this);
    auto secondaryLineThicknessWidget   = stylingAction->_secondaryLineThicknessAction.createWidget(this);

    opacityWidget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    
    auto miscellaneousLayout = new QHBoxLayout();

    miscellaneousLayout->addWidget(showRangeWidget);
    miscellaneousLayout->addWidget(showPointsWidget);

    _layout->addLayout(miscellaneousLayout, 0, 1);

    _layout->addWidget(new QLabel("Color"), 1, 0);
    _layout->addWidget(colorWidget, 1, 1);

    _layout->addWidget(new QLabel("Opacity"), 2, 0);
    _layout->addWidget(opacityWidget, 2, 1);

    _layout->addWidget(new QLabel("Primary line"), 3, 0);

    auto primaryLayout = new QHBoxLayout();

    primaryLayout->addWidget(primaryLineTypeWidget);
    primaryLayout->addWidget(primaryLineThicknessWidget);

    _layout->addWidget(new QLabel("Secondary line"), 4, 0);
    _layout->addLayout(primaryLayout, 3, 1);

    auto secondaryLayout = new QHBoxLayout();

    secondaryLayout->addWidget(secondaryLineTypeWidget);
    secondaryLayout->addWidget(secondaryLineThicknessWidget);

    _layout->addLayout(secondaryLayout, 4, 1);

    setLayout(_layout);
}
