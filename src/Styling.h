#pragma once

#include "Common.h"

#include <QObject>
#include <QMap>
#include <QColor>
#include <QIcon>
#include <QPixmap>
#include <QPainter>
#include <QAbstractListModel>

/**
 * Styling utility class
 *
 * @author T. Kroes
 */
class Styling {

public: // Columns and rows

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

    /** Default constructor */
    Styling();

public: // Operators

    Styling& operator=(const Styling& other)
    {
        _lineTypeProfile    = other._lineTypeProfile;
        _lineTypeRange      = other._lineTypeRange;
        _renderPoints       = other._renderPoints;
        _opacity            = other._opacity;
        //_color              = other._color;

        return *this;
    }

public: // Getters/setters

    /** Gets line type names */
    QStringList getLineTypeNames() const;

    /** Gets profile line type */
    LineType getLineTypeProfile() const;

    /**
     * Sets profile line type
     * @param lineTypeProfile Line type for profile
     */
    void setLineTypeProfile(const LineType& lineTypeProfile);

    /** Gets range line type */
    LineType getLineTypeRange() const;

    /**
     * Sets range line type
     * @param lineTypeRange Line type for range
     */
    void setLineTypeRange(const LineType& lineTypeRange);

    /** Gets render points setting */
    bool getRenderPoints() const;

    /**
     * Sets render points setting
     * @param renderPoints Render points
     */
    void setRenderPoints(const bool& renderPoints);

    /** Gets opacity */
    float getOpacity() const;

    /**
     * Sets opacity
     * @param opacity Opacity
     */
    void setOpacity(const float& opacity);

    /** Gets color */
    QColor getColor() const;

    /**
     * Sets color
     * @param color Color
     */
    void setColor(const QColor& color);
    
private:
    LineType    _lineTypeProfile;       /** Line type for drawing data profile */
    LineType    _lineTypeRange;         /** Line type for drawing data range */
    bool        _renderPoints;          /** Whether to render points */
    float       _opacity;               /** Opacity for data range */
    QColor      _color;                 /** Color */
};