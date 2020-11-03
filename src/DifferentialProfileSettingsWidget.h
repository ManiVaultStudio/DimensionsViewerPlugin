#pragma once

#include <memory>

#include <QWidget.h>
#include <QVector.h>

class DimensionsViewerPlugin;

class QCheckBox;
class QComboBox;
class QPushButton;

namespace Ui {
	class DifferentialProfileSettingsWidget;
}

/**
 * Differential profile widget class
 *
 * @author T. Kroes
 */
class DifferentialProfileSettingsWidget : public QWidget
{
	Q_OBJECT

public: // Construction

    /**
     * Constructor
     * @param parent Parent widget
     */
    DifferentialProfileSettingsWidget(QWidget* parent);

public:

	/**
	 * Updates the UI with model indices ranging from \p begin to \p end
	 * @param begin Start of model index range
	 * @param end End of model index range
	 * @param roles Data roles
	 */
	void updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles = QVector<int>());

private:
	std::unique_ptr<Ui::DifferentialProfileSettingsWidget>  _ui;    /** UI from creator */

protected:
    static DimensionsViewerPlugin*  dimensionsViewerPlugin;         /** Pointer to an instance of the dimensions viewer plugin */

    friend class DimensionsViewerPlugin;
};