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

    _outputTree->Branch("EcalTotalE", &_EcalTotalE, "EcalTotalE/F");
    _outputTree->Branch("HcalTotalE", &_HcalTotalE, "HcalTotalE/F");

    _outputTree->Branch("Mass_a", &_Mass_a, "Mass_a/F");
    _outputTree->Branch("Mass_p", &_Mass_p, "Mass_p/F");

    _outputMC = new TTree("MC","MC");
    _outputMC->Branch("mc_energy", &mc_energy, "mc_energy/F");

    _outputPFO_a = new TTree("PFO_a","PFO_a");
    _outputPFO_a->Branch("pfo_a_energy", &pfo_a_energy, "pfo_a_energy/F");

    _outputPFO_p = new TTree("PFO_p","PFO_p");
    _outputPFO_p->Branch("pfo_p_energy", &pfo_p_energy, "pfo_p_energy/F");

    _Num = 0;

    info() << "MyProcessor::initializd" << endmsg;

    return GaudiAlgorithm::initialize();

}

StatusCode MyProcessor::execute()
{
    info() << "MyProcessor::executing..." << endmsg;

    EVENT::LCEvent* evtP = nullptr;

    MCParticleColHandler m_mcParticle{_MCPCollectionName, Gaudi::DataHandle::Reader, this};

    TLorentzVector ArborTotalP(0, 0, 0, 0);
    TLorentzVector PandoraTotalP(0, 0, 0, 0);

    mc_energy = 0;
    pfo_a_energy = 0;
    pfo_p_energy = 0;

    std::vector<CaloHitColHandler*> hdl_EcalHitColl{
        &m_ecalbarrelhitcol,
            &m_ecalendcaphitcol
            };
    std::vector<CaloHitColHandler*> hdl_HcalHitColl{
        &m_hcalbarrelhitcol,
            &m_hcalendcaphitcol,
            &m_hcalotherhitcol
            };

    std::vector<std::string> EcalHitColl;
    std::vector<std::string> HcalHitColl;
    EcalHitColl.push_back("ECALBarrel");
    EcalHitColl.push_back("ECALEndcap");
    //EcalHitColl.push_back("ECALOther");
    //EcalHitColl.push_back("LCAL");
    //EcalHitColl.push_back("LHCAL");
    HcalHitColl.push_back("HCALBarrel");
    HcalHitColl.push_back("HCALEndcap");
    HcalHitColl.push_back("HCALOther");

    try{

        for(int t = 0; t< int(hdl_EcalHitColl.size()); t++) {
            const edm4hep::CalorimeterHitCollection* ecalcoll = hdl_EcalHitColl[t]->get();
            for(auto hit: *ecalcoll) {
                _EcalTotalE += hit.getEnergy();
            }
        }

        for(int t2 = 0; t2< int(hdl_HcalHitColl.size()); t2++) {
            const edm4hep::CalorimeterHitCollection* hcalcoll = hdl_HcalHitColl[t2]->get();
            for (auto hit: *hcalcoll) {
                _HcalTotalE += hit.getEnergy();
            }
        }

    }catch(lcio::DataNotAvailableException err) { }

    try{
        auto MCPCol = m_mcParticle.get();

        for (int s0 = 0; s0 < MCPCol->size(); ++s0) {
            auto MCP = (*MCPCol)[s0];

            mc_energy = MCP.getEnergy();

            _outputMC->Fill();
        }

    }catch(lcio::DataNotAvailableException err) { }

    try{
        auto col_RecoNeP = m_reconep.get();
        for(int i0 = 0; i0 < col_RecoNeP->size(); i0++) {
            auto a_RecoP = (*col_RecoNeP)[i0];
            TLorentzVector currP( a_RecoP.getMomentum()[0], a_RecoP.getMomentum()[1], a_RecoP.getMomentum()[2], a_RecoP.getEnergy());
            ArborTotalP += currP;

            pfo_a_energy = a_RecoP.getEnergy();

            _outputPFO_a->Fill();

        }
    }catch (lcio::DataNotAvailableException err) { }

    try{
        //LCCollection* col_RecoPandora = evtP->getCollection( "PandoraPFOs" );
        //for(int i2 = 0; i2 < col_RecoPandora->getNumberOfElements(); i2++)
        for(int s = 0; s < 1; s++) {
            auto col_PFO_iter = m_arbopfo.get();

            for(int i2 = 0; i2 < col_PFO_iter->size(); i2++) {
                auto a_RecoP = (*col_PFO_iter)[i2];
                TLorentzVector currP( a_RecoP.getMomentum()[0], a_RecoP.getMomentum()[1], a_RecoP.getMomentum()[2], a_RecoP.getEnergy());
                PandoraTotalP += currP;

                pfo_p_energy = a_RecoP.getEnergy();

                _outputPFO_p->Fill();

            }
        }
    }catch (lcio::DataNotAvailableException err) { }

    _Mass_a = 0;
    _Mass_p = 0;

    _Mass_a = ArborTotalP.M();
    _Mass_p = PandoraTotalP.M();

    info()<<_Mass_a<<" : "<<_Mass_p<<endmsg;

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



