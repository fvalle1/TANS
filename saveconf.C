//
// Created by Filippo Valle on 17/10/2018.
//

#include <TFile.h>
#include <TF1.h>
#include <TString.h>
#include "SimulatorConfiguration.h"

void saveconf(){
//    double a = 0.001;
//    float b = 1000;
//    auto f = new TF1("f", [](double* xx, double* params){return 1./(TMath::Sin(xx[0])*TMath::Sin(xx[0]) + params[0] * TMath::Cos(xx[0]) * TMath::Cos(xx[0]));}, 0, 2 * TMath::Pi(), 1);
//    auto fbig = new TF1("fbig", [](double* x, double* params){return params[0];}, 0, 2 * TMath::Pi(), 1);
//    auto finv = new TF1("finv", [](double* x, double* params){return TMath::ATan(TMath::Sqrt(params[0])*TMath::Tan(TMath::Pi() * x[0] - TMath::PiOver2()));}, 0, 2 * TMath::Pi(), 1);
//    auto fimportance = new TF1("fimp", [](double* x, double* params){return ((x[0] > 1 && x[0] < 2.2) || (x[0] > 4 && x[0] < 5.4)) ? params[1] : params[0];}, 0, TMath::TwoPi(), 2);
//    double* params[4];
//    params[0] = new double[1];
//    params[1] = new double[1];
//    params[2] = new double[1];
//    params[3] = new double[2];
//    params[0][0] = a;
//    params[1][0] = b;
//    params[2][0] = a;
//    params[3][0] = b;
//    params[3][1] = b - 0.7;
//
//    auto config = SimulatorConfiguration(f, fbig, finv, fimportance, b + 0.1, 0, TMath::TwoPi(), Form("a:%5.3f b:%4.2f", a, b), params, 10000000, 150);


    auto f = new TF1("f", [](double* xx, double* params){return (TMath::Sin(xx[0])/xx[0]*TMath::Sin(xx[0])/xx[0]);}, -10, 10, 0);
    auto fbig = new TF1("fbig", [](double* x, double* params){return params[0];}, -10, 10, 1);
    auto fimportance = new TF1("fimp", [](double* x, double* params){return (TMath::Abs(x[0]) < 3.2) ? params[0] : params[1];}, -10, 10, 2);
    double* params[4];
    params[0] = new double[1];
    params[1] = new double[1];
    params[2] = new double[1];
    params[3] = new double[2];
    params[0][0] = 0.;
    params[1][0] = 1.;
    params[2][0] = 0.;
    params[3][0] = 1.;
    params[3][1] = 0.07;

    auto config = SimulatorConfiguration(f, fbig, nullptr, fimportance, 1.01, -10, 10, "sinc", params, 100000);

    auto file = new TFile("config.root", "UPDATE");
    config.Write("sinc");
    file->Close();
    delete file;
}