#pragma once

#include <memory>

#include <QWidget.h>
#include <QVector.h>

class DimensionsViewerPlugin;

namespace Ui {
	class ChannelWidget;
}

/**
 * Channel widget class
 *
 * UI widget class for interfacing with channel settings
 *
 * @author T. Kroes
 */
class ChannelWidget : public QWidget
{
	Q_OBJECT

public: // Construction

    /**
     * Constructor
     * @param dimensionsViewerPlugin Pointer to an instance of the dimensions viewer plugin
     * @param channelIndex Channel index
     */
    ChannelWidget(DimensionsViewerPlugin* dimensionsViewerPlugin, const std::uint32_t& channelIndex);

public:

	/**
	 * Updates the UI with model indices ranging from \p begin to \p end
	 * @param begin Start of model index range
	 * @param end End of model index range
	 * @param roles Data roles
	 */
	void updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles = QVector<int>());

private:
	DimensionsViewerPlugin*					_dimensionsViewerPlugin;        /** Pointer to an instance of the dimensions viewer plugin */
	std::unique_ptr<Ui::ChannelWidget>		_ui;                            /** UI from creator */
    std::uint32_t                           _channelIndex;                  /** Channel index */
};