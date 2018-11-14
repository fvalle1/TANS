#include "TSystem.h"
#include "TString.h"

int compile(TString myopt = "fast"){
    TString opt;
    if(myopt.Contains("force")) {
        opt = "kfO";
        gSystem->SetFlagsOpt("-Wunused-parameter -Winconsistent-missing-override");
    }else{
        gSystem->SetFlagsOpt("-Wunused-parameter -Winconsistent-missing-override -O3");
        opt = "kg0";
    }
//    gSystem->AddIncludePath("/Users/filippo/anaconda3/include/curl/curl.h");
//    gSystem->AddLinkedLibs("/usr/lib/libcurl.dylib");
//    gSystem->AddLinkedLibs("/usr/lib/libssl.dylib");
//    gSystem->AddLinkedLibs("/usr/lib/libcrypto.dylib");
//    gSystem->AddLinkedLibs("/usr/lib/libz.dylib");
//    gSystem->CompileMacro("TelegramWatch.cxx", opt.Data());
    gSystem->CompileMacro("StopWatch.cxx", opt.Data());
    gSystem->CompileMacro("TRandomPlus.cxx", opt.Data());
    gSystem->CompileMacro("SimulatorConfiguration.cxx", opt.Data());
    gSystem->CompileMacro("Simulator.cxx", opt.Data());
    gSystem->CompileMacro("randomF.cxx", opt.Data());
    gSystem->CompileMacro("chisquare.cxx", opt.Data());
    return 0;
}