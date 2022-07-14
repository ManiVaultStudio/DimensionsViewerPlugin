#include "SubsamplingAction.h"
#include "SettingsAction.h"
#include "DimensionsViewerPlugin.h"

#include <QGroupBox>

using namespace hdps;
using namespace hdps::gui;

SubsamplingAction::SubsamplingAction(SettingsAction& settingsAction) :
    WidgetAction(&settingsAction),
    _settingsAction(settingsAction),
    _subsamplingFactorAction(this, "Subsampling factor", 1.0, 100.0, 50.0, 50.0, 1),
    _subsamplingFactorOneOverEighthAction(this, "12.5%"),
    _subsamplingFactorOneOverFourAction(this, "25.0%"),
    _subsamplingFactorOneOverTwoAction(this, "50.0%")
{
    setCheckable(true);
    setChecked(true);
    setEnabled(false);

    _subsamplingFactorAction.setUpdateDuringDrag(false);
    _subsamplingFactorAction.setSuffix("%");

    const auto updateUI = [this]() -> void {
        //_subsamplingFactorAction.setEnabled(isChecked());
    };

    connect(&_subsamplingFactorOneOverEighthAction, &TriggerAction::triggered, [this]() {
        _subsamplingFactorAction.setValue(12.5);
    });

    connect(&_subsamplingFactorOneOverFourAction, &TriggerAction::triggered, [this]() {
        _subsamplingFactorAction.setValue(25.0);
    });

    connect(&_subsamplingFactorOneOverTwoAction, &TriggerAction::triggered, [this]() {
        _subsamplingFactorAction.setValue(50.0);
    });

    updateUI();
}

bool SubsamplingAction::shouldSubsample() const
{
    return isChecked() && _subsamplingFactorAction.getValue() < 100.0;
}

double SubsamplingAction::getNormalizedFactor() const
{
    return 0.01 * _subsamplingFactorAction.getValue();
}

SubsamplingAction::Widget::Widget(QWidget* parent, SubsamplingAction* subsamplingAction) :
    WidgetActionWidget(parent, subsamplingAction, State::Standard)
{
    auto layout = new QVBoxLayout();
    
    setLayout(layout);

    auto groupBox = new QGroupBox("Subsampling");

    layout->setContentsMargins(2, 2, 2, 2);
    layout->addWidget(groupBox);

    auto groupBoxLayout = new QHBoxLayout();
    
    groupBox->setCheckable(true);
    groupBox->setLayout(groupBoxLayout);

    auto subsamplingFactorWidget                = subsamplingAction->_subsamplingFactorAction.createWidget(this);
    //auto subsamplingFactorOneOverEighthWidget   = subsamplingAction->_subsamplingFactorOneOverEighthAction.createWidget(this);
    //auto subsamplingFactorOneOverFourWidget     = subsamplingAction->_subsamplingFactorOneOverFourAction.createWidget(this);
    //auto subsamplingFactorOneOverTwoWidget      = subsamplingAction->_subsamplingFactorOneOverTwoAction.createWidget(this);

    groupBoxLayout->setContentsMargins(9, 9, 9, 9);
    groupBoxLayout->setSpacing(10);
    groupBoxLayout->addWidget(subsamplingFactorWidget);

    auto factorsLayout = new QHBoxLayout();

    factorsLayout->setContentsMargins(0, 0, 0, 0);
    factorsLayout->setSpacing(2);

    groupBoxLayout->addLayout(factorsLayout);
    
    /*
    factorsLayout->addWidget(subsamplingFactorOneOverEighthWidget);
    factorsLayout->addWidget(subsamplingFactorOneOverFourWidget);
    factorsLayout->addWidget(subsamplingFactorOneOverTwoWidget);
    */

    const auto updateUI = [this, groupBox, subsamplingAction]() -> void {
        QSignalBlocker groupBoxBlocker(groupBox);

        if (groupBox->isChecked() != subsamplingAction->isChecked())
            groupBox->setChecked(subsamplingAction->isChecked());
    };
    
    connect(groupBox, &QGroupBox::toggled, [this, subsamplingAction](bool state) {
        subsamplingAction->setChecked(state);
    });

    connect(subsamplingAction, &SubsamplingAction::toggled, [this, updateUI]() {
        updateUI();
    });

    updateUI();
}