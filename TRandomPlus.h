//
// Created by Filippo Valle on 17/10/2018.
//

#ifndef NEURALANALYZER_TRANDOMPLUS_H
#define NEURALANALYZER_TRANDOMPLUS_H

#include <TRandom3.h>
#include <TF1.h>

class TRandomPlus : public TRandom3 {

public:
    TRandomPlus();
    TRandomPlus(const TRandomPlus& );
    TRandomPlus(TF1 *f, TF1 *fbig, double xmin, double xmax, uint32_t seed = 42);

    virtual ~TRandomPlus();

    TRandomPlus& operator=(const TRandomPlus &source);

    double GetByRejection();
    double GetByRejectionRecursive();
    double GetByInversion(TF1 *finv);
    double GetByImportanceSampling(double overfBig, TF1 *fimportance);


private:
    TF1* ff;
    TF1* ffbig;

    double fxmin;
    double fxmax;

    double GetForImportanceSampling(TF1 *fimp, double fSuperBig = 100);

ClassDef(TRandomPlus, 2)
};


#endif //NEURALANALYZER_TRANDOMPLUS_H
