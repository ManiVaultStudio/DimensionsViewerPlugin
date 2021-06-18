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
    _enableSubsamplingAction(this, "Subsampling"),
    _subsamplingFactorAction(this, "Subsampling factor", 0.1, 1.0, 0.5, 0.5, 2)
{
    setEnabled(false);
    setEventCore(_dimensionsViewerPlugin->getCore());

    const auto updateUI = [this]() -> void {
        _subsamplingFactorAction.setEnabled(_enableSubsamplingAction.isChecked());
    };

    connect(&_enableSubsamplingAction, &ToggleAction::toggled, [this, updateUI]() {
        updateUI();
    });

    updateUI();
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

    auto enableSubsamplingWidget    = miscellaneousAction->_enableSubsamplingAction.createWidget(this);
    auto subsamplingFactorWidget    = miscellaneousAction->_subsamplingFactorAction.createWidget(this);

    //selectionCenterNameWidget->getComboBox()->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    groupBoxLayout->setMargin(9);
    groupBoxLayout->setSpacing(10);
    groupBoxLayout->addWidget(enableSubsamplingWidget);
    groupBoxLayout->addWidget(subsamplingFactorWidget);
}