#include <SQuIDS/const.h>
#include <iostream>
#include "solar_probabilities.h"
#include "SMinit.h"

using namespace squids;
using namespace nusquids;

int main(){
  Const units;
  std::shared_ptr<littlemermaid> sm = std::make_shared<littlemermaid>("AGSS09");

  auto r_range = linspace(0.01,0.49,100);
  for(double r: r_range){
    std::cout << r << " " << sm->nuFlux(r,1.0*units.MeV,littlemermaid::pp) << std::endl;
  }

  return 0;
}
