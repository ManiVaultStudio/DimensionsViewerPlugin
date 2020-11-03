#pragma once

#include <memory>

#include <QWidget.h>

class DimensionsViewerPlugin;

namespace Ui {
	class MiscellaneousSettingsWidget;
}

/**
 * Miscellaneous settings widget class
 *
 * @author T. Kroes
 */
class MiscellaneousSettingsWidget : public QWidget
{
	Q_OBJECT

public: // Construction

    /**
     * Constructor
     * @param parent Parent widget
     */
    MiscellaneousSettingsWidget(QWidget* parent);

public:

	/**
	 * Updates the UI with model indices ranging from \p begin to \p end
	 * @param begin Start of model index range
	 * @param end End of model index range
	 * @param roles Data roles
	 */
	void updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles = QVector<int>());

private:
	std::unique_ptr<Ui::MiscellaneousSettingsWidget>	_ui;    /** UI from creator */

protected:
    static DimensionsViewerPlugin*  dimensionsViewerPlugin;     /** Pointer to an instance of the dimensions viewer plugin */

    friend class DimensionsViewerPlugin;
};