//
// Created by Filippo Valle on 03/10/2018.
//

#ifndef TANS_STOPWATCH_H
#define TANS_STOPWATCH_H

#include <iostream>
#include <chrono>

class StopWatch {
public:
    inline StopWatch(){fTime = std::chrono::system_clock::now();};
    inline ~StopWatch(){std::cout<<"time: "<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - fTime).count()/1000.<<std::endl;}
private:
    std::chrono::time_point<std::chrono::system_clock> fTime;
};


#endif //TANS_STOPWATCH_H
