#include <SQuIDS/const.h>
#include <iostream>
#include "solar_probabilities.h"
#include "SMinit.h"

using namespace squids;
using namespace nusquids;

int main(){
  SOP sop;
  Const units;

  std::shared_ptr<littlemermaid> sm = std::make_shared<littlemermaid>("AGSS09");
  std::shared_ptr<Const> parameters = std::make_shared<Const>();

  sop.SetMixingParameters(parameters);
  sop.SetSolarModel(sm);

  auto e_range = linspace(0.1,10,100);
  for(double e: e_range){
    std::cout << e << " " << sop.SolarOscillationProbability(e*units.MeV,0.1) << std::endl;
  }

  return 0;
}
