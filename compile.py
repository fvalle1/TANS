from ROOT import *

gSystem.CompileMacro("StopWatch.cxx", "kO");
gSystem.CompileMacro("TRandomPlus.cxx", "kO");
gSystem.CompileMacro("SimulatorConfiguration.cxx", "kO");
gSystem.CompileMacro("Simulator.cxx", "kO");
gSystem.CompileMacro("RandomF.cxx", "kO");

randomF()
