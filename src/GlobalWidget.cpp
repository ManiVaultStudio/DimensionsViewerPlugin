#include "GlobalWidget.h"
#include "DimensionsViewerPlugin.h"
#include "Configuration.h"

#include "ui_GlobalWidget.h"

#include <QDebug>
#include <QStringListModel>

GlobalWidget::GlobalWidget(QWidget* parent) :
    ModelItemWidget(parent),
	_ui{ std::make_unique<Ui::GlobalWidget>() }
{
	_ui->setupUi(this);

    QObject::connect(_ui->groupBox, &QGroupBox::toggled, [this](bool state) {
        setData(to_ul(Global::Columns::Enabled), state);
    });

    QObject::connect(_ui->profileTypeComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        setData(to_ul(Global::Columns::ProfileType), currentText);
    });

    QObject::connect(_ui->rangeTypeComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        setData(to_ul(Global::Columns::RangeType), currentText);
    });

    reset();
}

void GlobalWidget::updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles /*= QVector<int>()*/)
{
    QVector<QSharedPointer<QSignalBlocker>> signalBlockers;

    signalBlockers << QSharedPointer<QSignalBlocker>::create(_ui->groupBox);
    signalBlockers << QSharedPointer<QSignalBlocker>::create(_ui->profileTypeComboBox);
    signalBlockers << QSharedPointer<QSignalBlocker>::create(_ui->rangeTypeComboBox);

    if (begin == QModelIndex() && end == QModelIndex()) {
        _ui->groupBox->setEnabled(false);
        _ui->groupBox->setChecked(false);

        return;
	}

	for (int column = begin.column(); column <= end.column(); column++) {
		const auto index = begin.siblingAtColumn(column);

        if (column == to_ul(Global::Columns::Enabled)) {
            _ui->groupBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->groupBox->setChecked(index.data(Qt::EditRole).toBool());
            _ui->groupBox->setToolTip(index.data(Qt::ToolTipRole).toString());
        }

        if (column == to_ul(Global::Columns::ProfileTypes)) {
            _ui->profileTypeComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
        }

        if (column == to_ul(Global::Columns::ProfileType)) {
            _ui->profileTypeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->profileTypeComboBox->setCurrentText(index.data(Qt::DisplayRole).toString());
            _ui->profileTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
        }

        if (column == to_ul(Global::Columns::RangeTypes)) {
            _ui->rangeTypeComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
        }

        if (column == to_ul(Global::Columns::RangeType)) {
            _ui->rangeTypeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->rangeTypeComboBox->setCurrentText(index.data(Qt::DisplayRole).toString());
            _ui->rangeTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
        }
	}
}

void GlobalWidget::setModelIndex(const QPersistentModelIndex& modelIndex)
{
    ModelItemWidget::setModelIndex(modelIndex);
}