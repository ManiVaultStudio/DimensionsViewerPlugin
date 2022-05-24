#include "Layer.h"
#include "SettingsAction.h"

#include <PointData.h>
#include <event/Event.h>

Layer::Layer(SettingsAction& settingsAction, Dataset<Points> dataset, const ChannelAction::ProfileType& profileType /*= ChannelConfigurationAction::ProfileType::Mean*/) :
    QObject(&settingsAction),
    _settingsAction(settingsAction),
    _dataset(dataset),
    _channelAction(*this, dataset->getGuiName(), profileType),
    _eventListener()
{
    _eventListener.setEventCore(Application::core());

    _eventListener.registerDataEventByType(PointType, [this](hdps::DataEvent* dataEvent) {
        if (dataEvent->getType() == EventType::DataAdded || dataEvent->getType() == EventType::DataChanged)
            updateDifferentialDatasetCandidates();
    });

    updateDifferentialDatasetCandidates();
}

SettingsAction& Layer::getSettingsAction()
{
    return _settingsAction;
}

Dataset<Points>& Layer::getDatasetReference()
{
    return _dataset;
}

Datasets Layer::getDifferentialDatasetCandidates()
{
    return _differentialDatasetCandidates;
}

void Layer::updateDifferentialDatasetCandidates()
{
    _differentialDatasetCandidates.clear();

    if (!_dataset.isValid())
        return;

    const auto numDimensions = _dataset->getNumDimensions();

    QStringList datasetNames;

    for (auto candidateDataset : Application::core()->requestAllDataSets(QVector<DataType>({ PointType })))
        if (Dataset<Points>(candidateDataset)->getNumDimensions() == numDimensions && candidateDataset != _dataset)
            _differentialDatasetCandidates << candidateDataset;

    emit differentialDatasetCandidatesChanged(_differentialDatasetCandidates);
}
