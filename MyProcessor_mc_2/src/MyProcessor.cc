#include "MyProcessor.hh"
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

DECLARE_COMPONENT(MyProcessor)

MyProcessor::MyProcessor(const std::string& name, ISvcLocator* svcLoc)
    : GaudiAlgorithm(name, svcLoc),
      _output(0)
{
    // _description = "Print MC Truth" ;

}

StatusCode MyProcessor::initialize() {
    info() << "MyProcessor::initializing..." << endmsg;
    // printParameters();

    TFile *tree_file=new TFile(_treeFileName.value().c_str(),(_overwrite ? "RECREATE" : "UPDATE"));

    if (!tree_file->IsOpen()) {
        delete tree_file;
        tree_file=new TFile(_treeFileName.value().c_str(),"NEW");
    }

    _outputTree = new TTree(_treeName.value().c_str(),_treeName.value().c_str());
    _outputTree->SetAutoSave(32*1024*1024);  // autosave every 32MB
    _outputTree->Branch("mc_energy", &mc_energy, "mc_energy/F");

    _outputMC = new TTree("MC","MC");
    _outputMC->Branch("mc_energy", &mc_energy, "mc_energy/F");

    _Num = 0;

    info() << "MyProcessor::initializd" << endmsg;

    return GaudiAlgorithm::initialize();

}

StatusCode MyProcessor::execute()
{
    info() << "MyProcessor::executing..." << endmsg;

    std::cout << "Event: " << _Num << std::endl;

    EVENT::LCEvent* evtP = nullptr;

    MCParticleColHandler m_mcParticle{_MCPCollectionName, Gaudi::DataHandle::Reader, this};

    mc_energy = 0;

    try{
        auto MCPCol = m_mcParticle.get();

        for (int s0 = 0; s0 < MCPCol->size(); ++s0) {
            auto MCP = (*MCPCol)[s0];

            mc_energy = MCP.getEnergy();
            std::cout << "mc_energy === " << mc_energy << std::endl;

            _outputMC->Fill();
        }

    }catch(lcio::DataNotAvailableException err) { }

    _outputTree->Fill();
    _Num++;

    info() << "MyProcessor::execute done" << endmsg;

    return StatusCode::SUCCESS;

}

StatusCode MyProcessor::finalize()
{

    if (_outputTree) {

        TFile *tree_file = _outputTree->GetCurrentFile(); //just in case we switched to a new file
        tree_file->Write();
        delete tree_file;
    }

    return GaudiAlgorithm::finalize();
}



