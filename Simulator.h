//
// Created by Filippo Valle on 11/10/2018.
//

#ifndef TANS_TRANDOMF_H
#define TANS_TRANDOMF_H

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TObject.h>
#include <Riostream.h>
#include <TH1.h>
#include <TFile.h>
#include <TLegend.h>
#include <TCanvas.h>
#include "StopWatch.h"
#include <TMath.h>
#include "SimulatorConfiguration.h"
#include "TRandomPlus.h"
#endif

using namespace std;

class Simulator : public TObject{
public:
    Simulator();
    Simulator(const Simulator &source);
    Simulator(TF1 *, TF1 *, TF1 *funcinv, TF1 *funcimp, double impmax, double min, double max, uint64_t hits,
                  uint32_t bins, uint32_t seed);
    explicit Simulator(SimulatorConfiguration problem);
    ~Simulator() override;

    Simulator& operator=(const Simulator &source);

    void SetConfiguration(SimulatorConfiguration);

    Simulator& DoRejectionAnalysis();
    Simulator& DoRejectionRecursiveAnalysis();
    Simulator& DoInversionAnalysis();
    Simulator& DoInversionNumericAnalysis();
    Simulator& DoImportanceSamplingcAnalysis();

    Simulator& Save(Option_t *option = "");
    Simulator& Show(TCanvas *canvas);

private:
    TF1* ff;
    TF1* ffBig;
    TF1* ffinv;
    TF1* ffimp;

    double fxmin;
    double fxmax;
    double fImpsamplingBig;

    uint32_t fNBins;
    uint64_t fNHits;

    TH1* fTH1func;
    TH1* fTH1g;
    TH1* fTH1RejSim;
    TH1* fTH1RejrecSim;
    TH1* fTH1InvSim;
    TH1* fTH1NumSim;
    TH1* fTH1ImpSim;

    TLegend* fLegend;

    void PrintCurrentHit(uint64_t hit); //!

    static const string fFilename;
    static const string fFuncname;
    static const string fBigname;
    static const string fSimRejName;
    static const string fSimRejrecName;
    static const string fSimInvName;
    static const string fSimNumName;
    static const string fSimImpName;
    static const string fImpsamplingName;

ClassDef(Simulator, 6)

};

#endif //TANS_TRANDOMF_H
