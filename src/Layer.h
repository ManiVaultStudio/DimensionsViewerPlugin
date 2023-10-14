#pragma once

#include <QObject>

#include "ChannelAction.h"

using namespace mv;

class SettingsAction;

class Layer : public QObject
{
    Q_OBJECT

public:
    
    /**
     * Constructor
     * @param settingsAction Reference to the global settings action
     * @param dataset Smart pointer to primary dataset
     * @param profileType Profile type
     */
    Layer(SettingsAction& settingsAction, Dataset<Points> dataset, const ChannelAction::ProfileType& profileType = ChannelAction::ProfileType::Mean);

    /**
     * Get settings action
     * @return Reference to the global settings action
     */
    SettingsAction& getSettingsAction();

    /**
     * Get reference to the dataset smart pointer
     * @return Reference to the dataset smart pointer
     */
    Dataset<Points>& getDatasetReference();

    /**
     * Get differential dataset candidates
     * @return Vector of smart pointers to datasets which can be used for differential analysis
     */
    Datasets getDifferentialDatasetCandidates();

protected:

    /** Updates the differential dataset candidates */
    void updateDifferentialDatasetCandidates();

public: // Action getters

    ChannelAction& getChannelAction() { return _channelAction; }

signals:

    /**
     * Signals that the differential candidates changed
     * @param differentialDatasetCandidates Differential candidates
     */
    void differentialDatasetCandidatesChanged(Datasets differentialDatasetCandidates);

private:
    SettingsAction&     _settingsAction;                    /** Reference to the global settings action */
    Dataset<Points>     _dataset;                           /** Smart pointer to primary dataset */
    Datasets            _differentialDatasetCandidates;     /** Vector of smart pointers to datasets which can be used for differential analysis */
    ChannelAction       _channelAction;                     /** Channel action */
    EventListener       _eventListener;                     /** Liste to HDPS events */
};
