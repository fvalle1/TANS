//
// Created by Filippo Valle on 11/10/2018.
//

#include "TRandomF.h"

ClassImp(TRandomF)

const string TRandomF::ffilename = "distributions.root";
const string TRandomF::ffuncname = "TF1func";
const string TRandomF::fgname = "fbig";
const string TRandomF::fsimRejname = "frejsim";
const string TRandomF::fsimInvname = "finvsim";
const string TRandomF::fsimNumname = "fnumsim";


TRandomF::TRandomF():TRandom3(),
                     ff(nullptr),
                     ffBig(nullptr),
                     ffinv(nullptr),
                     fTH1func(nullptr),
                     fTH1g(nullptr),
                     fTH1RejSim(nullptr),
                     fTH1InvSim(nullptr),
                     fLegend(nullptr),
                     fxmin(0.),
                     fxmax(1.),
                     fNHits(0),
                     fNBins(0) {

}

TRandomF::TRandomF(const TRandomF &source): TRandom3(source),
                                            fxmin(source.fxmin),
                                            fxmax(source.fxmax),
                                            fNHits(source.fNHits),
                                            fNBins(source.fNBins) {
    if(source.ff){
        ff = new TF1(*source.ff);
    }

    if(source.ffBig){
        ffBig = new TF1(*source.ffBig);
    }

    if(source.ffinv){
        ffinv = new TF1(*source.ffinv);
    }

    if(source.fTH1func){
        fTH1func = new TH1D(*(dynamic_cast<TH1D*>(source.fTH1func)));
        fTH1func->SetDirectory(nullptr);
    }

    if(source.fTH1g){
        fTH1g = new TH1D(*(dynamic_cast<TH1D*>(source.fTH1g)));
        fTH1g->SetDirectory(nullptr);
    }

    if(source.fTH1RejSim){
        fTH1RejSim = new TH1D(*(dynamic_cast<TH1D*>(source.fTH1RejSim)));
        fTH1RejSim->SetDirectory(nullptr);
    }

    if(source.fTH1InvSim){
        fTH1InvSim = new TH1D(*(dynamic_cast<TH1D*>(source.fTH1InvSim)));
        fTH1InvSim->SetDirectory(nullptr);
    }

    if(source.fTH1NumSim){
        fTH1NumSim = new TH1D(*(dynamic_cast<TH1D*>(source.fTH1NumSim)));
        fTH1NumSim->SetDirectory(nullptr);
    }

    if(source.fLegend){
        fLegend = new TLegend();
    }
}

TRandomF::TRandomF(TF1 *func, TF1 *funcbig, TF1 *funcinv, double min, double max, uint64_t hits, uint32_t bins)
        : TRandom3(),
          fxmin(min),
          fxmax(max),
          fNHits(hits),
          fNBins(bins){

    ff = new TF1(*func);
    ff->SetNpx(bins);
    ffBig = new TF1(*funcbig);
    ffBig->SetNpx(bins);
    ffinv = new TF1(*funcinv);
    ffinv->SetNpx(bins);

    auto delta = static_cast<double>(TMath::Abs(fxmax - fxmin) / fNBins);

    fTH1func = new TH1D(ffuncname.c_str(), "function", fNBins, fxmin, fxmax);
    fTH1g = new TH1D(fgname.c_str(), "fBig", fNBins, fxmin, fxmax);
    fTH1RejSim = new TH1D(fsimRejname.c_str(), "fSimulation_rejection", fNBins, fxmin , fxmax);
    fTH1InvSim = new TH1D(fsimInvname.c_str(), "fSimulation_inversion", fNBins, fxmin , fxmax);
    fTH1NumSim = new TH1D(fsimNumname.c_str(), "fSimulation_GetRandom", fNBins, fxmin , fxmax);

    fLegend = new TLegend();
    fLegend->AddEntry(fTH1func, "function");
    fLegend->AddEntry(fTH1g, "fbig");
    fLegend->AddEntry(fTH1RejSim, "simulation_rejection");
    fLegend->AddEntry(fTH1InvSim, "simulation_inversion");
    fLegend->AddEntry(fTH1NumSim, "simulation_grandom");

    fTH1g->SetDirectory(nullptr);
    fTH1RejSim->SetDirectory(nullptr);
    fTH1func->SetDirectory(nullptr);

    double data = fxmin;
    for(uint32_t bin = 1; bin <= fNBins; bin++){
        fTH1func->SetBinContent(bin, (*ff)(data));
        fTH1g->SetBinContent(bin, (*ffBig)(data));
        data+=delta;
    }

    fTH1func->SetLineColor(kBlue);
    fTH1g->SetLineColor(kRed);
    fTH1RejSim->SetLineColor(kGreen);
    fTH1InvSim->SetLineColor(kCyan);
    fTH1NumSim->SetLineColor(kYellow);
}


