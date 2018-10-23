//
// Created by Filippo Valle on 15/10/2018.
//

#ifndef TANS_TRANDOMFPROBLEMCONFIGURATION_H
#define TANS_TRANDOMFPROBLEMCONFIGURATION_H

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <TObject.h>
#include <TF1.h>
#include <TString.h>
#include <functional>
#endif

using namespace std;

class SimulatorConfiguration : public TObject {
public:
    SimulatorConfiguration();
    SimulatorConfiguration(const SimulatorConfiguration &source);
    SimulatorConfiguration(TF1 *ff, TF1 *ffbig, TF1 *inv, TF1 *imp, double impmax, double xmin,
                               double xmax, TString descr, double **params, uint64_t hits = 1000000,
                               uint32_t bins = 150, uint32_t seed = 42);
    virtual ~SimulatorConfiguration();

    friend ostream &operator<<(ostream &os, const SimulatorConfiguration &configuration);
    SimulatorConfiguration& operator=(const SimulatorConfiguration& source);

public:
    TF1* ff;
    TF1* ffbig;
    TF1* finv;
    TF1* fimp;

    double fxmin;
    double fxmax;
    double fimpbig;
    uint64_t fNhits;
    uint32_t fNbins;
    uint32_t fSeed;
    TString fDescription;

    ClassDef(SimulatorConfiguration, 4)
};


#endif //TANS_TRANDOMFPROBLEMCONFIGURATION_H
