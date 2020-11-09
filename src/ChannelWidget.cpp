#include "ChannelWidget.h"
#include "DimensionsViewerPlugin.h"
#include "Application.h"

#include "ui_ChannelWidget.h"

#include <QDebug>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QStringListModel>

ChannelWidget::ChannelWidget(QWidget* parent) :
    ModelItemWidget(parent),
	_ui{ std::make_unique<Ui::ChannelWidget>() }
{
	_ui->setupUi(this);

    const auto fontAwesome = QFont("Font Awesome 5 Free Solid", 8);

    QObject::connect(_ui->enabledCheckBox, &QCheckBox::stateChanged, [this](int state) {
        setData(to_ul(Channel::Column::Enabled), state);
    });
    /*
    QObject::connect(_ui->datasetNameComboBox, qOverload<int>(&QComboBox::currentIndexChanged), [this](int currentIndex) {
        if (_persistentModelIndex.row() == 0)
            getConfigurationsModel().selectRow(currentIndex);
        else
            setData(to_ul(Channel::Column::DatasetName), _ui->datasetNameComboBox->currentText());
    });

    _ui->colorPushButton->setShowText(false);
    _ui->colorPushButton->setColor(Qt::gray);

    QObject::connect(_ui->colorPushButton, &ColorPickerPushButton::colorChanged, [this](const QColor& color) {
        setData(to_ul(Channel::Column::Color), color);
    });

    QObject::connect(_ui->profileTypeComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        setData(to_ul(Channel::Column::ProfileType), currentText);
    });

    QObject::connect(_ui->rangeTypeComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        setData(to_ul(Channel::Column::RangeType), currentText);
    });*/

    

    reset();
}

void ChannelWidget::updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles /*= QVector<int>()*/)
{
    QVector<QSharedPointer<QSignalBlocker>> signalBlockers;

    signalBlockers << QSharedPointer<QSignalBlocker>::create(_ui->enabledCheckBox);
    signalBlockers << QSharedPointer<QSignalBlocker>::create(_ui->datasetNameComboBox);
    signalBlockers << QSharedPointer<QSignalBlocker>::create(_ui->colorPushButton);
    signalBlockers << QSharedPointer<QSignalBlocker>::create(_ui->profileTypeComboBox);
    signalBlockers << QSharedPointer<QSignalBlocker>::create(_ui->rangeTypeComboBox);
    signalBlockers << QSharedPointer<QSignalBlocker>::create(_ui->stylingPushButton);

	if (begin == QModelIndex() && end == QModelIndex()) {
        _ui->enabledCheckBox->setEnabled(false);
        _ui->enabledCheckBox->setChecked(false);
        _ui->datasetNameComboBox->setEnabled(false);
        _ui->datasetNameComboBox->setCurrentIndex(-1);
        _ui->colorPushButton->setEnabled(false);
        _ui->colorPushButton->setColor(Qt::gray);
        _ui->profileTypeComboBox->setEnabled(false);
        _ui->profileTypeComboBox->setCurrentIndex(-1);
        _ui->rangeTypeComboBox->setEnabled(false);
        _ui->rangeTypeComboBox->setCurrentIndex(-1);
        _ui->stylingPushButton->setEnabled(false);

		return;
	}

	for (int column = begin.column(); column <= end.column(); column++) {
		/*const auto index = begin.siblingAtColumn(column);

        if (column == to_ul(Channel::Column::DisplayName)) {
            _ui->enabledCheckBox->setText(index.data(Qt::EditRole).toString());
        }
        
        if (column == to_ul(Channel::Column::DatasetNames)) {
            _ui->datasetNameComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
        }

		if (column == to_ul(Channel::Column::DatasetName)) {
            _ui->datasetNameComboBox->setVisible(index.flags() & Qt::ItemIsEditable);
            _ui->datasetNameComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->datasetNameComboBox->setCurrentText(index.data(Qt::EditRole).toString());
            _ui->datasetNameComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
		}
        
		if (column == to_ul(Channel::Column::Color)) {
            _ui->colorPushButton->setVisible(index.flags() & Qt::ItemIsEditable);
            _ui->colorPushButton->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->colorPushButton->setColor(index.data(Qt::EditRole).value<QColor>());
            _ui->colorPushButton->setToolTip(index.data(Qt::ToolTipRole).toString());
		}
        
        if (column == to_ul(Channel::Column::ProfileTypes)) {
            _ui->profileTypeComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
        }

        if (column == to_ul(Channel::Column::ProfileType)) {
            _ui->profileTypeComboBox->setVisible(index.flags() & Qt::ItemIsEditable);
            _ui->profileTypeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->profileTypeComboBox->setCurrentText(index.data(Qt::DisplayRole).toString());
            _ui->profileTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
        }

        if (column == to_ul(Channel::Column::RangeTypes)) {
            _ui->rangeTypeComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
        }
        
		if (column == to_ul(Channel::Column::RangeType)) {
            _ui->rangeTypeComboBox->setVisible(index.flags() & Qt::ItemIsEditable);
            _ui->rangeTypeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->rangeTypeComboBox->setCurrentText(index.data(Qt::DisplayRole).toString());
            _ui->rangeTypeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
		}

        if (column == to_ul(Channel::Column::Settings)) {
            _ui->stylingPushButton->setVisible(index.flags() & Qt::ItemIsEditable);
            _ui->stylingPushButton->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->stylingPushButton->setToolTip(index.data(Qt::ToolTipRole).toString());
        }*/
	}
}

void ChannelWidget::setModelIndex(const QPersistentModelIndex& modelIndex)
{
    ModelItemWidget::setModelIndex(modelIndex);

    _ui->stylingPushButton->setModelIndex(getSiblingModelIndex(static_cast<int>(Channel::Row::Styling)));

    addMapper(getSiblingModelIndex(0, to_ul(Channel::Column::Enabled)), [this](const QModelIndex& index) {
        QSignalBlocker signalBlocker(_ui->enabledCheckBox);

        _ui->enabledCheckBox->setVisible(index.flags() & Qt::ItemIsEditable);
        _ui->enabledCheckBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
        _ui->enabledCheckBox->setChecked(index.data(Qt::EditRole).toBool());
        _ui->enabledCheckBox->setToolTip(index.data(Qt::ToolTipRole).toString());
    });

    addMapper(getSiblingModelIndex(0, to_ul(Channel::Column::DatasetNames)), [this](const QModelIndex& index) {
        QSignalBlocker signalBlocker(_ui->datasetNameComboBox);

        _ui->datasetNameComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
    });
}