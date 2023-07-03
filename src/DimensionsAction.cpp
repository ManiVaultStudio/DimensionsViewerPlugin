#include "DimensionsAction.h"
#include "SettingsAction.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>

using namespace hdps;
using namespace hdps::gui;

DimensionsAction::DimensionsAction(SettingsAction& settingsAction) :
    WidgetAction(&settingsAction, "DimensionsAction"),
    _settingsAction(settingsAction),
    _selectionCenterIndexAction(this, "Selection center dimension index"),
    _selectionCenterNameAction(this, "Selection center dimension name"),
    _selectionRadiusAction(this, "Selection radius", 1, 100, 25),
    _showNamesAction(this, "Show names"),
    _dimensionNames()
{
    setText("Dimensions");
    setIcon(Application::getIconFont("FontAwesome").getIcon("ruler-horizontal"));

    _selectionCenterIndexAction.setUpdateDuringDrag(false);

    _selectionRadiusAction.setUpdateDuringDrag(false);
    _selectionRadiusAction.setSuffix(" dims");

    _showNamesAction.setChecked(true);

    connect(&_selectionCenterIndexAction, &IntegralAction::valueChanged, [this](const std::int32_t& value){
        //_selectionCenterNameAction.setCurrentIndex(value);
    });

    connect(&_selectionCenterNameAction, &OptionAction::currentIndexChanged, [this](const std::int32_t& currentIndex) {
        _selectionCenterIndexAction.setValue(currentIndex);
    });

    auto& layersModel = _settingsAction.getDimensionsViewerPlugin().getLayersModel();

    const auto updateReadOnly = [this, &layersModel]() {
        setEnabled(layersModel.rowCount() > 0);
    };

    connect(&layersModel, &QAbstractListModel::rowsInserted, this, updateReadOnly);
    connect(&layersModel, &QAbstractListModel::rowsRemoved, this, updateReadOnly);
}

DimensionsAction::Widget::Widget(QWidget* parent, DimensionsAction* DimensionsAction) :
    WidgetActionWidget(parent, DimensionsAction, State::Standard)
{
    auto layout = new QHBoxLayout();

    auto selectionCenterLabel           = new QLabel("Center");
    auto selectionCenterIndexWidget     = DimensionsAction->_selectionCenterIndexAction.createWidget(this);
    auto selectionCenterNameWidget      = DimensionsAction->_selectionCenterNameAction.createWidget(this);
    auto selectionRadiusLabel           = new QLabel("Radius");
    auto selectionRadiusWidget          = DimensionsAction->_selectionRadiusAction.createWidget(this);
    auto showDimensionNamesWidget       = DimensionsAction->_showNamesAction.createWidget(this);

    selectionCenterNameWidget->findChild<QComboBox*>("ComboBox")->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    layout->addWidget(selectionCenterLabel);
    layout->addWidget(selectionCenterNameWidget);
    layout->addWidget(selectionCenterIndexWidget, 1);
    layout->addWidget(selectionRadiusLabel);
    layout->addWidget(selectionRadiusWidget, 1);
    layout->addWidget(showDimensionNamesWidget);

    setLayout(layout);
}
