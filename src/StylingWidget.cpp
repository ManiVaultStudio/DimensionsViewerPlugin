#include "StylingWidget.h"
#include "Styling.h"

#include "ui_StylingWidget.h"

#include <QDebug>
#include <QStringListModel>

StylingWidget::StylingWidget(QWidget* parent) :
    ModelItemWidget(parent),
    _ui { std::make_unique<Ui::StylingWidget>() }
{
    setWindowFlags(Qt::Popup);

    setObjectName("StylingWidget");
    setStyleSheet("QWidget#StylingWidget { border: 1px solid grey; }");

    _ui->setupUi(this);

    move(parent->mapToGlobal(parent->rect().bottomRight()) - QPoint(width(), 0));

    QObject::connect(_ui->lineTypeProfileComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        setData(to_ul(Styling::Column::LineTypeProfile), currentText);
    });

    QObject::connect(_ui->lineTypeRangeComboBox, &QComboBox::currentTextChanged, [this](QString currentText) {
        setData(to_ul(Styling::Column::LineTypeRange), currentText);
    });

    QObject::connect(_ui->opacitySpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), [this](double value) {
        setData(to_ul(Styling::Column::Opacity), value);
    });

    QObject::connect(_ui->opacitySlider, &QSlider::valueChanged, [this](int value) {
        setData(to_ul(Styling::Column::Opacity), 0.01f * static_cast<float>(value));
    });
}

void StylingWidget::updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles /*= QVector<int>()*/)
{
    QVector<QSharedPointer<QSignalBlocker>> signalBlockers;

    signalBlockers << QSharedPointer<QSignalBlocker>::create(_ui->groupBox);
    signalBlockers << QSharedPointer<QSignalBlocker>::create(_ui->lineTypeProfileComboBox);
    signalBlockers << QSharedPointer<QSignalBlocker>::create(_ui->lineTypeRangeComboBox);
    signalBlockers << QSharedPointer<QSignalBlocker>::create(_ui->opacitySpinBox);
    signalBlockers << QSharedPointer<QSignalBlocker>::create(_ui->opacitySlider);

    if (begin == QModelIndex() && end == QModelIndex()) {
        _ui->groupBox->setEnabled(false);
        _ui->groupBox->setChecked(false);

        return;
    }

    for (int column = begin.column(); column <= end.column(); column++) {
        const auto index = begin.siblingAtColumn(column);

        if (column == to_ul(Styling::Column::LineTypes)) {
            _ui->lineTypeProfileComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
            _ui->lineTypeRangeComboBox->setModel(new QStringListModel(index.data(Qt::EditRole).toStringList()));
        }

        if (column == to_ul(Styling::Column::LineTypeProfile)) {
            _ui->lineTypeProfileComboBox->setVisible(index.flags() & Qt::ItemIsEditable);
            _ui->lineTypeProfileComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->lineTypeProfileComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
            _ui->lineTypeProfileComboBox->setCurrentText(index.data(Qt::DisplayRole).toString());
        }

        if (column == to_ul(Styling::Column::LineTypeRange)) {
            _ui->lineTypeRangeComboBox->setVisible(index.flags() & Qt::ItemIsEditable);
            _ui->lineTypeRangeComboBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->lineTypeRangeComboBox->setToolTip(index.data(Qt::ToolTipRole).toString());
            _ui->lineTypeRangeComboBox->setCurrentText(index.data(Qt::DisplayRole).toString());
        }

        if (column == to_ul(Styling::Column::Opacity)) {
            _ui->opacitySpinBox->setVisible(index.flags() & Qt::ItemIsEditable);
            _ui->opacitySpinBox->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->opacitySpinBox->setToolTip(index.data(Qt::ToolTipRole).toString());
            _ui->opacitySpinBox->setValue(index.data(Qt::EditRole).toFloat());

            _ui->opacitySlider->setVisible(index.flags() & Qt::ItemIsEditable);
            _ui->opacitySlider->setEnabled(index.flags() & Qt::ItemIsEnabled);
            _ui->opacitySlider->setToolTip(index.data(Qt::ToolTipRole).toString());
            _ui->opacitySlider->setValue(index.data(Qt::EditRole).toFloat() * 100.0f);
        }
    }
}

void StylingWidget::setModelIndex(const QPersistentModelIndex& modelIndex)
{
    ModelItemWidget::setModelIndex(modelIndex);
}