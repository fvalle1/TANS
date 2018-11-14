#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TRandomPlus.h"
#include <Riostream.h>
#include <cmath>
#include "StopWatch.h"
#include "TCanvas.h"
#include "TH1D.h"
#endif

void hopfield(uint64_t Nexperiments = 100000, uint64_t P = 100, uint64_t N = 500){
    delete gRandom;
    gRandom = new TRandomPlus();
    auto file = new TFile("hopfield.root", "RECREATE");
    auto tree = new TTree("hop", "hoptree");
    auto delta = 2*N*P/100.;
    auto hist = new TH1D("hist", "hist", 101, -N*P - delta/2, N*P + delta/2);
    double gamma;
    tree->Branch("gamma", &gamma);
    StopWatch watch;
    for (uint64_t exp = 0; exp < Nexperiments; ++exp) {
        gamma = 0.;
        double temp = 1.;
        for (uint64_t mu = 0; mu < P; ++mu) {
            for (uint64_t j = 0; j < N-1; ++j) {
                temp+= dynamic_cast<TRandomPlus*>(gRandom)->GetBinary();
            }
        }
        gamma = temp/N;
        tree->Fill();
        hist->Fill(gamma);
    }
    hist->Scale(1./hist->Integral());
    new TCanvas();
    hist->Draw("same");
    hist->SetDirectory(0);
    printf("mean: %d std:%f", 0, sqrt(static_cast<double>(P)/N));
    file->Write();
    file->Close();
//    delete tree;
    delete file;
}