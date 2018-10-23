//
// Created by Filippo Valle on 03/10/2018.
//

#ifndef TANS_STOPWATCH_H
#define TANS_STOPWATCH_H

#include <iostream>
#include <chrono>

class StopWatch {
public:
    inline StopWatch(){
        fSystemTime = std::chrono::system_clock::now();
        fCPUTime = std::clock();
    };
    inline ~StopWatch(){
        std::cout<<"\nRunned in: "<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - fSystemTime).count()/1000.<<" sec [Wall time]"<<std::endl;
        std::cout<<"Runned in: "<<(std::clock() - fCPUTime) / (double)CLOCKS_PER_SEC<<" sec [CPU time]"<<std::endl;
    }
private:
    std::chrono::time_point<std::chrono::system_clock> fSystemTime;
    std::clock_t fCPUTime;
};


#endif //TANS_STOPWATCH_H
