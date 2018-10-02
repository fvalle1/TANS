//
// Created by Filippo Valle on 26/09/2018.
//


#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <TH1.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TRandom3.h>
#endif

using namespace std;

typedef function<bool(double)> distribution;

const uint32_t seed = 12345;

bool f0208(double x){
    bool dx = (x<0.2) && (x>0);
    bool sx = (x > 0.8) && (x < 1);
    return dx || sx;
}

void central(int N = 5, uint64_t Ndata = 100000, distribution f = f0208){

    vector<double> data;
    data.reserve(Ndata);
    double x;

    gRandom->SetSeed(seed);

    TH1* hist = nullptr;
    hist = new TH1D("Single distribution", "Single disconnected", 100, 0, 1);
    for (uint64_t i = 0; i < Ndata; ++i) {
        x = gRandom->Rndm();
        if(f(x)) {
            data.emplace_back(x);
            hist->Fill(x);
        }else{
            i--;
        }
    }

    auto file = new TFile("central.root", "RECREATE");
    file->cd();
    hist->Write("single_dist", TFile::kSingleKey | TFile::kOverwrite);
    file->Flush();
    hist->Delete();

    hist = nullptr;
    auto fill_sequent = [f](double &n){
        double xi;
        do{
            xi = gRandom->Rndm();
        }while(!f(xi));
        n+=xi;
    };

    for (int terms = 2; terms <= N; ++terms) {
        auto name = Form("Histo_%u",terms);
        for_each(data.begin(), data.end(), fill_sequent);

        double max = *max_element(data.begin(),data.end());
        double min = *min_element(data.begin(), data.end());
        double delta =(max-min)*0.1;
        hist = new TH1D(name, name, 100, min - delta , max + delta);
        for_each(data.begin(), data.end(), [hist](double &n){hist->Fill(n);});

        file->cd();
        hist->Write(name, TFile::kSingleKey | TFile::kOverwrite);
        file->Flush();
        hist->Delete();
        hist = nullptr;
    }

    file->Close();

}