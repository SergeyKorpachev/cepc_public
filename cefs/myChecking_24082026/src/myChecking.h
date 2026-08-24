#ifndef MY_CHECKING_H
#define MY_CHECKING_H

#include "GaudiKernel/Algorithm.h"
#include "k4FWCore/DataHandle.h"
#include "edm4hep/ReconstructedParticleCollection.h"
#include "edm4hep/MCParticleCollection.h"
#include <map>

class myChecking : public Algorithm {
public:
    myChecking(const std::string& name, ISvcLocator* pSvcLocator);
    
    StatusCode initialize() override;
    StatusCode execute() override;
    StatusCode finalize() override;

private:
    // Входные коллекции
    DataHandle<edm4hep::ReconstructedParticleCollection> m_pfoHandle{
        "CyberPFOPID", Gaudi::DataHandle::Reader, this};
    DataHandle<edm4hep::MCParticleCollection> m_mcHandle{
        "MCParticle", Gaudi::DataHandle::Reader, this};
    
    // Структура для счетчиков
    struct Counters {
        long long electrons = 0;
        long long muons = 0;
        long long photons = 0;
        long long others = 0;
        long long total = 0;
        long long eventsProcessed = 0;
        
        // Для отладки - все уникальные PDG
        std::map<int, long long> byPDG;
        
        void clear() {
            electrons = 0; muons = 0; photons = 0; 
            others = 0; total = 0; eventsProcessed = 0;
            byPDG.clear();
        }
    } m_pfo, m_mc;
    
    // Свойства
    Gaudi::Property<int> m_printInterval{this, "printInterval", 1000, 
        "Печатать прогресс каждые N событий"};
    Gaudi::Property<bool> m_printAllPDG{this, "printAllPDG", false,
        "Печатать все уникальные PDG коды в конце"};
    
    void countParticle(int pdg, bool isMC);
    void printProgress(const std::string& label, const Counters& c);
    void printFinalStats();
};

#endif
