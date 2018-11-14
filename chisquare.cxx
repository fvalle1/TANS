//
// Created by Filippo Valle on 29/10/2018.
//
#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <TStyle.h>
#include <TString.h>
#include <TMath.h>
#include <TH1D.h>
#include <TF1.h>
#include <TRandom3.h>
#include <TCanvas.h>
#include "StopWatch.h"
#include <TFile.h>
#include <TLine.h>
#endif

using std::cout;
using std::endl;

double background(double x){
    return 0.1690*(10-0.01*x);
}

void chisquare(uint64_t Nexperiments = 100000){
    StopWatch stopwatch;
    //gStyle->SetOptStat(00000000);
    int NDF = 12;
    int xmin = 0;
    int xmax = 24;
    int chimax = 80;
    auto data = new TH1D("data", "data", NDF, xmin, xmax);
    data->SetFillColor(kOrange);
    int realData[] = {1, 3, 1, 4, 3, 5, 2, 1, 1, 0, 1, 3};
    double realChi = 0.;
    for (int j = 0; j < 12; ++j) {
        double x = 2 * j + 1;
        double v = background(x);
        data->Fill(x, realData[j]);
        realChi+=(realData[j]-v)*(realData[j]-v)/v;
    }

    cout<<"chi square: "<<realChi<<endl;

    auto back = new TF1("back", [](double *x, double* par){return background(x[0]);}, xmin, xmax, 0);
    auto chihist = new TH1D("chi", "chi", 100, 0, chimax);
    chihist->SetMarkerSize(1.5);
    chihist->SetMarkerStyle(20);
    auto chisquaredist = [](double* xx, double* par){
        auto x = xx[0];
        double k = par[1];
        double ret = TMath::Power(x, k/2. - 1);
        ret*=TMath::Exp(-x/2. - TMath::LnGamma(k/2.));
        ret*=TMath::Power(2, - k/2.);
        return ret*par[0];
    };
    auto chiteo = new TF1("chiteo", chisquaredist, 0, chimax, 2);
    chiteo->SetParameter(0, 1);
    chiteo->SetParameter(1, NDF);
    chihist->SetMarkerColor(kCyan);
    chihist->SetMarkerSize(0.5);

    double norm = 0;
    for (uint64_t experiment = 0; experiment < Nexperiments; ++experiment) {
        double chi = 0.;
        for (int i = 1; i < NDF; ++i) {
            double vi = background(i);
            double ni = gRandom->Poisson(vi);
            chi += (ni - vi) * (ni - vi) / vi;
        }
        chihist->Fill(chi);
    }

    norm = chihist->Integral("width");
    chiteo->SetParameter(0, norm);
    auto chiline = new TLine(realChi, 0, realChi, 1.1*chiteo->GetMaximum());
    chiline->SetLineColor(kOrange);
    chiline->SetLineWidth(4);

    auto cx = new TCanvas();
    cx->Divide(3,1);
    cx->cd(1);
    data->Draw("hist");
    back->Draw("same");

    cx->cd(2);
    chihist->Draw("pe");
    chiteo->Draw("same");
    chiline->Draw("same");


    cx->cd(3)->SetLogy();
    chihist->Draw("pe");
    chiteo->Draw("same");
    chiline->Draw("same");
//    new TCanvas();
//    chiteo->SetParameter(0, 1);
//    chiteo->SetParameter(1, 4);
//    chiteo->Draw("same");
}