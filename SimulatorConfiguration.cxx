//
// Created by Filippo Valle on 15/10/2018.
//

#include "SimulatorConfiguration.h"

ClassImp(SimulatorConfiguration)

SimulatorConfiguration::SimulatorConfiguration() : TObject(),
                                                   ff(nullptr),
                                                   ffbig(nullptr),
                                                   finv(nullptr),
                                                   fimp(nullptr),
                                                   fimpbig(0.),
                                                   fxmin(0.),
                                                   fxmax(1.),
                                                   fNhits(0),
                                                   fNbins(0),
                                                   fSeed(42){
}


SimulatorConfiguration::SimulatorConfiguration(const SimulatorConfiguration& source) : TObject(source),
                                                                                       fimpbig(source.fimpbig),
                                                                                       fxmin(source.fxmin),
                                                                                       fxmax(source.fxmax),
                                                                                       finv(source.finv),
                                                                                       fNhits(source.fNhits),
                                                                                       fNbins(source.fNbins),
                                                                                       fSeed(source.fSeed){

    if(source.ff){
        ff = new TF1(*source.ff);
    }else{
        ff = new TF1();
    }

    if(source.ffbig){
        ffbig = new TF1(*source.ffbig);
    }else{
        ffbig = new TF1();
    }

    if(source.finv){
        finv = new TF1(*source.finv);
    }else{
        finv = new TF1();
    }

    if(source.fimp){
        fimp = new TF1(*source.fimp);
    }else{
        fimp = new TF1();
    }
}

SimulatorConfiguration::SimulatorConfiguration(TF1 *f, TF1 *fbig, TF1 *inv, TF1 *imp, double impmax, double xmin,
                                               double xmax, TString descr, double **params, uint64_t hits,
                                               uint32_t bins, uint32_t seed) :
        TObject(),
        fimpbig(impmax),
        fxmin(xmin),
        fxmax(xmax),
        fDescription(move(descr)),
        fNhits(hits),
        fNbins(bins),
        fSeed(seed){
    if (f) {
        ff = new TF1(*f);
        ff->SetParameters(params[0]);
    } else {
        ff = new TF1();
    }

    if (fbig) {
        ffbig = new TF1(*fbig);
        ffbig->SetParameters(params[1]);
    } else {
        ffbig = new TF1();
    }

    if (inv) {
        finv = new TF1(*inv);
        finv->SetParameters(params[2]);
    } else {
        finv = nullptr;
    }

    if (imp) {
        fimp = new TF1(*imp);
        fimp->SetParameters(params[3]);
    } else {
        fimp = nullptr;
    }
}

ostream &operator<<(ostream &os, const SimulatorConfiguration &configuration) {
    os << configuration.fDescription.Data() << " fxmin: " << configuration.fxmin << " fxmax: " << configuration.fxmax << " fNhits: "
       << configuration.fNhits << " fNbins: " << configuration.fNbins << " fSeed: " << configuration.fSeed;
    return os;
}

SimulatorConfiguration &SimulatorConfiguration::operator=(const SimulatorConfiguration &source) {
    if(this == &source) return *this;
    this->~SimulatorConfiguration();
    new(this) SimulatorConfiguration(source);
    return *this;
}

SimulatorConfiguration::~SimulatorConfiguration() {
    delete ff;
    delete ffbig;
    delete finv;
}

