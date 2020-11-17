#pragma once

#include "TreeItem.h"

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
class Styling : public TreeItem {

public: // Columns and rows

    /** Tree item columns */
    enum class Column {
        
        /** Derived tree item columns */
        Type,                           /** Type of tree item */
        Name,                           /** Name of tree item */
        Enabled,                        /** Whether the tree item is enabled or not */
        Modified,                       /** Last modified integer stamp */
        UUID,                           /** Universal unique identifier */

        /** Styling tree item columns */
        LineTypes,                      /** Line types */
        LineTypeProfile,                /** Line type for drawing data profile */
        LineTypeRange,                  /** Line type for drawing data range */
        RenderPoints,                   /** Render points */
        Opacity,                        /** Opacity */
        Color,                          /** Color */

        _Start  = LineTypes,
        _End    = Color,
        _Count  = _End + 1
    };

    /** Columns set alias */
    using Columns = QSet<Column>;

    /** Maps column name to column enum */
    static QMap<QString, Column> const columns;

    /** Get column name from column enum */
    static QString getColumnTypeName(const Column& column) {
        return columns.key(column);
    }

    /** Get column enum from column name */
    static Column getColumnTypeEnum(const QString& columnName) {
        return columns[columnName];
    }

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
     */
    Styling(TreeItem* parent);

public: // TreeItem: model API

    /**
     * Returns the item flags for the given model index
     * @param index Model index
     * @return Item flags for the index
     */
    Qt::ItemFlags getFlags(const QModelIndex& index) const override;

    /**
        * Get data role
        * @param column Column to fetch data from
        * @param role Data role
        * @return Data in variant form
        */
    QVariant getData(const std::int32_t& column, const std::int32_t& role) const override;

    /**
        * Get data role
        * @param column Column to fetch data from
        * @param role Data role
        * @return Data in variant form
        */
    QVariant getData(const Column& column, const std::int32_t& role) const;

    /**
        * Sets data
        * @param index Model index
        * @param value Data value in variant form
        * @param role Data role
        * @return Model indices that are affected by the operation
        */
    QModelIndexList setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role = Qt::EditRole) override;

    /**
     * Get indices that are affected by a data change of \p index
     * @param index Model index which changed
     * @return Affected model indices
     */
    QModelIndexList getAffectedIndices(const QModelIndex& index) const override;

    /** Get number of columns */
    std::uint32_t getColumnCount() const override {
        return to_ul(Column::_Count);
    };

public: // TreeItem: visitor API

    /** Accept visitor */
    void accept(Visitor* visitor) const override;

private: // Miscellaneous

    /** get channel */
    const Channel* getChannel() const;

protected:
    LineType    _lineTypeProfile;       /** Line type for drawing data profile */
    LineType    _lineTypeRange;         /** Line type for drawing data range */
    bool        _renderPoints;          /** Whether to render points */
    float       _opacity;               /** Opacity for data range */
    QColor      _color;                 /** Color */

    friend class Channel;
};

/** Get scoped enum in columns set to work */
inline uint qHash(Styling::Column key, uint seed) {
    return ::qHash(static_cast<uint>(key), seed);
}