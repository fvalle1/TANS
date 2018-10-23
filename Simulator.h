//
// Created by Filippo Valle on 11/10/2018.
//

#ifndef TANS_TRANDOMF_H
#define TANS_TRANDOMF_H

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <TH1.h>
#include <TFile.h>
#include <TLegend.h>
#include <TCanvas.h>
#include "StopWatch.h"
#include <TMath.h>
#include "TRandomFProblemConfiguration.h"
#include "TRandomPlus.h
#endif

using namespace std;

class Simulator : public TRandom3{
public:
    Simulator();
    Simulator(const Simulator &source);
    Simulator(TF1 *, TF1 *, TF1 *funcinv, double min, double max, uint64_t hits, uint32_t bins);
    explicit Simulator(TRandomFProblemConfiguration problem);
    ~Simulator() override;

    double GetByRejection();
    double GetByInversion();

    Simulator& operator=(const Simulator &source);

    Simulator& DoRejectionAnalysis();
    Simulator& DoInversionAnalysis();
    Simulator& DoInversionNumericAnalysis();

    Simulator& Save(Option_t *option = "");
    Simulator& Show(TCanvas *canvas);

private:
    TF1* ff;
    TF1* ffBig;
    TF1* ffinv;

    double fxmin;
    double fxmax;

    uint32_t fNBins;
    uint64_t fNHits;

    TH1* fTH1func;
    TH1* fTH1g;
    TH1* fTH1RejSim;
    TH1* fTH1InvSim;
    TH1* fTH1NumSim;

    TLegend* fLegend;
    
    TRandomPlus* fRandgen; 

    static const string ffilename;
    static const string ffuncname;
    static const string fgname;
    static const string fsimRejname;
    static const string fsimInvname;
    static const string fsimNumname;

ClassDef(Simulator, 1)

};

#endif //TANS_TRANDOMF_H
