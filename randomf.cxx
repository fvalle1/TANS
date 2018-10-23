//
// Created by Filippo Valle on 11/10/2018.
//

#ifndef TANS__randomF_CXX
#define TANS__randomF_CXX

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <TStyle.h>
#include <TString.h>
#include "Simulator.h"
#include "SimulatorConfiguration.h"
#include <vector>
#include <TMath.h>
#endif


void randomF(float a = 0.5, float b = 2.f, bool log = false, uint64_t hits = 500000, uint32_t bins = 150, int seed = 42){
    gStyle->SetOptStat(0000000);
    auto cx = new TCanvas();
    if(log) cx->SetLogy();

    auto f = new TF1("f", [](double* xx, double* params){return 1./(TMath::Sin(xx[0])*TMath::Sin(xx[0]) + params[0] * TMath::Cos(xx[0]) * TMath::Cos(xx[0]));}, 0, TMath::TwoPi(), 1);
    auto fbig = new TF1("fbig", [](double* x, double* params){return params[0];}, 0, TMath::TwoPi(), 1);
    auto finv = new TF1("finv", [](double* x, double* params){return TMath::ATan(TMath::Sqrt(params[0])*TMath::Tan(TMath::Pi() * x[0] - TMath::PiOver2()));}, 0, TMath::TwoPi(), 1);
    auto fimportance = new TF1("fimp", [](double* x, double* params){return ((x[0] > 1.1 && x[0] < 2.) || (x[0] > 4.1 && x[0] < 5.2)) ? params[1] : params[0];}, 0, TMath::TwoPi(), 2);
    double* params[4];
    params[0] = new double[1];
    params[1] = new double[1];
    params[2] = new double[1];
    params[3] = new double[2];
    params[0][0] = a;
    params[1][0] = b;
    params[2][0] = a;
    params[3][0] = b;
    params[3][1] = (*f)(TMath::PiOver2())*1.6;

    auto config = SimulatorConfiguration(f, fbig, finv, fimportance, b + 0.1, 0, TMath::TwoPi(), Form("a:%5.3f b:%4.2f", a, b), params, hits, bins, seed);


//    auto f = new TF1("f", [](double* xx, double* params){return (TMath::Abs(xx[0]) < 1.e-9 ? 1 : (TMath::Sin(xx[0])/xx[0]*TMath::Sin(xx[0])/xx[0]));}, -10, 10, 0);
//    auto fbig = new TF1("fbig", [](double* x, double* params){return params[0];}, -10, 10, 1);
//    auto fimportance = new TF1("fimp", [](double* x, double* params){return (TMath::Abs(x[0]) < 3.2) ? params[0] : params[1];}, -10, 10, 2);
//    double* params[4];
//    params[0] = new double[1];
//    params[1] = new double[1];
//    params[2] = new double[1];
//    params[3] = new double[2];
//    params[0][0] = 0.;
//    params[1][0] = 1.;
//    params[2][0] = 0.;
//    params[3][0] = 1.;
//    params[3][1] = 0.07;
//
//    auto config = SimulatorConfiguration(f, fbig, nullptr, fimportance, 1.01, -10, 10, Form("sinc"), params);

    cout<<config<<endl;
    Simulator sim(config);
    sim.DoRejectionAnalysis()/*.Save().Show(cx)*/;
    sim.DoRejectionRecursiveAnalysis()/*.Save().Show(cx)*/;
    sim.DoInversionAnalysis()/*.Save().Show(cx)*/;
    sim.DoInversionNumericAnalysis()/*.Save().Show(cx)*/;
    sim.DoImportanceSamplingcAnalysis().Save().Show(cx);

    delete f;
    delete fbig;
    delete fimportance;
    delete params[0];
    delete params[1];
    delete params[2];
    delete params[3];
}


#endif
