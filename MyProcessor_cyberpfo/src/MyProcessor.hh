#ifndef _MyProcessor_hh_
#define _MyProcessor_hh_

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

using namespace std;

class MyProcessor  : public GaudiAlgorithm
{
public:

    MyProcessor(const std::string& name, ISvcLocator* svcLoc);

    ~MyProcessor() {};

    StatusCode initialize() override;

    StatusCode execute() override;

    StatusCode finalize() override;

protected:
    
    typedef DataHandle<edm4hep::ReconstructedParticleCollection> RecParticleColHandler;
    RecParticleColHandler m_reco{"CyberPFO", Gaudi::DataHandle::Reader, this};

    Gaudi::Property<std::string> _treeFileName{this,
            "TreeOutputFile", "MCTruth.root",
            "The name of the file to which the ROOT tree will be written"};

    Gaudi::Property<std::string> _treeName{this,
            "TreeName", "CyberPFO",
            "The name of the ROOT tree"};

    Gaudi::Property<int> _overwrite{this,
            "OverwriteFile", 0,
            "If zero an already existing file will not be overwritten."};

    TTree *_outputTree;

    float _mass_cyberpfo;

    std::ostream *_output;
};

#endif


