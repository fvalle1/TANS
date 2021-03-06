cmake_minimum_required(VERSION 3.10)
project(tans)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OpenMP_SHARED_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()


project(NeuralAnalyzer)

##root
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS RIO Net)
include(${ROOT_USE_FILE})

ROOT_GENERATE_DICTIONARY(G__TRandomPlus TRandomPlus.h LINKDEF)
add_library(TRandomPlus SHARED TRandomPlus.cxx G__TRandomPlus.cxx)
target_link_libraries(TRandomPlus ${ROOT_LIBRARIES})

ROOT_GENERATE_DICTIONARY(G__SimulatorConfiguration SimulatorConfiguration.h LINKDEF)
add_library(SimulatorConfiguration SHARED SimulatorConfiguration.cxx G__SimulatorConfiguration.cxx)
target_link_libraries(SimulatorConfiguration ${ROOT_LIBRARIES})

ROOT_GENERATE_DICTIONARY(G__Simulator Simulator.h LINKDEF)
add_library(Simulator SHARED Simulator.cxx G__Simulator.cxx metropolis.cxx)
target_link_libraries(Simulator TRandomPlus SimulatorConfiguration ${ROOT_LIBRARIES})

ROOT_GENERATE_DICTIONARY(G__chisquareomp chisquareomp.h LINKDEF)
add_library(chisquareomp SHARED chisquareomp.cxx G__chisquareomp.cxx chisquareomp.cxx)
target_link_libraries(chisquareomp ${ROOT_LIBRARIES})

include_directories(
        .
        ~/root/include
        ~/anaconda3/include
)

link_directories(/Users/filippo/anaconda3/lib)

add_executable(tans
        main.cxx
        StopWatch.cxx
        TelegramWatch.cxx
        Simulator.cxx
        SimulatorConfiguration.cxx
        TRandomPlus.cxx
        )

target_link_libraries(tans
        Simulator
        SimulatorConfiguration
        TRandomPlus
        curl
        )

add_executable(various
        sphere.cxx
        chisquare.cxx
        chimain.cxx
        compile.C
        randomF.cxx
        tree.cxx
        )

add_executable(
        chi
        StopWatch.cxx
        chisquareomp.cxx
)

target_link_libraries(
        chi
        chisquareomp
)