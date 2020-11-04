#pragma once

#include "ModelItemWidget.h"

#include <memory>

#include <QWidget.h>

class DimensionsViewerPlugin;

namespace Ui {
	class ChannelsWidget;
}

/**
 * Channels container widget class
 *
 * @author T. Kroes
 */
class ChannelsWidget : public ModelItemWidget
{
public: // Construction

    /**
     * Constructor
     * @param parent Parent widget
     */
	ChannelsWidget(QWidget* parent);

public: // Inherited from ModelItemWidget

    /**
     * Updates the UI with model indices ranging from \p begin to \p end
     * @param begin Start of model index range
     * @param end End of model index range
     * @param roles Data roles
     */
    void updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles = QVector<int>()) override;

private:
	std::unique_ptr<Ui::ChannelsWidget>		_ui;    /** UI from creator */
};