#pragma once

#include <memory>

#include <QWidget.h>

class DimensionsViewerPlugin;

namespace Ui {
	class ChannelSettingsWidget;
}

/**
 * Channel settings widget class
 *
 * @author T. Kroes
 */
class ChannelSettingsWidget : public QWidget
{
	Q_OBJECT

public: // Construction

    /**
     * Constructor
     * @param parent Parent widget
     * @param channelIndex Channel index
     */
    ChannelSettingsWidget(QWidget* parent, const std::uint32_t& channelIndex);

public:

	/**
	 * Updates the UI with model indices ranging from \p begin to \p end
	 * @param begin Start of model index range
	 * @param end End of model index range
	 * @param roles Data roles
	 */
	void updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles = QVector<int>());

private:
	std::unique_ptr<Ui::ChannelSettingsWidget>		_ui;                            /** UI from creator */
    std::uint32_t                                   _channelIndex;                  /** Channel index */

protected:
    static DimensionsViewerPlugin*  dimensionsViewerPlugin;         /** Pointer to an instance of the dimensions viewer plugin */

    friend class DimensionsViewerPlugin;
};