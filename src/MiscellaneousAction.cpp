#include "MiscellaneousAction.h"
#include "ConfigurationAction.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>
#include <QLabel>
#include <QComboBox>

using namespace hdps;
using namespace hdps::gui;

MiscellaneousAction::MiscellaneousAction(ConfigurationAction* configurationAction) :
    PluginAction(configurationAction->getDimensionsViewerPlugin(), "Configuration"),
    hdps::EventListener(),
    _enableSubsamplingAction(this, "Enable subsampling"),
    _subsamplingFactorAction(this, "Subsampling factor")
{
    setEnabled(false);
    setEventCore(_dimensionsViewerPlugin->getCore());

    _subsamplingFactorAction.setMinimum(1);
    _subsamplingFactorAction.setMaximum(10);
    _subsamplingFactorAction.setValue(50);
    _subsamplingFactorAction.setPrefix(" dimensions");
}

MiscellaneousAction::Widget::Widget(QWidget* parent, MiscellaneousAction* miscellaneousAction) :
    WidgetAction::Widget(parent, miscellaneousAction, State::Standard)
{
    auto layout = new QVBoxLayout();
    
    setLayout(layout);

    auto groupBox = new QGroupBox("Miscellaneous");

    layout->setMargin(2);
    layout->addWidget(groupBox);

    auto groupBoxLayout = new QHBoxLayout();
    
    groupBox->setLayout(groupBoxLayout);

    /*
    auto selectionCenterLabel           = new QLabel("Center");
    auto selectionCenterIndexWidget     = DimensionsAction->_selectionCenterIndexAction.createSliderWidget(this);
    auto selectionCenterNameWidget      = dynamic_cast<OptionAction::Widget*>(DimensionsAction->_selectionCenterNameAction.createWidget(this));
    auto selectionRadiusLabel           = new QLabel("Radius");
    auto selectionRadiusWidget          = DimensionsAction->_selectionRadiusAction.createWidget(this);
    auto showDimensionNamesWidget       = DimensionsAction->_showNamesAction.createWidget(this);

    selectionCenterNameWidget->getComboBox()->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    groupBoxLayout->setMargin(9);
    groupBoxLayout->setSpacing(10);
    groupBoxLayout->addWidget(selectionCenterLabel);
    groupBoxLayout->addWidget(selectionCenterNameWidget);
    groupBoxLayout->addWidget(selectionCenterIndexWidget, 1);
    groupBoxLayout->addWidget(selectionRadiusLabel);
    groupBoxLayout->addWidget(selectionRadiusWidget, 1);
    groupBoxLayout->addWidget(showDimensionNamesWidget);
    */
}