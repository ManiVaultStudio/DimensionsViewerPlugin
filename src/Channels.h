#pragma once

#include "ModelItem.h"

class Channel;

#include <QObject>
#include <QVector>

/**
 * Channels class
 *
 * @author T. Kroes
 */
class Channels : public ModelItem {

protected: // Construction

	Channels(ModelItem* parent, const QString& datasetName, const QString& dataName);

public: // ModelIndex: MVC

    /**
     * Returns the number of rows in the model
     * @param parentIndex Parent index
     */
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    /** Returns the number of columns */
    int getColumnCount() const override;

    /**
     * Returns the item flags for the given model index
     * @param index Model index
     * @return Item flags for the index
     */
    Qt::ItemFlags getFlags(const QModelIndex& index) const override;

    /**
     * Returns the data for the given model index and data role
     * @param index Model index
     * @param role Data role
     * @return Data in variant form
     */
    QVariant getData(const QModelIndex& index, const int& role) const override;

    /**
     * Sets the data value for the given model index and data role
     * @param index Model index
     * @param value Data value in variant form
     * @param role Data role
     * @return Model indices that are affected by the operation
     */
    QModelIndexList setData(const QModelIndex& index, const QVariant& value, const int& role) override;

    /**
     * Returns the model index belonging to the given model row and column
     * @param row Model row
     * @param column Model column
     * @param parent Parent model index
     * @return Model index for the given model row and column
     */
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;

    /**
     * Returns the parent model index
     * @param index Model index
     * @return Parent model index for the given model index
     */
    QModelIndex parent(const QModelIndex& index) const override;

public: // ModelIndex: Hierarchy

    /**
     * Returns a model item node by index
     * @param index Index of the child model item
     * @return Model item at index
     */
    ModelItem* getChild(const int& index) const override;

    /** Returns the number of children */
    int getChildCount() const override;

public: // Overloaded operators

    Channel* operator [](int i) const { return _channels[i]; }

public:
    Channel* getChannelByDatasetName(const QString& datasetName);
    QVector<std::uint32_t> getChannelsEnabled() const;
    std::int32_t getNoChannelsEnabled() const;
    std::int32_t getNoDisplayChannels() const;

private:
    QVector<Channel*>       _channels;

    friend class Configuration;
};