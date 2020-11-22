#pragma once

#include "Item.h"

#include <QObject>
#include <QMap>
#include <QColor>
#include <QIcon>
#include <QPixmap>
#include <QPainter>
#include <QAbstractListModel>

class Channel;

/**
 * Styling utility class
 *
 * @author T. Kroes
 */
class Styling : public tree::Item {

public: // Enumerations

    /** Child enumeration (each child enum item maps to a child tree item) */
    enum class Child {
        LineTypes,              /** Line types */
        LineTypeProfile,        /** Line type for drawing data profile */
        LineTypeRange,          /** Line type for drawing data range */
        RenderPoints,           /** Render points */
        Opacity,                /** Opacity */
        Color,                  /** Color */

        _Start  = LineTypes,
        _End    = Color,
        _Count  = _End + 1
    };

    /** Children set alias */
    using Children = QSet<Child>;

    /** Maps child name to child enum */
    static QMap<QString, Child> const children;

public: // Enumerations
    
        /** Line types */
    enum class LineType {
        Solid,          /** Display solid line */
        Dash,           /** Display dash line */
        Dot,            /** Display dot line */
        DashDot,        /** Display dash-dot line */
        DashDotDot,     /** Display dash-dot-dot line */

        _Start  = Solid,
        _End    = DashDot,
        _Count  = _End + 1
    };

    /** Maps line type name to line type enum */
    static QMap<QString, LineType> const lineTypes;

    /** Get line type name from line type enum */
    static QString getLineTypeName(const LineType& lineType) {
        return lineTypes.key(lineType);
    }

    /** Get line type enum from line type name */
    static LineType getLineTypeEnum(const QString& lineTypeName) {
        return lineTypes[lineTypeName];
    }

public: // Line types model

    class LineTypesModel : public QAbstractListModel {
    public:
        int rowCount(const QModelIndex& parent = QModelIndex()) const {
            return to_ul(Styling::LineType::_Count);
        }

        int columnCount(const QModelIndex& parent = QModelIndex()) const {
            return 1;
        }

        QVariant data(const QModelIndex& index, int role) const {
            const auto column   = index.column();
            const auto lineType = static_cast<LineType>(index.row());

            switch (role)
            {
                case Qt::DecorationRole:
                    return getDecorationRole(lineType);

                case Qt::DisplayRole:
                    return Styling::getLineTypeName(lineType);

                default:
                    break;
            }
            
            return QVariant();
        }

    private:
        /** Gets the decoration role */
        QIcon getDecorationRole(const LineType& lineType) const {
            QPixmap pixmap(iconSize);
            
            pixmap.fill(Qt::transparent);

            QPainter painter(&pixmap);

            painter.setRenderHint(QPainter::HighQualityAntialiasing);

            const auto lineWidth    = 1.6;
            const auto color        = QColor(0, 0, 0);
            const auto margin       = 2;

            switch (lineType)
            {
                case LineType::Solid:
                    painter.setPen(QPen(color, lineWidth, Qt::SolidLine));
                    break;

                case LineType::Dash:
                    painter.setPen(QPen(color, lineWidth, Qt::DashLine));
                    break;

                case LineType::Dot:
                    painter.setPen(QPen(color, lineWidth, Qt::DotLine));
                    break;

                case LineType::DashDot:
                    painter.setPen(QPen(color, lineWidth, Qt::DashDotLine));
                    break;

                case LineType::DashDotDot:
                    painter.setPen(QPen(color, lineWidth, Qt::DashDotDotLine));
                    break;
            }
            
            const auto y = iconSize.height() / 2.0;

            painter.drawLine(margin, y, iconSize.width() - margin, y);

            return QIcon(pixmap);
        }

    public:
        static const QSize iconSize;
    };

public: // Construction

    /**
     * Constructor
     * @param parent Parent tree item
     * @param name Name of the item
     */
    Styling(Item* parent, const QString& name);

public: // TreeItem: visitor API

    /** Accept visitor */
    void accept(tree::Visitor* visitor) const override;

private: // Miscellaneous

    /** get channel */
    const Channel* getChannel() const;

    friend class Channel;
};

/** Get scoped enum in columns set to work */
inline uint qHash(Styling::Child key, uint seed) {
    return ::qHash(static_cast<uint>(key), seed);
}