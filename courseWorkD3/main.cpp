#include "mainForm.hpp"
#include <cstdint>
#include <iostream>
#include <limits>

int main(int argc, char* argv[]) {
  std::cout << static_cast<int>(std::numeric_limits<int>::min() - 1); }