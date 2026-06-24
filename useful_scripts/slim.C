//
// How to run:
// root -l -b -q 'slim.C("Reconstruction_e-_45gev_tdr25.6.0_TDR_o1_v01_1000evt_seed1.root", "Reconstruction_slim_e-_45gev_tdr25.6.0_TDR_o1_v01_1000evt_seed1.root")'
//

void slim(const char* inputFile, const char* outputFile) {

//    gROOT->SetMustClean(false);

    TFile* oldFile = TFile::Open(inputFile);
    if (!oldFile || oldFile->IsZombie()) {
        printf("Error opening file %s\n", inputFile);
        return;
    }

    TTree* oldTree = (TTree*)oldFile->Get("events");
    if (!oldTree) {
        printf("Tree 'events' not found!\n");
        return;
    }

//    oldTree->SetDirectory(0);

    Long64_t nEntries = oldTree->GetEntries();
    printf("Total records: %lld\n", nEntries);

    TFile* newFile = new TFile(outputFile, "RECREATE", "",
                               ROOT::CompressionSettings(ROOT::kLZMA, 9));

    oldTree->SetBranchStatus("*", 0);
    oldTree->SetBranchStatus("CyberPFO*", 1);
    oldTree->SetBranchStatus("_CyberPFO*", 1);

    printf("Included branches:\n");
    oldTree->Print("toponly");

    TTree* newTree = oldTree->CloneTree(0);

    for (Long64_t i = 0; i < nEntries; i++) {
        oldTree->GetEntry(i);
        newTree->Fill();

        if (i % 100 == 0) {
            printf("%lld of %lld records processed\r", i, nEntries);
            fflush(stdout);
        }
    }
    printf("\n%lld records copied\n", newTree->GetEntries());

    newFile->cd();
    newTree->Write();

/*
    TTree* mdTree = (TTree*)oldFile->Get("metadata");
    if (mdTree) {
        mdTree->CloneTree()->Write();
        printf("Metadata copied\n");
    }

    TTree* podio_mdTree = (TTree*)oldFile->Get("podio_metadata");
    if (podio_mdTree) {
        podio_mdTree->CloneTree()->Write();
        printf("PODIO metadata copied\n");
    }

    TTree* configTree = (TTree*)oldFile->Get("configuration_metadata");
    if (configTree) {
        configTree->CloneTree()->Write();
        printf("Configuration copied\n");
    }
*/

    TTree* mdTree = (TTree*)oldFile->Get("metadata");
    if (mdTree) mdTree->CloneTree()->Write();

    TTree* podio_mdTree = (TTree*)oldFile->Get("podio_metadata");
    if (podio_mdTree) podio_mdTree->CloneTree()->Write();

    TTree* configTree = (TTree*)oldFile->Get("configuration_metadata");
    if (configTree) configTree->CloneTree()->Write();

    newFile->Close();
    oldFile->Close();

    printf("Done! File: %s (%.2f MB)\n", outputFile, newFile->GetSize()/1e6);

}
