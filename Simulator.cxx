//
// Created by Filippo Valle on 11/10/2018.
//

#include "Simulator.h"

ClassImp(Simulator)

const string Simulator::fFilename = "distributions.root";
const string Simulator::fFuncname = "TF1func";
const string Simulator::fBigname = "fbig";
const string Simulator::fImpsamplingName = "fimp";
const string Simulator::fSimRejName = "frejsim";
const string Simulator::fSimRejrecName = "frejrecsim";
const string Simulator::fSimInvName = "finvsim";
const string Simulator::fSimNumName = "fnumsim";
const string Simulator::fSimImpName = "fimpsamplsim";



Simulator::Simulator():TObject(),
                       ff(nullptr),
                       ffBig(nullptr),
                       ffinv(nullptr),
                       ffimp(nullptr),
                       fTH1func(nullptr),
                       fTH1g(nullptr),
                       fTH1RejSim(nullptr),
                       fTH1RejrecSim(nullptr),
                       fTH1InvSim(nullptr),
                       fLegend(nullptr),
                       fTH1ImpSim(nullptr),
                       fImpsamplingBig(0.),
                       fxmin(0.),
                       fxmax(1.),
                       fNHits(0),
                       fNBins(0) {

}

Simulator::Simulator(const Simulator &source): TObject(source),
                                               fImpsamplingBig(source.fImpsamplingBig),
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

    if(source.ffimp){
        ffimp = new TF1(*source.ffimp);
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

    if(source.fTH1RejrecSim){
        fTH1RejrecSim = new TH1D(*(dynamic_cast<TH1D*>(source.fTH1RejrecSim)));
        fTH1RejrecSim->SetDirectory(nullptr);
    }

    if(source.fTH1InvSim){
        fTH1InvSim = new TH1D(*(dynamic_cast<TH1D*>(source.fTH1InvSim)));
        fTH1InvSim->SetDirectory(nullptr);
    }

    if(source.fTH1NumSim){
        fTH1NumSim = new TH1D(*(dynamic_cast<TH1D*>(source.fTH1NumSim)));
        fTH1NumSim->SetDirectory(nullptr);
    }

    if(source.fTH1ImpSim){
        fTH1ImpSim = new TH1D(*(dynamic_cast<TH1D*>(source.fTH1ImpSim)));
        fTH1ImpSim->SetDirectory(nullptr);
    }


    if(source.fLegend){
        fLegend = new TLegend();
    }

}

Simulator::Simulator(TF1 *func, TF1 *funcbig, TF1 *funcinv, TF1 *funcimp, double impmax, double min, double max,
                     uint64_t hits, uint32_t bins, uint32_t seed)
        : TObject(),
          fImpsamplingBig(impmax),
          fxmin(min),
          fxmax(max),
          fNHits(hits),
          fNBins(bins) {

    ff = new TF1(*func);
    ff->SetNpx(bins);
    ffBig = new TF1(*funcbig);
    ffBig->SetNpx(bins);
    if (funcinv) {
        ffinv = new TF1(*funcinv);
        ffinv->SetNpx(bins);
    } else {
        ffinv = nullptr;
    }
    if (funcimp) {
        ffimp = new TF1(*funcimp);
        ffimp->SetNpx(bins);
        ffimp->SetLineColor(kOrange);
    } else {
        ffimp = nullptr;
    }

    delete gRandom;
    gRandom = new TRandomPlus(ff, ffBig, fxmin, fxmax, seed);
    //anche TF1 usa gRandom, devo averne solo uno in generale
    //devo garantire che gRandom funzioni ovunque!

    auto delta = static_cast<double>(TMath::Abs(fxmax - fxmin) / fNBins);

    fTH1func = new TH1D(fFuncname.c_str(), "function", fNBins, fxmin, fxmax);
    fTH1g = new TH1D(fBigname.c_str(), "fBig", fNBins, fxmin, fxmax);
    fTH1RejSim = new TH1D(fSimRejName.c_str(), "fSimulation_rejection", fNBins, fxmin, fxmax);
    fTH1RejrecSim = new TH1D(fSimRejrecName.c_str(), "fSimulation_rejection_recursive", fNBins, fxmin, fxmax);
    fTH1InvSim = new TH1D(fSimInvName.c_str(), "fSimulation_inversion", fNBins, fxmin, fxmax);
    fTH1NumSim = new TH1D(fSimNumName.c_str(), "fSimulation_GetRandom", fNBins, fxmin, fxmax);
    fTH1ImpSim = new TH1D(fSimImpName.c_str(), "fSimulation_Importance sampling", fNBins, fxmin, fxmax);

    fLegend = new TLegend();
    fLegend->AddEntry(fTH1func, "function");
    fLegend->AddEntry(fTH1g, "fbig");
    fLegend->AddEntry(fTH1RejSim, "simulation_rejection");
    fLegend->AddEntry(fTH1RejrecSim, "simulation_rejection_recursive");
    fLegend->AddEntry(fTH1InvSim, "simulation_inversion");
    fLegend->AddEntry(fTH1NumSim, "simulation_grandom");
    fLegend->AddEntry(fTH1ImpSim, "simulation_importance_sampling");
    if (ffimp->IsValid()) fLegend->AddEntry(ffimp, "fbig_importance_sampling");

    fTH1func->SetDirectory(nullptr);
    fTH1g->SetDirectory(nullptr);
    fTH1RejSim->SetDirectory(nullptr);
    fTH1RejrecSim->SetDirectory(nullptr);
    fTH1InvSim->SetDirectory(nullptr);
    fTH1ImpSim->SetDirectory(nullptr);
    fTH1NumSim->SetDirectory(nullptr);

    double data = fxmin;
    for (uint32_t bin = 1; bin <= fNBins; bin++) {
        fTH1func->SetBinContent(bin, (*ff)(data));
        fTH1g->SetBinContent(bin, (*ffBig)(data));
        data += delta;
    }


    fTH1func->SetLineColor(kBlue);
    fTH1g->SetLineColor(kRed);
    fTH1RejSim->SetLineColor(kGreen);
    fTH1InvSim->SetLineColor(kCyan);
    fTH1NumSim->SetLineColor(kYellow);
    fTH1ImpSim->SetLineColor(kViolet);
    fTH1RejrecSim->SetLineColor(kOrange + 2);
}


