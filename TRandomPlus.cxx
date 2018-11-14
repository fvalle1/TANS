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
    fstatus = TRandomPlusStatuses::statuses::kFree;
    SetName("TRandomPlus");
    SetTitle("TRandom3 with Rejection and Inverion");
}

TRandomPlus::TRandomPlus(const TRandomPlus& source):TRandom3(source),
                                                    fxmin(source.fxmin),
                                                    fxmax(source.fxmax)
{
    if(source.ff){
        ff = new TF1(*source.ff);
        fstatus = TRandomPlusStatuses::statuses::kDefined;
    }else{
        fstatus = TRandomPlusStatuses::statuses::kFree;
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
    fstatus = TRandomPlusStatuses::statuses::kDefined;
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
    if(fstatus == 0) {
        auto *X = new double[2];
        double u, x;
        do {
            RndmArray(2, X);
            x = fxmin + (fxmax - fxmin) * X[0];
            u = (*ffbig)(x) * X[1];
        } while ((*ff)(x) <= u);
        delete[] X;
        return x;
    }else{
        std::cerr<<"Functions not defined"<<std::endl;
        return -1;
    }
}

double TRandomPlus::GetByRejectionRecursive() {
    if(fstatus == 0) {
        auto *X = new double[2];
        RndmArray(2, X);
        double x = fxmin + (fxmax - fxmin) * X[0];
        double u = (*ffbig)(x) * X[1];
        delete[] X;
        if ((*ff)(x) >= u) return x;
        else return GetByRejectionRecursive();
    }else{
        std::cerr<<"Functions not defined"<<std::endl;
        return -1;
    }
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
    if(fstatus == 0) {
        auto *X = new double[2];
        double u, x;
        do {
            RndmArray(2, X);
            x = GetForImportanceSampling(fimportance, overfBig);
            u = (*fimportance)(x) * X[1];
        } while ((*ff)(x) <= u);
        delete[] X;
        return x;
    }else{
        std::cerr<<"Functions not defined"<<std::endl;
        return -1;
    }
}

double TRandomPlus::GetForImportanceSampling(TF1 *fimp, double fSuperBig) {
    if(fstatus == 0){
        auto *X = new double[2];
        double u, x;
        do{
            RndmArray(2, X);
            x = fxmin + (fxmax -  fxmin) * X[0];
            u = fSuperBig * X[1];
        }while((*fimp)(x) <= u);
        delete[] X;
        return x;
    }else{
        std::cerr<<"Functions not defined"<<std::endl;
        return -1;
    }
}

void TRandomPlus::GetBySphere(double *coords) {
    RndmArray(2, coords);
    coords[0] = coords[0] * TMath::TwoPi();
    coords[1] = TMath::ACos(1. - 2. * coords[1]);
}

void TRandomPlus::GetBySphere(double &th, double &phi){
    th = Rndm() * TMath::TwoPi();
    phi = TMath::ACos(1. - 2. * Rndm());
};

void TRandomPlus::GetByMetropolis(double &x, func f, double xmin, double xmax) {
    do {
        double delta = (xmax - xmin)/10.;
        double dx = Rndm() * 2 * delta - delta;
//        double dx = Gaus(0, 0.5);
        double a = f(x + dx) / f(x);
        if (a > 1) {
            x += dx;
        } else {
            if (Rndm() < a) {
                x += dx;
            }
        }
    }while(x > xmax || x < xmin);
}

