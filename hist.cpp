//
// Created by Filippo Valle on 26/09/2018.
//


#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <TH1.h>
#include <TFile.h>
#include <TCanvas.h>
#include <limits>
#endif

using namespace std;

void io(const char * inFilename, const char* histname, int nbins = 100){
    ifstream infile(inFilename);
    auto file = new TFile("output.root", "UPDATE");


    if(!infile){
        cerr<<"File "<< inFilename <<" not found"<<endl;
        return;
    }

    std::vector<double> X;
    double x;
    uint8_t count = 0;

    while(infile>>x){
        X.push_back(x);
        count++;
    }

    if(count==0){
        cerr<<"File is empty"<<endl;
        infile.close();
        return;
    }

    auto max = *max_element(X.begin(),X.end());
    auto min = *min_element(X.begin(),X.end());
    double delta = (max - min) * 1./ 10;

    TH1* hist = new TH1D("hist", histname, nbins, min - delta, max + delta);

    for_each(X.begin(), X.end(), [hist](double &n) {hist->Fill(n);});

    X.clear();

    hist->DrawCopy();

    file->cd();
    hist->Write(inFilename, TFile::kSingleKey | TFile::kOverwrite);

    file->Close();

    infile.close();
}