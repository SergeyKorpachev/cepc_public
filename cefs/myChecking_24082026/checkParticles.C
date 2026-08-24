#include <TFile.h>
#include <TTree.h>
#include <vector>
#include <iostream>
#include <map>

using namespace std;

void countParticles(const char* filename) {
    cout << "\n========================================" << endl;
    cout << "Анализ файла: " << filename << endl;
    cout << "========================================" << endl;
    
    TFile *f = TFile::Open(filename);
    if (!f || f->IsZombie()) {
        cerr << "ERROR: Не могу открыть файл " << filename << endl;
        return;
    }
    
    TTree *tree = (TTree*)f->Get("outputTree");
    if (!tree || tree->IsZombie()) {
        cerr << "ERROR: Не могу найти дерево outputTree" << endl;
        f->Close();
        return;
    }
    
    vector<int> *types = nullptr;
    vector<double> *energy = nullptr;
    
    tree->SetBranchAddress("particleType", &types);
    tree->SetBranchAddress("pfoE", &energy);
    
    long int evnum = tree->GetEntries();
    cout << "Всего событий: " << evnum << endl;
    
    long long nElectrons = 0;
    long long nMuons = 0;
    long long nPhotons = 0;
    long long nOther = 0;
    long long totalPFO = 0;
    
    long long eventsWithElectrons = 0;
    long long eventsWithMuons = 0;
    long long eventsWithBoth = 0;
    
    map<int, long long> typeCounter;
    
    for (int i = 0; i < evnum; ++i) {
        tree->GetEntry(i);
        
        bool hasElectron = false;
        bool hasMuon = false;
        
        for (int j = 0; j < energy->size(); ++j) {
            int type = types->at(j);
            totalPFO++;
            
            typeCounter[type]++;
            
            if (type == 11) {
                nElectrons++;
                hasElectron = true;
            } else if (type == 13) {
                nMuons++;
                hasMuon = true;
            } else if (type == 22) {
                nPhotons++;
            } else {
                nOther++;
            }
        }
        
        if (hasElectron) eventsWithElectrons++;
        if (hasMuon) eventsWithMuons++;
        if (hasElectron && hasMuon) eventsWithBoth++;
    }
    
    cout << "\n--- ОБЩАЯ СТАТИСТИКА ПО PFO ---" << endl;
    cout << "Всего PFO объектов: " << totalPFO << endl;
    cout << "Электроны (PDG 11): " << nElectrons << endl;
    cout << "Мюоны (PDG 13): " << nMuons << endl;
    cout << "Фотоны (PDG 22): " << nPhotons << endl;
    cout << "Другие частицы: " << nOther << endl;
    
    cout << "\n--- СТАТИСТИКА ПО СОБЫТИЯМ ---" << endl;
    cout << "Событий с электронами: " << eventsWithElectrons << endl;
    cout << "Событий с мюонами: " << eventsWithMuons << endl;
    cout << "Событий с обоими: " << eventsWithBoth << endl;
    
    cout << "\n--- ВСЕ УНИКАЛЬНЫЕ ТИПЫ В ФАЙЛЕ ---" << endl;
    cout << "Тип (type) : Количество" << endl;
    for (const auto& pair : typeCounter) {
        cout << "  " << pair.first << " : " << pair.second << endl;
    }
    
    f->Close();
    cout << "\nАнализ завершён." << endl;
}
