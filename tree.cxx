//
// Created by Filippo Valle on 05/11/2018.
//

#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TClonesArray.h"
#include "TRandomPlus.h"
#include "Riostream.h"
#endif

using namespace std;

class myDot : public TObject {
public:
    myDot(){
        fx = 0;
        fy = 0;
        fz = 0;
    };

    myDot(double x, double y, double z) {
        fx = x;
        fy = y;
        fz = z;
    };

    friend ostream& operator<<(ostream &os, const myDot &dot){
        os<<dot.fx<<" "<<dot.fy<<" "<<dot.fz<<endl;
        return os;
    }

private:
    double fx;
    double fy;
    double fz;
};

typedef struct{
    double x;
    double y;
    double z;
    int mult;
} VTX; //vertice

void tree(){
    TFile file("tree.root", "RECREATE");
    auto tree = new TTree("T", "tree");

    auto ptrhits = new TClonesArray("myDot", 100);
    TClonesArray& hits = *ptrhits;

    static VTX point; //sempre lo stesso anche se richiamo funzione

    tree->Branch("Vtx", &point.x, "x/D:y:z:mult/I");
    tree->Branch("Hits", &ptrhits);

    for (int i = 0; i < 10000; ++i) {
        int numPart = 0;
        do {
            numPart = static_cast<int>(0.5*gRandom->Gaus(50, 20));
        }while (numPart < 0);
        point.mult = numPart;
        point.x = gRandom->Gaus(0., 0.01);
        point.y = gRandom->Gaus(0., 0.01);
        point.z = gRandom->Gaus(0., 5.3);

        for (int particle = 0; particle < numPart; ++particle) {
            new(hits[particle])myDot(gRandom->Rndm(), gRandom->Rndm(), gRandom->Rndm());
            //placement operator crea oggetto senza allocare memoria
        }

        tree->Fill();
        ptrhits->Clear();
    }

    tree->Print();

    file.Write();
    file.Close();
}

void readtree(){
    auto hits = new TClonesArray("myDot", 100);
    static VTX point;

    TFile file("tree.root", "READ");
    auto tree = dynamic_cast<TTree*>(file.Get("T"));
    TBranch *b1 = tree->GetBranch("Vtx");
    TBranch *b2 = tree->GetBranch("Hits");

    b1->SetAddress(&point.x);
    b2->SetAddress(&hits);
    for (int i = 0; i < tree->GetEntries(); ++i) {
        tree->GetEvent(i);
        cout<<point.x<<" "<<point.y<<" "<<point.z<<endl;
        for (int particle = 0; particle < hits->GetEntries(); ++particle) {
            auto dot = (myDot*)hits->At(particle);
            cout<<*dot<<endl;
        }
    }


    file.Close();
}