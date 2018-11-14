//
// Created by Filippo Valle on 25/09/2018.
//

#include "TFile.h"
#include "Simulator.h"
#include "SimulatorConfiguration.h"
#include "TelegramWatch.h"

using std::cout;
using std::endl;

void sim(const char* name){
    auto file = new TFile("config.root", "READ");
    auto config = (SimulatorConfiguration*) file->Get(name);
    if(config){
        cout<<*config<<endl;
        Simulator sim(*config);
        sim.DoRejectionAnalysis()/*.Save()*/;
        sim.DoRejectionRecursiveAnalysis()/*.Save()*/;
        sim.DoInversionAnalysis()/*.Save()*/;
        sim.DoImportanceSamplingcAnalysis()/*.Save()*/;
        sim.DoInversionNumericAnalysis().Save();
    }

    file->Close();
    delete file;
}

int main(){
    TelegramWatch telegramWatch("simulation_random_function");
    sim("theta_001_long");
//    sim(" theta_01_long");
//    sim("theta_05_long");
//    sim("theta_05");
//    sim("sinc_05");
//    sim("theta_01");
//    sim("theta_001");
//    sim("sinc_long");
//    sim("sinc_long");
    return 0;
}