Simulator::Simulator(SimulatorConfiguration problem) :
        Simulator(problem.ff, problem.ffbig, problem.finv, problem.fimp, problem.fimpbig, problem.fxmin, problem.fxmax,
                  problem.fNhits,
                  problem.fNbins, problem.fSeed) {
}


Simulator::~Simulator() {
    delete fTH1func;
    delete fTH1g;
    delete fTH1RejSim;
    delete fTH1RejrecSim;
    delete fTH1InvSim;
    delete fTH1NumSim;
    delete fTH1ImpSim;
    delete ff;
    delete ffBig;
    delete ffinv;
    delete ffimp;
    delete fLegend;
}


Simulator &Simulator::operator=(const Simulator &source) {
    if(this == &source) return *this;
    this->~Simulator();
    new(this) Simulator(source);
    return *this;
}

void Simulator::SetConfiguration(SimulatorConfiguration conf){
    this->~Simulator();
    new(this) Simulator(conf);
}


Simulator& Simulator::DoRejectionAnalysis() {
    printf("\nDoing rejection analysis\n");
    StopWatch stopwatch;
    for (uint64_t hit = 0; hit < fNHits; hit++){
        PrintCurrentHit(hit);
        fTH1RejSim->Fill(dynamic_cast<TRandomPlus*>(gRandom)->GetByRejection());
    }
    fTH1RejSim->Scale(fTH1func->Integral("width") / fTH1RejSim->GetEntries(), "width");
    return (*this);
}

Simulator& Simulator::DoRejectionRecursiveAnalysis() {
    printf("\nDoing rejection recursive analysis\n");
    StopWatch stopwatch;
    for (uint64_t hit = 0; hit < fNHits; hit++){
        PrintCurrentHit(hit);
        fTH1RejrecSim->Fill(dynamic_cast<TRandomPlus*>(gRandom)->GetByRejectionRecursive());
    }
    fTH1RejrecSim->Scale(fTH1func->Integral("width") / fTH1RejrecSim->GetEntries(), "width");
    return (*this);
}


Simulator& Simulator::DoInversionAnalysis() {
    printf("\nDoing inversion analysis\n");
    if(ffinv->IsValid()) {
        StopWatch stopwatch;
        for (uint64_t hit = 0; hit < fNHits; hit++) {
            PrintCurrentHit(hit);
            double x = dynamic_cast<TRandomPlus*>(gRandom)->GetByInversion(ffinv);
            double w = gRandom->Rndm();
            if (w < 0.5) {
                x += TMath::Pi();
            } else {
                if (x < 0) {
                    x += TMath::TwoPi();
                }
            }
            fTH1InvSim->Fill(x);
        }
        fTH1InvSim->Scale(fTH1func->Integral("width") / fTH1InvSim->GetEntries(), "width");
    }else{
        cerr<<" f for importance sampling is nullptr"<<endl;
    }
    return (*this);
};

