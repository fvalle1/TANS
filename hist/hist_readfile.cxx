//
// Created by Filippo Valle on 28/09/2018.
//

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <TH1.h>
#include <TFile.h>
#include <TCanvas.h>
#include <limits>
#endif

using namespace std;


void io_file(const char * inFilename, const char* histname, int nbins = 100){
    ifstream infile(inFilename);
    auto file = new TFile("output.root", "UPDATE");

    if(!infile){
        cerr<<"File "<< inFilename <<" not found"<<endl;
        return;
    }

    int count = 0;
    double x;
    double max = - std::numeric_limits<double>::infinity();
    double min = std::numeric_limits<double>::infinity();

    while(infile>>x){
        count++;
        if(x > max) max = x;
        if(x < min) min = x;
    }

    double delta = (max - min) * 1. / 10;

    TH1* hist = new TH1D("hist", histname, nbins, min - delta, max + delta);

    infile.clear();
    infile.seekg(0, ios::beg);

    while(infile>>x){
        hist->Fill(x);
    }

    hist->DrawCopy();

    file->cd();
    hist->Write(inFilename, TFile::kSingleKey | TFile::kOverwrite);

    file->Close();

    infile.close();
}