//
// How to run:
// root -l -b -q 'debug.C("Reconstruction_e-_45gev_tdr25.6.0_TDR_o1_v01_1000evt_seed1.root", "Reconstruction_slim_e-_45gev_tdr25.6.0_TDR_o1_v01_1000evt_seed1.root")'
//

// for analysis: events->Draw("CyberPFO.energy", "CyberPFO.energy < 1e6 && CyberPFO.energy > -1e6");

void debug(const char* originalFile, const char* slimFile) {

    TFile* orig = TFile::Open(originalFile);
    TTree* origTree = (TTree*)orig->Get("events");

    for (int i = 0; i < 100; i++) {
        origTree->Draw("CyberPFO.energy", "", "goff", 1, i);
        int n = origTree->GetSelectedRows();
        double* e = origTree->GetV1();

        for (int j = 0; j < n; j++) {
            if (std::isinf(e[j]) || std::isnan(e[j])) {
                cout << "Event " << i << ", object " << j
                     << ": energy = " << e[j] << "\n";
            }
            if (e[j] > 1e6) {
                cout << "Event " << i << ", object " << j
                     << ": VERY GREAT energy = " << e[j] << " GeV\n";
            }
        }
    }

    orig->Close();
}
