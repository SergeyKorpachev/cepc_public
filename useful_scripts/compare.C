//
// How to run:
// root -l -b -q 'compare.C("Reconstruction_e-_45gev_tdr25.6.0_TDR_o1_v01_1000evt_seed1.root", "Reconstruction_slim_e-_45gev_tdr25.6.0_TDR_o1_v01_1000evt_seed1.root")'
//

#include <iostream>
#include <cmath>

void compare(const char* originalFile, const char* slimFile) {

    TFile* orig = TFile::Open(originalFile);
    TTree* origTree = (TTree*)orig->Get("events");

    cout << "1. Original:\n";
    cout << "Total events: " << origTree->GetEntries() << "\n";

    cout << "First 5 events (energies):\n";
    for (int i = 0; i < 5; i++) {
        origTree->Draw("CyberPFO.energy", "", "goff", 1, i);
        int n = origTree->GetSelectedRows();
        double* energies = origTree->GetV1();

        cout << "   Event " << i << ": " << n << " objects, energy: ";
        for (int j = 0; j < n && j < 5; j++) {
            cout << energies[j] << " ";
        }
        cout << "\n";
    }

    TFile* slim = TFile::Open(slimFile);
    TTree* slimTree = (TTree*)slim->Get("events");

    cout << "\n2. Slim file:\n";
    cout << "Total events: " << slimTree->GetEntries() << "\n";

    cout << "First 5 events (energies):\n";
    for (int i = 0; i < 5; i++) {
        slimTree->Draw("CyberPFO.energy", "", "goff", 1, i);
        int n = slimTree->GetSelectedRows();
        double* energies = slimTree->GetV1();

        cout << "   Event " << i << ": " << n << " objects, energy: ";
        for (int j = 0; j < n && j < 5; j++) {
            cout << energies[j] << " ";
        }
        cout << "\n";
    }

    cout << "\n3. Statistics comparison (first 100 events):\n";

    double origTotalE = 0;
    double slimTotalE = 0;
    int origTotalObj = 0;
    int slimTotalObj = 0;

    for (int i = 0; i < 100; i++) {
        origTree->Draw("CyberPFO.energy", "", "goff", 1, i);
        int nOrig = origTree->GetSelectedRows();
        origTotalObj += nOrig;
        double* eOrig = origTree->GetV1();
        for (int j = 0; j < nOrig; j++) {
            if (!std::isnan(eOrig[j])) {
                origTotalE += eOrig[j];
            }
        }

        slimTree->Draw("CyberPFO.energy", "", "goff", 1, i);
        int nSlim = slimTree->GetSelectedRows();
        slimTotalObj += nSlim;
        double* eSlim = slimTree->GetV1();
        for (int j = 0; j < nSlim; j++) {
            if (!std::isnan(eSlim[j])) {
                slimTotalE += eSlim[j];
            }
        }
    }

    cout << "   Total records (first 100 events):\n";
    cout << "      Original: " << origTotalObj << "\n";
    cout << "      Slim:     " << slimTotalObj << "\n";

    cout << "   Total energy (first 100 events):\n";
    cout << "      Original: " << origTotalE << " GeV\n";
    cout << "      Slim:     " << slimTotalE << " GeV\n";
    cout << "      Difference:  " << (slimTotalE - origTotalE) << " GeV\n";

    if (origTotalObj == slimTotalObj && fabs(slimTotalE - origTotalE) < 0.1) {
        cout << "\nData is completely identical! The file was copied correctly.\n";
    } else {
        cout << "\nThere are discrepancies in numbers, but the structure is preserved.\n";
    }

    orig->Close();
    slim->Close();
}