TRandomF::TRandomF(TRandomFProblemConfiguration problem) :
        TRandomF(problem.ff, problem.ffbig, problem.finv, problem.fxmin, problem.fxmax, problem.fNhits, problem.fNbins) {
}


TRandomF::~TRandomF() {
    delete fTH1func;
    delete fTH1g;
    delete fTH1RejSim;
    delete fTH1InvSim;
    delete fTH1NumSim;
    delete ff;
    delete ffBig;
    delete ffinv;
    delete fLegend;
}


TRandomF &TRandomF::operator=(const TRandomF &source) {
    if(this == &source) return *this;
    this->~TRandomF();
    new(this) TRandomF(source);
    return *this;
}


TRandomF& TRandomF::DoRejectionAnalysis() {
    printf("\nDoing rejection analysis");
    StopWatch stopwatch;
    for (uint64_t hit = 0; hit < fNHits; hit++){
        fTH1RejSim->Fill(GetByRejection());
    }
    fTH1RejSim->Scale(fTH1func->Integral("width") / fTH1RejSim->GetEntries(), "width");
    return (*this);
}


TRandomF& TRandomF::DoInversionAnalysis() {
    printf("\nDoing inversion analysis");
    StopWatch stopwatch;
    for (uint64_t hit = 0; hit < fNHits; hit++){
        fTH1InvSim->Fill(GetByInversion());
    }
    fTH1InvSim->Scale(fTH1func->Integral("width") / fTH1InvSim->GetEntries(), "width");
    return (*this);
};

TRandomF& TRandomF::DoInversionNumericAnalysis(){
    printf("\nDoing numeric analysis");
    StopWatch stopwatch;
    for (uint64_t hit = 0; hit < fNHits; hit++){
        fTH1NumSim->Fill(ff->GetRandom());
    }
    fTH1NumSim->Scale(fTH1func->Integral("width") / fTH1NumSim->GetEntries(), "width");
    return (*this);
};

TRandomF& TRandomF::Save(Option_t *option){
    auto file = new TFile(ffilename.c_str(), "UPDATE");
    fTH1func->Write();
    fTH1g->Write();
    fTH1RejSim->Write();
    fTH1InvSim->Write();
    fTH1NumSim->Write();
    fLegend->Write("leg");

    file->Close();
    delete file;

    return(*this);
}


TRandomF &TRandomF::Show(TCanvas *canvas){
    auto file = new TFile(ffilename.c_str(), "READ");

    auto hfunc = dynamic_cast<TH1D*>(file->Get(ffuncname.c_str()));
    auto hg = dynamic_cast<TH1D*>(file->Get(fgname.c_str()));
    auto hrej = dynamic_cast<TH1D*>(file->Get(fsimRejname.c_str()));
    auto hinv = dynamic_cast<TH1D*>(file->Get(fsimInvname.c_str()));
    auto hnum = dynamic_cast<TH1D*>(file->Get(fsimNumname.c_str()));
    auto leg = (TLegend*) file->Get("leg");

    if(hfunc && hg && hrej && hinv && hnum && leg) {

        hfunc->SetDirectory(nullptr);
        hg->SetDirectory(nullptr);
        hrej->SetDirectory(nullptr);
        hinv->SetDirectory(nullptr);
        hnum->SetDirectory(nullptr);

        file->Close();
        delete file;

        canvas->cd();
        canvas->Clear();
        canvas->SetTitle("Distributions");
        canvas->SetName("distributions");

        hfunc->Draw("hist c");
        hg->Draw("hist c same");
        hrej->Draw("hist c same");
        hinv->Draw("hist c same");
        hnum->Draw("hist c same");
        leg->Draw("same");

    }else{
        cerr<<"File has no objects: call Save()"<<endl;
    }
    return (*this);
}


double TRandomF::GetByRejection(){
    auto *X = new double[2];
    double u, x;
    do{
        RndmArray(2, X);
        x = fxmin + (fxmax -  fxmin) * X[0];
        u = (*ffBig)(x) * X[1];
    }while((*ff)(x) <= u);
    delete[] X;
    return x;
}

double TRandomF::GetByInversion(){
    auto *X = new double[2];
    double w, x;
    RndmArray(2, X);
    w = X[1];
    x = (*ffinv)(X[0]);
    if(w < 0.5){
        x+=TMath::Pi();
    }else{
        x+=TMath::TwoPi();
    }
    delete[] X;
    return x;
}