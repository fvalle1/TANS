//
// Created by Filippo Valle on 24/10/2018.
//

#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TRandomPlus.h"
#include "TH1D.h"
#include "StopWatch.h"
#endif

using namespace std;

double f(double x) {
    if (fabs(x) < 1e-9) return 1 - x * x / 3.;
    else {
        double temp = sin(x) / x;
        return temp * temp;
    }
}

double g(double theta){
    double c = cos(theta);
    double s = sin(theta);
    double temp = 0.5 * c * c + s * s;
    return 1/temp;
}

void metropolis(uint64_t Ntrials = 100000){
    delete gRandom;
    gRandom = new TRandomPlus();
    double x0 = 0.;
    double min = -10;
    double max = 10;
    auto hist = new TH1D("hist", "hist", 100, min, max);
    {
        StopWatch watch;
        for (uint64_t trash = 0; trash < 1000; ++trash)
            dynamic_cast<TRandomPlus *>(gRandom)->GetByMetropolis(x0, f, min, max);
        for (uint64_t hit = 0; hit < Ntrials; ++hit) {
            if (hit+1 % 10 == 0) printf("\r%llu/%llu", hit + 1, Ntrials);
            dynamic_cast<TRandomPlus *>(gRandom)->GetByMetropolis(x0, f, min, max);
            hist->Fill(x0);
        }
    }
    hist->Draw("");
}