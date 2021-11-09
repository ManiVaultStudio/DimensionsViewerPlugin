#include "DimensionsAction.h"
#include "ConfigurationAction.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>

using namespace hdps;
using namespace hdps::gui;

DimensionsAction::DimensionsAction(ConfigurationAction* configurationAction) :
    PluginAction(configurationAction->getDimensionsViewerPlugin(), "Configuration"),
    hdps::EventListener(),
    _selectionCenterIndexAction(this, "Selection center dimension index"),
    _selectionCenterNameAction(this, "Selection center dimension name"),
    _selectionRadiusAction(this, "Selection radius", 1, 100, 25, 25),
    _showNamesAction(this, "Show names"),
    _dimensionNames()
{
    setEnabled(false);
    setEventCore(_dimensionsViewerPlugin->getCore());

    _selectionCenterIndexAction.setUpdateDuringDrag(false);

    _selectionRadiusAction.setUpdateDuringDrag(false);
    _selectionRadiusAction.setSuffix(" dims");

    _showNamesAction.setChecked(true);

    connect(&_selectionCenterIndexAction, &IntegralAction::valueChanged, [this](const std::int32_t& value){
        _selectionCenterNameAction.setCurrentIndex(value);
    });

    connect(&_selectionCenterNameAction, &OptionAction::currentIndexChanged, [this](const std::int32_t& currentIndex) {
        _selectionCenterIndexAction.setValue(currentIndex);
    });
}

DimensionsAction::Widget::Widget(QWidget* parent, DimensionsAction* DimensionsAction) :
    WidgetActionWidget(parent, DimensionsAction, State::Standard)
{
    auto layout = new QVBoxLayout();
    
    setLayout(layout);

    auto groupBox = new QGroupBox("Dimensions");

    layout->setMargin(2);
    layout->addWidget(groupBox);

    auto groupBoxLayout = new QHBoxLayout();
    
    groupBox->setLayout(groupBoxLayout);

    auto selectionCenterLabel           = new QLabel("Center");
    auto selectionCenterIndexWidget     = DimensionsAction->_selectionCenterIndexAction.createWidget(this);
    auto selectionCenterNameWidget      = DimensionsAction->_selectionCenterNameAction.createWidget(this);
    auto selectionRadiusLabel           = new QLabel("Radius");
    auto selectionRadiusWidget          = DimensionsAction->_selectionRadiusAction.createWidget(this);
    auto showDimensionNamesWidget       = DimensionsAction->_showNamesAction.createWidget(this);

    selectionCenterNameWidget->findChild<QComboBox*>("ComboBox")->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    groupBoxLayout->setMargin(9);
    groupBoxLayout->setSpacing(10);
    groupBoxLayout->addWidget(selectionCenterLabel);
    groupBoxLayout->addWidget(selectionCenterNameWidget);
    groupBoxLayout->addWidget(selectionCenterIndexWidget, 1);
    groupBoxLayout->addWidget(selectionRadiusLabel);
    groupBoxLayout->addWidget(selectionRadiusWidget, 1);
    groupBoxLayout->addWidget(showDimensionNamesWidget);
}
