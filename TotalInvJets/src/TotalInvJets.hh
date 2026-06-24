#ifndef _TotalInvJets_hh_
#define _TotalInvJets_hh_

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

class TotalInvJets  : public GaudiAlgorithm
{
public:

    TotalInvJets(const std::string& name, ISvcLocator* svcLoc);

    ~TotalInvJets() {};

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

    Gaudi::Property<std::string> _treeFileName{this,
            "TreeOutputFile", "MCTruth.root",
            "The name of the file to which the ROOT tree will be written"};
    Gaudi::Property<std::string> _treeName{this,
            "TreeName", "MCPart",
            "The name of the ROOT tree"};
    std::string _colName;

    Gaudi::Property<int> _overwrite{this,
            "OverwriteFile", 0,
            "If zero an already existing file will not be overwritten."};
    TTree *_outputTree;

    int _Num;

    std::string _fileName;
    std::ostream *_output;
    std::string _histFileName;
};

#endif