Simulator& Simulator::DoInversionNumericAnalysis(){
    printf("\nDoing numeric analysis\n");
    StopWatch stopwatch;
    for (uint64_t hit = 0; hit < fNHits; hit++){
        PrintCurrentHit(hit);
        fTH1NumSim->Fill(ff->GetRandom());
    }
    fTH1NumSim->Scale(fTH1func->Integral("width") / fTH1NumSim->GetEntries(), "width");
    return (*this);
};

Simulator& Simulator::DoImportanceSamplingcAnalysis() {
    printf("\nDoing importance sampling analysis\n");
    if(ffimp->IsValid()) {
        StopWatch stopwatch;
        for (uint64_t hit = 0; hit < fNHits; hit++) {
            PrintCurrentHit(hit);
            fTH1ImpSim->Fill(dynamic_cast<TRandomPlus*>(gRandom)->GetByImportanceSampling(fImpsamplingBig, ffimp));
        }
        fTH1ImpSim->Scale(fTH1func->Integral("width") / fTH1ImpSim->GetEntries(), "width");
    }else{
        cerr<<" f for importance sampling is nullptr"<<endl;
    }
    return (*this);
}


Simulator& Simulator::Save(Option_t *option){
    cout<<"\nSaving results..";
    auto file = new TFile(fFilename.c_str(), "UPDATE");
    if (fTH1func)   fTH1func->Write();
    if (fTH1g)      fTH1g->Write();
    if (fTH1RejSim) fTH1RejSim->Write();
    if (fTH1RejrecSim) fTH1RejrecSim->Write();
    if (fTH1InvSim) fTH1InvSim->Write();
    if (fTH1NumSim) fTH1NumSim->Write();
    if (fTH1ImpSim) fTH1ImpSim->Write();
    if(ffimp)       ffimp->Write(fImpsamplingName.c_str());
    if (fLegend)    fLegend->Write("leg", TObject::kOverwrite | TObject::kSingleKey);

    this->Write(option);

    file->Close();
    delete file;

    cout<<"\rResults saved.."<<endl;
    return(*this);
}


Simulator &Simulator::Show(TCanvas *canvas){
    cout<<"\nShowing results..";
    auto file = new TFile(fFilename.c_str(), "READ");

    auto hfunc = dynamic_cast<TH1D*>(file->Get(fFuncname.c_str()));
    auto hg = dynamic_cast<TH1D*>(file->Get(fBigname.c_str()));
    auto hrej = dynamic_cast<TH1D*>(file->Get(fSimRejName.c_str()));
    auto hrejrec = dynamic_cast<TH1D*>(file->Get(fSimRejrecName.c_str()));
    auto hinv = dynamic_cast<TH1D*>(file->Get(fSimInvName.c_str()));
    auto hnum = dynamic_cast<TH1D*>(file->Get(fSimNumName.c_str()));
    auto himp = dynamic_cast<TH1D*>(file->Get(fSimImpName.c_str()));
    auto fimp = dynamic_cast<TF1*>(file->Get(fImpsamplingName.c_str()));

    auto leg = (TLegend*) file->Get("leg");


    if(hfunc) hfunc->SetDirectory(nullptr);
    if(hg) hg->SetDirectory(nullptr);
    if(hrej) hrej->SetDirectory(nullptr);
    if(hinv) hinv->SetDirectory(nullptr);
    if(hnum) hnum->SetDirectory(nullptr);
    if(himp) himp->SetDirectory(nullptr);
    if(hrejrec) hrejrec->SetDirectory(nullptr);

    file->Close();
    delete file;

    if(canvas) {
        canvas->cd();
        canvas->Clear();
        canvas->SetTitle("Distributions");
        canvas->SetName("distributions");

        if (hfunc) hfunc->Draw("hist c");
        if (hg) hg->Draw("hist c same");
        if (hrej) hrej->Draw("hist c same");
        if (hinv) hinv->Draw("hist c same");
        if (hnum) hnum->Draw("hist c same");
        if (himp) himp->Draw("hist c same");
        if (hrejrec) hrejrec->Draw("hist c same");
        if (fimp->IsValid()) fimp->Draw("same");
        if (leg) leg->Draw("same");
    }else{
        cerr<<"TCanvas is invalid"<<endl;
    }
    return (*this);
}


void Simulator::PrintCurrentHit(uint64_t hit){
    if((hit+1)%10==0){
        printf("\r%llu/%llu", hit + 1 , fNHits);
    }
}