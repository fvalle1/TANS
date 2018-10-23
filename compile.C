#include "TSystem.h"
#include "TString.h"

int compile(TString myopt = "fast"){
    TString opt;
    if(myopt.Contains("force")) {
        opt = "kfg";
    }else{
        opt = "kg0";
    }
    gSystem->CompileMacro("StopWatch.cxx", opt.Data());
    gSystem->CompileMacro("TRandomPlus.cxx", opt.Data());
    gSystem->CompileMacro("SimulatorConfiguration.cxx", opt.Data());
    gSystem->CompileMacro("Simulator.cxx", opt.Data());
    gSystem->CompileMacro("randomF.cxx", opt.Data());
    return 0;
}