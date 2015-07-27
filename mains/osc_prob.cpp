#include <SQuIDS/const.h>
#include <iostream>
#include "solar_probabilities.h"
#include "SMinit.h"

using namespace squids;

int main(){
  SOP sop;
  std::shared_ptr<littlemermaid> sm = std::make_shared<littlemermaid>("AGSS09");
  std::shared_ptr<Const> parameters = std::make_shared<Const>();

  sop.SetMixingParameters(parameters);
  sop.SetSolarModel(sm);

  std::cout << sop.SolarOscillationProbability(1.0,0.1) << std::endl;

  return 0;
}
