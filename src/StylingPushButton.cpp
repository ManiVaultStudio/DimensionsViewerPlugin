#include "StylingPushButton.h"
#include "StylingWidget.h"
#include "Application.h"

#include <QDebug>

StylingPushButton::StylingPushButton(QWidget* parent /*= nullptr*/) :
	QPushButton(parent),
    _modelIndex()
{
    const auto& fontAwesome = hdps::Application::getIconFont("FontAwesome");

    setFont(fontAwesome.getFont(9));
    setText(fontAwesome.getIconCharacter("cog"));

    QObject::connect(this, &QPushButton::clicked, [this]() {
        auto stylingWidget = new StylingWidget(this);

        stylingWidget->setModelIndex(_modelIndex);
        stylingWidget->show();
    });
}

void StylingPushButton::setModelIndex(const QPersistentModelIndex& modelIndex)
{
    _modelIndex = modelIndex;
}