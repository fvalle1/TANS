#include "TSystem.h"

int compile(){
    gSystem->CompileMacro("io.cpp", "kO");
    gSystem->CompileMacro("hist.cpp", "kO");
    gSystem->CompileMacro("hist_readfile.cxx", "kO");
    gSystem->CompileMacro("central.cpp", "kO");
    gSystem->CompileMacro("decay.cxx", "kO");

    return 0;
}