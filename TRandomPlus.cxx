//
// Created by Filippo Valle on 17/10/2018.
//

#include "TRandomPlus.h"

ClassImp(TRandomPlus)


TRandomPlus::TRandomPlus() : TRandom3(),
                             ff(nullptr),
                             ffbig(nullptr),
                             fxmin(0.),
                             fxmax(1.)
{
    SetName("TRandomPlus");
    SetTitle("TRandom3 with Rejection and Inverion");
}

TRandomPlus::TRandomPlus(const TRandomPlus& source):TRandom3(source),
                                                    fxmin(source.fxmin),
                                                    fxmax(source.fxmax)
{
    if(source.ff){
        ff = new TF1(*source.ff);
    }

    if(source.ffbig){
        ffbig = new TF1(*source.ffbig);
    }

}

TRandomPlus::TRandomPlus(TF1 *f, TF1 *fbig, double fxmin, double fxmax, uint32_t seed) : TRandom3(seed),
                                                                                                    fxmin(fxmin),
                                                                                                    fxmax(fxmax) {
    ff = new TF1(*f);
    ffbig = new TF1(*fbig);
    SetName("TRandomPlus");
    SetTitle("TRandom3 with Rejection and Inverion");
}

TRandomPlus::~TRandomPlus() {
    delete ff;
    delete ffbig;
}

TRandomPlus &TRandomPlus::operator=(const TRandomPlus &source) {
    if(this == &source) return *this;
    this->~TRandomPlus();
    new(this) TRandomPlus(source);
    return *this;
}

double TRandomPlus::GetByRejection() {
    auto *X = new double[2];
    double u, x;
    do{
        RndmArray(2, X);
        x = fxmin + (fxmax -  fxmin) * X[0];
        u = (*ffbig)(x) * X[1];
    }while((*ff)(x) <= u);
    delete[] X;
    return x;
}

double TRandomPlus::GetByRejectionRecursive() {
    auto *X = new double[2];
    RndmArray(2, X);
    double x = fxmin + (fxmax -  fxmin) * X[0];
    double u =  (*ffbig)(x) * X[1];
    delete[] X;
    if((*ff)(x) >= u) return x;
    else return GetByRejectionRecursive();
}

double TRandomPlus::GetByInversion(TF1 *finv) {
    if(finv) {
        auto *X = new double[1];
        double x;
        RndmArray(1, X);
        x = (*finv)(X[0]);
        delete[] X;
        return x;
    }else{
        return -1.;
    }
}

double TRandomPlus::GetByImportanceSampling(double overfBig, TF1 *fimportance) {
    auto *X = new double[2];
    double u, x;
    do{
        RndmArray(2, X);
        x = GetForImportanceSampling(fimportance, overfBig);
        u = (*fimportance)(x) * X[1];
    }while((*ff)(x) <= u);
    delete[] X;
    return x;
}

double TRandomPlus::GetForImportanceSampling(TF1 *fimp, double fSuperBig) {
    auto *X = new double[2];
    double u, x;
    do{
        RndmArray(2, X);
        x = fxmin + (fxmax -  fxmin) * X[0];
        u = fSuperBig * X[1];
    }while((*fimp)(x) <= u);
    delete[] X;
    return x;
}


