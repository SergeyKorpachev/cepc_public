#include "TotalInvJets.hh"
#include <EVENT/LCCollection.h>
#include <IMPL/LCCollectionVec.h>
#include <EVENT/LCFloatVec.h>
#include <EVENT/MCParticle.h>
#include <EVENT/ReconstructedParticle.h>
#include <IMPL/MCParticleImpl.h>
#include <values.h>
#include <string>
#include <iostream>
#include <EVENT/LCFloatVec.h>
#include <EVENT/LCParameters.h>
#include <stdexcept>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TVector3.h>
#include <TRandom.h>
#include <Rtypes.h>
#include <sstream>
#include <cmath>
#include <vector>
#include <TMath.h>
#include "TLorentzVector.h"
#include <UTIL/CellIDDecoder.h>

using namespace std;

DECLARE_COMPONENT(TotalInvJets)

TotalInvJets::TotalInvJets(const std::string& name, ISvcLocator* svcLoc)
    : GaudiAlgorithm(name, svcLoc),
      _output(0)
{

}

StatusCode TotalInvJets::initialize() {
    info() << "TotalInvJets::initializing..." << endmsg;

    TFile *tree_file=new TFile(_treeFileName.value().c_str(),(_overwrite ? "RECREATE" : "UPDATE"));

    if (!tree_file->IsOpen()) {
        delete tree_file;
        tree_file=new TFile(_treeFileName.value().c_str(),"NEW");
    }

    _outputTree = new TTree(_treeName.value().c_str(),_treeName.value().c_str());
    _outputTree->SetAutoSave(32*1024*1024);  // autosave every 32MB
    _outputTree->Branch("Num", &_Num, "Num/I");

    _Num = 0;

    info() << "TotalInvJets::initializd" << endmsg;

     return GaudiAlgorithm::initialize();

}

StatusCode TotalInvJets::execute()
{
    info() << "TotalInvJets::executing..." << endmsg;

    EVENT::LCEvent* evtP = nullptr;

    MCParticleColHandler m_mcParticle{_MCPCollectionName, Gaudi::DataHandle::Reader, this};

    try{
        auto MCPCol = m_mcParticle.get();

        TVector3 tmpP;

        for (int s0 = 0; s0 < MCPCol->size(); ++s0) {

            auto MCP = (*MCPCol)[s0];
            int tmpPID = MCP.getPDG();

            if ((s0 == 6) || (s0 == 7) || (s0 == 8) || (s0 == 9)) {

               std::cout << "PDG: " << tmpPID << " and index: " << s0 << std::endl;

               auto tmpP0 = MCP.getMomentum();
               tmpP = TVector3(tmpP0.x, tmpP0.y, tmpP0.z);
               float energy = MCP.getEnergy();

               TLorentzVector MCPartic( MCP.getMomentum()[0], MCP.getMomentum()[1], MCP.getMomentum()[2], MCP.getEnergy());

            }

        }

    }catch(lcio::DataNotAvailableException err) { }

    _outputTree->Fill();
    _Num++;

    std::cout << "Event: " << _Num << std::endl;

    return StatusCode::SUCCESS;

}

StatusCode TotalInvJets::finalize()
{

    if (_outputTree) {

        TFile *tree_file = _outputTree->GetCurrentFile(); //just in case we switched to a new file
        tree_file->Write();
        delete tree_file;
    }

    return GaudiAlgorithm::finalize();
}



