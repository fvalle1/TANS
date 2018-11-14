//
// Created by Filippo Valle on 17/10/2018.
//

#ifndef NEURALANALYZER_TRANDOMPLUS_H
#define NEURALANALYZER_TRANDOMPLUS_H

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <functional>
#include <TRandom3.h>
#include <TF1.h>
#endif

typedef std::function<double(double)> func;


namespace TRandomPlusStatuses{
    enum statuses{
        kDefined,
        kFree
    };
}

class TRandomPlus : public TRandom3 {

public:
    TRandomPlus();
    TRandomPlus(const TRandomPlus& );
    TRandomPlus(TF1 *f, TF1 *fbig, double xmin, double xmax, uint32_t seed = 42);

    virtual ~TRandomPlus();

    TRandomPlus& operator=(const TRandomPlus &source);

    inline double GetBinary(){return Rndm()>0.5?1:-1;}
    double GetByRejection();
    double GetByRejectionRecursive();
    double GetByInversion(TF1 *finv);
    double GetByImportanceSampling(double overfBig, TF1 *fimportance);
    void GetBySphere(double *coords);
    void GetBySphere(double &th, double &phi);
    void GetByMetropolis(double& x, func f, double xmin = 0., double xmax = 1.);


private:
    TF1* ff;
    TF1* ffbig;

    double fxmin;
    double fxmax;

    TRandomPlusStatuses::statuses fstatus;

    double GetForImportanceSampling(TF1 *fimp, double fSuperBig = 100);

ClassDef(TRandomPlus, 3)
};


#endif //NEURALANALYZER_TRANDOMPLUS_H
