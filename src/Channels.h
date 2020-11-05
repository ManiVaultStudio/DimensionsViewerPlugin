#pragma once

#include "ModelItem.h"

class Channel;
class Configuration;

#include <QObject>
#include <QVector>

/**
 * Channels class
 *
 * @author T. Kroes
 */
class Channels : public ModelItem {

public: // Columns

    /** Data columns */
    enum class Column {
        Title,

        Start = Title,
        End = Title
    };

    /** TODO */
    enum class Child {
        Channel1,
        Channel2,
        Channel3,

        Start = Channel1,
        End = Channel3
    };

protected: // Construction

	Channels(ModelItem* parent, const QString& datasetName, const QString& dataName);

public: // ModelIndex: MVC

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
     * Returns the data for the given column and data role
     * @param index Model index
     * @param role Data role
     * @return Data in variant form
     */
    QVariant getData(const int& column, const int& role) const override;

    /**
     * Sets the data value for the given model index and data role
     * @param index Model index
     * @param value Data value in variant form
     * @param role Data role
     * @return Model indices that are affected by the operation
     */
    QModelIndexList setData(const QModelIndex& index, const QVariant& value, const int& role) override;

public: // ModelIndex: Hierarchy

    /**
     * Returns a model item node by index
     * @param index Index of the child model item
     * @return Model item at index
     */
    ModelItem* getChild(const int& index) const override;

    /** Returns the number of children */
    int getChildCount() const override;

    /**
     * Returns the child index
     * @param child Pointer to child model item
     */
    int getChildIndex(ModelItem* child) const override;

public: // Overloaded operators

    Channel* operator [](int i) const { return _channels[i]; }

private:

    /** Returns parent configuration */
    Configuration* getConfiguration() const;

public:
    Channel* getChannelByDatasetName(const QString& datasetName);
    QVector<std::uint32_t> getChannelsEnabled() const;
    std::int32_t getNoChannelsEnabled() const;
    std::int32_t getNoDisplayChannels() const;

private:
    QVector<Channel*>       _channels;

    friend class Configuration;
    friend class Channel;
};