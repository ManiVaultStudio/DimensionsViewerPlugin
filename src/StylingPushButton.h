#pragma once

#include <QPushButton>
#include <QPersistentModelIndex>

class StylingWidget;

/**
 * Styling push button class
 *
 * @author T. Kroes
 */
class StylingPushButton : public QPushButton
{
public: // Construction

    /**
     * Constructor
     * @param parent Parent widget
     */
    StylingPushButton(QWidget* parent = nullptr);

public: // Miscellaneous

    /**
     * Sets the styling model index
     * @param index Styling model index
     */
    void setModelIndex(const QPersistentModelIndex& modelIndex);

private:
    QPersistentModelIndex       _modelIndex;        /** Model index */
};