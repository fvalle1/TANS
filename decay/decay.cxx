//
// Created by Filippo Valle on 05/10/2018.
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
#include "TMath.h"
#endif

#if defined(_OPENMP)
#include <omp.h>
#endif

using namespace std;

double exponential(double *x, double *par){
    return par[0]*TMath::Exp(-par[1]*x[0]);
}

void Decay(uint64_t N = 10000, double alpha = 0.01, double dT =1., double T = 300){
    StopWatch stopwatch;
    gRandom->SetSeed(42);
    auto Ntot = N;
    int nbins = static_cast<int>(T / dT + 1);
    int currentbin = 1;
    auto fteo = new TF1("decay_teo", exponential, 0, T, 2); //2 è #parametri
    fteo->SetLineColor(kViolet-2);
    fteo->SetParameters(N, alpha);
    fteo->SetParNames("norm", "alpha");
    TH1* hist = new TH1D("decay", "decay", nbins, -dT/2., T + dT/2.);
    hist->SetFillColor(kCyan);
    auto canv = new TCanvas("decay", "Decay", 900, 500);
    canv->Divide(2, 1);

    for(int t = 0; t <= T / dT; t++){
        uint64_t Ndec = 0;
        for(uint64_t n = 0; n < Ntot; n++) if(gRandom->Rndm() < alpha * dT) Ndec++;
        hist->SetBinContent(currentbin++, Ntot);
        Ntot-=Ndec;
    }


    canv->cd(1);
    hist->Draw("histo");
    fteo->Draw("same");


    canv->cd(2)->SetLogy();
    hist->Draw("histo");
    fteo->Draw("same");


    auto file = new TFile("decay.root","RECREATE");

    hist->Write();
    fteo->Write();

    file->Close();

}



void DrawDecay(){
    auto file = new TFile("decay.root","READ");
    TH1* h1 = (TH1*)file->Get("decay");
    TF1* func = (TF1*)file->Get("decay_teo");
    h1->SetDirectory(0); //h1 appartiene al file
    file->Close();
    delete file;

    auto canv = new TCanvas("decay", "Decay", 900, 500);
    canv->Divide(2,1);
    canv->cd(1);
    h1->Draw("histo");
    func->Draw("same");


    canv->cd(2)->SetLogy();
    h1->Draw("histo");
    func->Draw("same");

}