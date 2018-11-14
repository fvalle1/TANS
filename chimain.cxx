//
// Created by Filippo Valle on 29/10/2018.
//
#include <iostream>
#include "chisquareomp.cxx"

int main(){
#pragma omp parallel
    if(omp_get_thread_num()==0) printf("Running with %u threads", omp_get_num_threads());
    chisquare(50000000);
}