#pragma once

namespace tree {

class Item;

/**
 * Visitor abstract base class
 *
 * @author T. Kroes
 */
class Visitor {
public:
    virtual void visitTreeItem(const Item* treeItem) = 0;
};

}