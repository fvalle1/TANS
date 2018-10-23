//
// Created by Filippo Valle on 15/10/2018.
//

#ifndef TANS_TRANDOMFPROBLEMCONFIGURATION_H
#define TANS_TRANDOMFPROBLEMCONFIGURATION_H

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <TObject.h>
#include <TF1.h>
#include <functional>
#endif


typedef std::function<double(double*, double*)> TF1func;


using namespace std;

class SimulatorConfiguration : public TObject {
public:
    SimulatorConfiguration();
    SimulatorConfiguration(const SimulatorConfiguration &source);
    SimulatorConfiguration(TF1 *ff, TF1 *ffbig, TF1* inv, double fxmin, double fxmax,
                                     string descr, double **params, uint64_t hits = 100000,
                                     uint32_t bins = 100);
    virtual ~SimulatorConfiguration();

    friend ostream &operator<<(ostream &os, const SimulatorConfiguration &configuration);
    SimulatorConfiguration& operator=(const SimulatorConfiguration& source);

public:
    TF1* ff;
    TF1* ffbig;
    TF1* finv;
    double fxmin;
    double fxmax;
    uint64_t fNhits;
    uint32_t fNbins;
    string fDescription;

    ClassDef(SimulatorConfiguration, 1)
};


#endif //TANS_TRANDOMFPROBLEMCONFIGURATION_H
