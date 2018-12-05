//
// Created by Filippo Valle on 29/10/2018.
//

#include "chisquareomp.h"

double background(double x){
    return 0.1690*(10-0.01*x)*1000;
}

void chisquare(uint64_t Nexperiments = 90000000){
    StopWatch stopwatch;
    //gStyle->SetOptStat(00000000);
    printf("\nInitializing with thread %u", omp_get_thread_num());
    int NDF = 12;
    int xmin = 0;
    int xmax = 24;
    int chimax = 80;
    auto data = new TH1D("data", "data", NDF, xmin, xmax);
    data->SetFillColor(kOrange);
    data->Fill(1, 1);
    data->Fill(3, 3);
    data->Fill(5, 1);
    data->Fill(7, 4);
    data->Fill(9, 3);
    data->Fill(11, 5);
    data->Fill(13, 2);
    data->Fill(15, 1);
    data->Fill(17, 1);
    data->Fill(19, 0);
    data->Fill(21, 1);
    data->Fill(23, 3);

    auto back = new TF1("back", [](double *x, double* par){return background(x[0]);}, xmin, xmax, 0);
    auto chihist = new TH1D("chi", "chi", 100, 0, chimax);
    auto chisquaredist = [](double* xx, double* par){
        auto x = xx[0];
        double k = par[1];
        double ret = TMath::Power(x, k/2. - 1);
        ret*=TMath::Exp(-x/2. - TMath::LnGamma(k/2.));
        ret*=TMath::Power(2, - k/2.);
        return ret*par[0];
    };
    auto chiteo = new TF1("chiteo", chisquaredist, 0, chimax, 2);
    chiteo->SetParameter(0, 1);
    chiteo->SetParameter(1, NDF);
    chihist->SetMarkerColor(kCyan);
    chihist->SetMarkerSize(1);

    double norm = 0;
    auto threads = new double*[omp_get_num_threads()];
#pragma omp parallel shared(threads)
    {
        int nthreads = omp_get_num_threads();
        uint64_t allocSize = Nexperiments / nthreads;
        int currThread = omp_get_thread_num();
        double* currarr = nullptr;
        currarr= new double[allocSize];
        uint64_t alloc = 0;
        printf("\nStarting thread %u", currThread);
        pcg64 rnd(42, currThread);
        for (uint64_t experiment = 0; experiment < Nexperiments / nthreads; ++experiment) {
            double chi = 0.;
            for (int i = 1; i < NDF; ++i) {
                double vi = background(i);
                std::poisson_distribution<int> pois(vi);
                double ni = pois(rnd);
                chi += (ni - vi) * (ni - vi) / vi;
            }
            if (alloc < allocSize) currarr[alloc++] = chi;
            else break;
            //chihist->Fill(chi);
        }
        threads[currThread] = currarr;
        printf("\nEnding thread %u", currThread);
    }
#pragma omp master
    {
        printf("\nFinishing with thread %u", omp_get_thread_num());
        uint64_t allocSize = Nexperiments / omp_get_num_threads();
        for (int t = 0; t < omp_get_num_threads(); t++)
        {
            auto arr = threads[t];
            for(int i = 0; i < allocSize; i++) {
                if (i < allocSize) chihist->Fill(arr[i]);
                else break;
            }
            delete arr;
        }

        norm = chihist->Integral("width");
        chiteo->SetParameter(0, norm);

        auto cx = new TCanvas();
        cx->Divide(3,1);
        cx->cd(1);
        data->Draw("hist");
        back->Draw("same");

        cx->cd(2);
        chihist->Draw("hist");
        chiteo->Draw("same");

        cx->cd(3)->SetLogy();
        chihist->Draw("hist");
        chiteo->Draw("same");
        auto file = new TFile("chisquare.root", "RECREATE");
        cx->Write();
        file->Close();
        delete file;
        delete back;
        delete cx;
        delete chihist;
        delete chiteo;
        delete data;
    }
//    new TCanvas();
//    chiteo->SetParameter(0, 1);
//    chiteo->SetParameter(1, 4);
//    chiteo->Draw();

}


int main(){
    chisquare();
}
