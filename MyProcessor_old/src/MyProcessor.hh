#ifndef _MyProcessor_hh_
#define _MyProcessor_hh_

#include "GaudiAlg/GaudiAlgorithm.h"
#include <Gaudi/Property.h>

#include "k4FWCore/DataHandle.h"

#include <string>
#include <iostream>
#include <fstream>
#include <TNtuple.h>
#include <TObject.h>
#include <TTree.h>
#include <TFile.h>

#include "edm4hep/MCParticleCollection.h"
#include "edm4hep/CalorimeterHitCollection.h"
#include "edm4hep/ReconstructedParticleCollection.h"

class MyProcessor  : public GaudiAlgorithm
{
public:

    MyProcessor(const std::string& name, ISvcLocator* svcLoc);

    ~MyProcessor() {};

    StatusCode initialize() override;

    StatusCode execute() override;

    StatusCode finalize() override;

protected:
    typedef DataHandle<edm4hep::MCParticleCollection> MCParticleColHandler;
    // MCParticleColHandler m_mcParticle{"MCParticle", Gaudi::DataHandle::Reader, this};
    // MCParticleColHandler m_mcParticle{"MCParticleGen", Gaudi::DataHandle::Reader, this};
    Gaudi::Property<std::string> _MCPCollectionName{this,
            "MCPCollectionName", "MCParticle",
            "MCPCollectionName"};

    typedef DataHandle<edm4hep::CalorimeterHitCollection> CaloHitColHandler;
    CaloHitColHandler m_ecalbarrelhitcol{"ECALBarrel", Gaudi::DataHandle::Reader, this};
    CaloHitColHandler m_ecalendcaphitcol{"ECALEndcap", Gaudi::DataHandle::Reader, this};

    CaloHitColHandler m_hcalbarrelhitcol{"HCALBarrel", Gaudi::DataHandle::Reader, this};
    CaloHitColHandler m_hcalendcaphitcol{"HCALEndcap", Gaudi::DataHandle::Reader, this};
    CaloHitColHandler m_hcalotherhitcol {"HCALOther", Gaudi::DataHandle::Reader, this};

    typedef DataHandle<edm4hep::ReconstructedParticleCollection> RecParticleColHandler;
    RecParticleColHandler m_reconep{"ArborPFO", Gaudi::DataHandle::Reader, this};
    RecParticleColHandler m_arbopfo{"ArborPFO", Gaudi::DataHandle::Reader, this};


    Gaudi::Property<std::string> _treeFileName{this,
            "TreeOutputFile", "MCTruth.root",
            "The name of the file to which the ROOT tree will be written"};
    Gaudi::Property<std::string> _treeName{this,
            "TreeName", "Tree",
            "The name of the ROOT tree"};
    std::string _colName;

    Gaudi::Property<int> _overwrite{this,
            "OverwriteFile", 0,
            "If zero an already existing file will not be overwritten."};
    TTree *_outputTree, *_outputMC, *_outputPFO_a, *_outputPFO_p;

    int _Num;

    float _EcalTotalE, _HcalTotalE;
    float _Mass_a, _Mass_p;
    float mc_energy;
    float pfo_a_energy, pfo_p_energy;

    std::string _fileName;
    std::ostream *_output;
    std::string _histFileName;
};

#endif


