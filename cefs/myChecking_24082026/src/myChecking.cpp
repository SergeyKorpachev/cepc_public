#include "myChecking.h"
#include "GaudiKernel/StatusCode.h"
#include <cmath>
#include <iomanip>

DECLARE_COMPONENT(myChecking)

myChecking::myChecking(const std::string& name, ISvcLocator* pSvcLocator)
    : Algorithm(name, pSvcLocator) {}

StatusCode myChecking::initialize() {
    info() << "========================================" << endmsg;
    info() << "myChecking initialized" << endmsg;
    info() << "Print interval: " << m_printInterval << " events" << endmsg;
    info() << "========================================" << endmsg;
    return StatusCode::SUCCESS;
}

StatusCode myChecking::execute() {
    // ---- 1. Обработка PFO коллекции ----
    const auto* pfos = m_pfoHandle.get();
    if (pfos) {
        for (const auto& pfo : *pfos) {
            if (std::isnan(pfo.getEnergy()) || pfo.getEnergy() <= 0) continue;
            
            int pfoType = pfo.getType();
            
            countParticle(pfoType, false);
        }
    }
    
    // ---- 2. Обработка MC коллекции ----
    const auto* mcParticle = m_mcHandle.get();
    if (mcParticle) {
        for (const auto& mc : *mcParticle) {
            if (mc.getEnergy() <= 0) continue;
            
            int mcPdg = mc.getPDG();
            
            countParticle(mcPdg, true);
        }
    }
    
    m_pfo.eventsProcessed++;
    m_mc.eventsProcessed++;
    
    // Печатаем прогресс
    if (m_pfo.eventsProcessed % m_printInterval == 0) {
        printProgress("PFO", m_pfo);
        printProgress("MC",  m_mc);
    }
    
    return StatusCode::SUCCESS;
}

StatusCode myChecking::finalize() {
    info() << endmsg << "========================================" << endmsg;
    info() << "FINAL STATISTICS" << endmsg;
    info() << "========================================" << endmsg;
    printFinalStats();
    return StatusCode::SUCCESS;
}

void myChecking::countParticle(int pdg, bool isMC) {
    Counters& c = isMC ? m_mc : m_pfo;
    int absPdg = std::abs(pdg);
    
    c.total++;
    c.byPDG[pdg]++;
    
    // Группировка
    if (absPdg == 11) {
        c.electrons++;
    } else if (absPdg == 13) {
        c.muons++;
    } else if (pdg == 22) {
        c.photons++;
    } else {
        c.others++;
    }
}

void myChecking::printProgress(const std::string& label, const Counters& c) {
    info() << "--- " << label << " (events: " << c.eventsProcessed << ") ---" << endmsg;
    info() << "  Total: " << c.total << endmsg;
    info() << "  Electrons: " << c.electrons << endmsg;
    info() << "  Muons:     " << c.muons << endmsg;
    info() << "  Photons:   " << c.photons << endmsg;
    info() << "  Others:    " << c.others << endmsg;
}

void myChecking::printFinalStats() {
    info() << endmsg;
    info() << "Total events processed: " << m_pfo.eventsProcessed << endmsg;
    info() << endmsg;
    
    // ---- PFO статистика ----
    info() << "=== PFO PARTICLES ===" << endmsg;
    info() << "  Total:      " << m_pfo.total << endmsg;
    info() << "  Electrons:  " << m_pfo.electrons << endmsg;
    info() << "  Muons:      " << m_pfo.muons << endmsg;
    info() << "  Photons:    " << m_pfo.photons << endmsg;
    info() << "  Others:     " << m_pfo.others << endmsg;
    info() << endmsg;
    
    // ---- MC статистика ----
    info() << "=== MC PARTICLES ===" << endmsg;
    info() << "  Total:      " << m_mc.total << endmsg;
    info() << "  Electrons:  " << m_mc.electrons << endmsg;
    info() << "  Muons:      " << m_mc.muons << endmsg;
    info() << "  Photons:    " << m_mc.photons << endmsg;
    info() << "  Others:     " << m_mc.others << endmsg;
    info() << endmsg;
    
    // ---- Все уникальные PDG (опционально) ----
    if (m_printAllPDG) {
        info() << "=== ALL UNIQUE PDG CODES (PFO) ===" << endmsg;
        for (const auto& pair : m_pfo.byPDG) {
            info() << "  PDG " << std::setw(6) << pair.first << " : " << pair.second << endmsg;
        }
        info() << endmsg;
        
        info() << "=== ALL UNIQUE PDG CODES (MC) ===" << endmsg;
        for (const auto& pair : m_mc.byPDG) {
            info() << "  PDG " << std::setw(6) << pair.first << " : " << pair.second << endmsg;
        }
        info() << endmsg;
    }
    
    info() << "========================================" << endmsg;
}
