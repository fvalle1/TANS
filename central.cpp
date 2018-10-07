//
// Created by Filippo Valle on 26/09/2018.
//

#ifndef TANS_CENTRAL
#define TANS_CENTRAL
#endif



#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <initializer_list>
#include <TH1.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TRandom3.h>
#include <TStopwatch.h>
#include "StopWatch.h"
#endif

#if defined(_OPENMP)
#include <omp.h>
#endif

using namespace std;

typedef function<bool(double)> distribution;
typedef function<double()> generator;

const uint32_t seed = 12345;

bool f0208(double x){
    bool dx = (x<0.2) && (x>0);
    bool sx = (x > 0.8) && (x < 1);
    return dx || sx;
}

double RndmGen(){
    double xi = gRandom->Rndm();
    if(xi < 0.5){
        xi = xi / 0.5 * 0.2;
    }else{
        xi = (xi - 0.5) / 0.5 * 0.2 + 0.8;
    }
    return xi;
}

double RndmGenUnif(){
    return gRandom->Uniform();
}

double RndmGenOld(){
    double xi;
    do{
        xi = gRandom->Rndm();
    }while(!f0208(xi));
    return xi;
}

void central(int N = 300, uint64_t Ndata = 10000, const generator &generator = RndmGen){

    auto file = new TFile("central.root", "RECREATE");

#if defined(_OPENMP)
    omp_set_dynamic(0);     // Explicitly disable dynamic teams
    omp_set_num_threads(4);
    // This is needed to allow the presence of a parallel for nested in another parallel region
    omp_set_nested(1);
#endif

    vector<double> data;
    data.reserve(Ndata);
    double x;

    TStopwatch Tstopwatch = TStopwatch();
    StopWatch stopwatch;

    gRandom->SetSeed(seed);

    TH1* hist = nullptr;
    hist = new TH1D("Histo_1", "Single disconnected", 100, 0, 1);
#pragma omp parallel for shared(data)
    for (uint64_t i = 0; i < Ndata; ++i) {
        auto x = generator();
        data.emplace_back(x);
        hist->Fill(x);
    }

    file->cd();
    hist->Write("Histo_1", TFile::kSingleKey | TFile::kOverwrite);
    file->Flush();
    hist->Delete();

    hist = nullptr;

    for (int terms = 2; terms <= N; ++terms) {
        auto name = Form("Histo_%u", terms);
        for_each(data.begin(), data.end(), [generator](double &n){n+=generator();});

        double max = *max_element(data.begin(), data.end());
        double min = *min_element(data.begin(), data.end());
        double delta = (max - min) * 0.1;
        file->cd();

        hist = new TH1D(name, name, 100, min - delta, max + delta);
        for_each(data.begin(), data.end(), [hist](double &n) { hist->Fill(n); });

        hist->Write(name, TFile::kSingleKey | TFile::kOverwrite);
        file->Flush();
        hist->Delete();
        hist = nullptr;
    }

    cout<<"Runned in "<< Tstopwatch.RealTime()<<endl;

    file->Close();

}



void central( std::initializer_list<int> list ){
    auto file = new TFile("central.root", "READ");

    auto histcontainer = new TH1D[list.size()];
    auto canvcontainer = new TCanvas*[list.size()];

    int i = 0;
    for(auto histo : list){
        TH1* h;
        TString histname = Form("Histo_%u", histo);
        file->GetObject(histname, h);
        if(h) {
            histcontainer[i] = TH1D(*((TH1D *) h));
            canvcontainer[i] = new TCanvas(histname, histname);
            canvcontainer[i]->cd();
            histcontainer[i].Draw();
        }
        i++;
    }

    file->Close();
}
