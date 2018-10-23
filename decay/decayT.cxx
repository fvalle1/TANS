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
#include <TLegend.h>
#include <TCanvas.h>
#include <TRandom3.h>
#include "StopWatch.h"
#include "TMath.h"
#endif

#if defined(_OPENMP)
#include <omp.h>
#endif

using namespace std;

uint64_t SingleDecay(uint64_t N, double alpha, double dT, double T){
    auto Ntot = N;
    int nbins = static_cast<int>(T / dT + 1);
    int currentbin = 1;

    for(int t = 0; t <= T / dT; t++){
        uint64_t Ndec = 0;
        for(uint64_t n = 0; n < Ntot; n++) if(gRandom->Rndm() < alpha * dT) Ndec++;
        Ntot-=Ndec;
    }

    return N-Ntot;
}

double LnFactorial(double x){
    return TMath::LnGamma(x + 1);
}

double poisson(const double x, const double mu, const double norm){
    return norm * TMath::Power(mu, x) * TMath::Exp(- mu - LnFactorial(x));
}

double poisson(const double *xx, const double *params){
    double x = xx[0];
    double mu = params[0];
    double N = params[1];
    return poisson(x, mu, N);
}

double binomial(const double k, double N, double p, const double norm = 1){
    return norm * TMath::Exp(LnFactorial(N) - LnFactorial(N - k) - LnFactorial(k)) * TMath::Power(p, k) * TMath::Power(1 - p, N - k);
}

double binomial(const double *xx, const double *params) {
    double k = xx[0];
    double N = params[1];
    double p = params[2];
    double norm = params[0];
    return binomial(k, N, p, norm);
}

void DecayT(uint64_t Nexperiments = 1000, uint64_t N = 1000, double alpha = 0.000025, double dT =1., double T = 100){
    StopWatch stopwatch;
    double mu = alpha * N * T;
    int max = static_cast<int>(4 * mu);
    auto hist = new TH1D("hist_T", "nDec", max + 1, -0.5, max + 0.5);
    hist->SetFillColor(kCyan);

    auto pois = new TF1("poisson", poisson, 0, max, 2);
    pois->SetParameters(mu, Nexperiments);
    pois->SetParNames("mu", "norm");
    pois->SetLineColor(kViolet);

    auto binom = new TF1("binomial", binomial, 0, max, 3);
    binom->SetParameters(Nexperiments, T/dT, alpha * N * dT);
    binom->SetParNames("norm", "m", "p");
    binom->SetLineColor(kGreen);

    auto binom_h = new TH1D("binomial_h", "binomial_h", max + 1, -0.5, max+0.5);
    binom_h->SetLineColor(kGreen + 3);

    auto pois_h = new TH1D("pois_h", "pois_h", max + 1, -0.5, max+0.5);
    pois_h->SetLineColor(kViolet + 3);

    cout<<"\nBegin\n"<<endl;
#pragma omp parallel for shared(hist, binom_h, pois_h)
    for (uint64_t experiment = 0; experiment < Nexperiments; ++experiment) {
        hist->Fill(SingleDecay(N, alpha, dT, T));
        binom_h->Fill(experiment, binomial(experiment, T/dT, alpha * N * dT, Nexperiments));
        pois_h->Fill(experiment, poisson(experiment, mu, Nexperiments));
#if defined(_OPENMP)
if(omp_get_thread_num() == 0)
#endif
        cout<<"\rRunning experiment: "<<experiment;
    }

    cout<<"\n Saving file"<<endl;
    auto file = new TFile("decay.root", "UPDATE");
    auto dir = Form("N%llu_alpha%1.7f_dt%1.2f_T%5.1f",N, alpha, dT, T);
    file->mkdir(dir, dir);
    file->cd(dir);
    hist->Write("hist_t", TFile::kSingleKey | TFile::kOverwrite);
    pois->Write("pois", TFile::kSingleKey | TFile::kOverwrite);
    binom->Write("binom", TFile::kSingleKey | TFile::kOverwrite);
    binom_h->Write("binom_h", TFile::kSingleKey | TFile::kOverwrite);
    pois_h->Write("pois_h", TFile::kSingleKey | TFile::kOverwrite);

    file->Close();
    delete hist;
    delete binom;
    delete pois;
    delete binom_h;
    delete pois_h;
    delete file;
}


enum classes{
    kTF1,
    kTH1
};

void DecayTDraw(classes cl = kTH1, uint64_t N = 1000, double alpha = 0.000025, double dT =1., double T = 100){
    auto file = new TFile("decay.root", "READ");
    auto dir = Form("N%llu_alpha%1.7f_dt%1.2f_T%5.1f",N, alpha, dT, T);
    TH1* hist = dynamic_cast<TH1*>(file->Get(Form("%s/hist_t",dir)));
    if(!hist){
        cerr<<"Data not found\n";
        return;
    }
    TObject *poisson, *binom;
    if(cl == 0) {
        poisson = (TF1 *) file->Get(Form("%s/pois", dir));
        binom = (TF1 *) file->Get(Form("%s/binom", dir));
    }else{
        poisson = (TH1 *) file->Get(Form("%s/pois_h", dir));
        binom = (TH1 *) file->Get(Form("%s/binom_h", dir));
        dynamic_cast<TH1*>(poisson)->SetDirectory(0);
        dynamic_cast<TH1*>(binom)->SetDirectory(0);
    }
    hist->SetDirectory(0);
    file->Close();
    delete file;

    auto leg = new TLegend(0.6, 0.5, 0.9, 0.7);
    leg->AddEntry(hist,"N_dec");
    leg->AddEntry(poisson, "poisson");
    leg->AddEntry(binom, "binomial");

    auto canv = new TCanvas("decay", "Decay", 900, 500);
    hist->Draw("hist");
    binom->Draw("hist same c");
    poisson->Draw("hist same c");
    leg->Draw("same");
}
