//
// Created by dym on 13.11.2020.
//
#include <iostream>
#include "Area.hpp"

int main() {
    using namespace simulationTree;
    Area area({20, 7}, 10,{},{3,0});
    std::cout << area;
    while(1) {
        area.life();
        std::cout << area;
    }


    return EXIT_SUCCESS;
}