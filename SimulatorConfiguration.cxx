//
// Created by Filippo Valle on 15/10/2018.
//

#include "TRandomFProblemConfiguration.h"

ClassImp(TRandomFProblemConfiguration)

TRandomFProblemConfiguration::TRandomFProblemConfiguration() : TObject(),
                                                               ff(nullptr),
                                                               ffbig(nullptr),
                                                               finv(nullptr),
                                                               fxmin(0.),
                                                               fxmax(1.),
                                                               fNhits(0),
                                                               fNbins(0) {
}


TRandomFProblemConfiguration::TRandomFProblemConfiguration(const TRandomFProblemConfiguration& source) : TObject(source),
                                                                                                         fxmin(source.fxmin),
                                                                                                         fxmax(source.fxmax),
                                                                                                         finv(source.finv),
                                                                                                         fNhits(source.fNhits),
                                                                                                         fNbins(source.fNbins) {

    if(source.ff){
        ff = new TF1(*source.ff);
    }else{
        ff = new TF1();
    }
    if(source.finv){
        finv = new TF1(*source.finv);
    }else{
        finv = new TF1();
    }
    if(source.ffbig){
        ffbig = new TF1(*source.ffbig);
    }else{
        ffbig = new TF1();
    }
}

TRandomFProblemConfiguration::TRandomFProblemConfiguration(TF1 *f, TF1 *fbig, TF1* inv, double fxmin, double fxmax,
                                                           string descr, double **params, uint64_t hits,
                                                           uint32_t bins) :
        TObject(),
        fxmin(fxmin),
        fxmax(fxmax),
        fDescription(move(descr)),
        fNhits(hits),
        fNbins(bins){
    ff = new TF1(*f);
    ff->SetParameters(params[0]);

    ffbig = new TF1(*fbig);
    ffbig->SetParameters(params[1]);

    finv = new TF1(*inv);
    finv->SetParameters(params[2]);
}

ostream &operator<<(ostream &os, const TRandomFProblemConfiguration &configuration) {
    os << configuration.fDescription.c_str() << " fxmin: " << configuration.fxmin << " fxmax: " << configuration.fxmax << " fNhits: "
       << configuration.fNhits << " fNbins: " << configuration.fNbins;
    return os;
}

TRandomFProblemConfiguration &TRandomFProblemConfiguration::operator=(const TRandomFProblemConfiguration &source) {
    if(this == &source) return *this;
    this->~TRandomFProblemConfiguration();
    new(this) TRandomFProblemConfiguration(source);
    return *this;
}

TRandomFProblemConfiguration::~TRandomFProblemConfiguration() {
    delete ff;
    delete ffbig;
    delete finv;
}

