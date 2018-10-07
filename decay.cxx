//
// Created by Filippo Valle on 05/10/2018.
//

//
// Created by Filippo Valle on 26/09/2018.
//

#ifndef TANS_CENTRAL
#define TANS_CENTRAL
#endif



#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <TH1.h>
#include <TF1.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TRandom3.h>
#include "StopWatch.h"
#endif

#if defined(_OPENMP)
#include <omp.h>
#endif

using namespace std;

void Decay(uint64_t N = 10000, double alpha = 0.01, double dT =1., double T = 300, bool fit = kFALSE){
    StopWatch stopwatch;
    gRandom->SetSeed(42);
    auto Ntot = N;
    int nbins = static_cast<int>(T / dT + 1);
    int currentbin = 1;
    TH1* hist = new TH1D("decay", "decay", nbins, -dT/2., T + dT/2.);
    hist->SetFillColor(kCyan);
    auto canv = new TCanvas("Decay", "decay", 900, 500);
    canv->Divide(2, 1);

    for(int t = 0; t <= T / dT; t++){
        uint64_t Ndec = 0;
        for(uint64_t n = 0; n < Ntot; n++) if(gRandom->Rndm() < alpha * dT) Ndec++;
        hist->SetBinContent(currentbin++, Ndec);
        Ntot-=Ndec;
    }


    canv->cd(1);
    hist->Draw();
    auto lin = new TF1("fitlin", "[0]*exp(-[1]*x)", 0, T);
    lin->SetParameter(0, N);
    lin->SetParameter(1, alpha);
    if(fit) {
        hist->Fit(lin, "R+");
        lin->Draw("SAME");
    }

    canv->cd(2);
    canv->cd(2)->SetLogy();
    hist->Draw();

    auto file = new TFile("decay.root","RECREATE");

    hist->Write();

    file->Close();

}