#include "MyProcessor.hh"

DECLARE_COMPONENT(MyProcessor)

MyProcessor::MyProcessor(const std::string& name, ISvcLocator* svcLoc)
    : GaudiAlgorithm(name, svcLoc),
      _output(0)
{

}

StatusCode MyProcessor::initialize() {

    info() << "MyProcessor::initializing..." << endmsg;

    _Number = 0;

    TFile *tree_file=new TFile(_treeFileName.value().c_str(),(_overwrite ? "RECREATE" : "UPDATE"));

    if (!tree_file->IsOpen()) {
        delete tree_file;
        tree_file=new TFile(_treeFileName.value().c_str(),"NEW");
    }

    _outputTree = new TTree(_treeName.value().c_str(),_treeName.value().c_str());
    _outputTree->SetAutoSave(32*1024*1024);  // autosave every 32MB
    _outputTree->Branch("mass_cyberpfo", &_mass_cyberpfo, "mass_cyberpfo/F");

    info() << "MyProcessor::initializd" << endmsg;

    return GaudiAlgorithm::initialize();

}

StatusCode MyProcessor::execute()
{
    info() << "MyProcessor::executing..." << endmsg;

    std::cout << "EVENT ::: "<< _Number << std::endl;

    TLorentzVector CyberPFOTotal(0, 0, 0, 0);

    try{

        auto col_Reco = m_reco.get();
        for(int i0 = 0; i0 < col_Reco->size(); i0++) {
            auto a_RecoP = (*col_Reco)[i0];
            TLorentzVector currP( a_RecoP.getMomentum()[0], a_RecoP.getMomentum()[1], a_RecoP.getMomentum()[2], a_RecoP.getEnergy());
            CyberPFOTotal += currP;
        }

    }catch(lcio::DataNotAvailableException err) { }

    _mass_cyberpfo = 0.0;
    _mass_cyberpfo = CyberPFOTotal.M();

    info() << "_mass_cyberpfo ::::: " << _mass_cyberpfo << endmsg;

    _outputTree->Fill();

    _Number += 1;

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



