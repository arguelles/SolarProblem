#include <SQuIDS/const.h>
#include <iostream>
#include "solar_probabilities.h"
#include "SMinit.h"

using namespace squids;
using namespace nusquids;

int main(){
  //std::cout << "DM" << std::endl;
  std::shared_ptr<littlemermaid> dm = std::make_shared<littlemermaid>("DM");
  //std::cout << "SM" << std::endl;
  std::shared_ptr<littlemermaid> sm = std::make_shared<littlemermaid>("Standard");
  return 0;
}
