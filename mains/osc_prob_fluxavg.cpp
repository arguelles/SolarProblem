#include <SQuIDS/const.h>
#include <iostream>
#include "solar_probabilities.h"
#include "SMinit.h"

using namespace squids;
using namespace nusquids;

int main(){
  SOP sop;
  Const units;

  std::shared_ptr<littlemermaid> sm = std::make_shared<littlemermaid>("Standard");
  std::shared_ptr<Const> parameters = std::make_shared<Const>();

  parameters->SetMixingAngle(0,1,0.583996); // th12
  parameters->SetMixingAngle(0,2,0.148190); // th13
  parameters->SetMixingAngle(1,2,0.737324); // th23
  parameters->SetEnergyDifference(1,7.5e-05); // dm^2_21
  parameters->SetEnergyDifference(2,0.00257); // dm^2_31
  parameters->SetPhase(0,2,0.0); // delta_13 = diract cp phase

  sop.SetMixingParameters(parameters);
  sop.SetSolarModel(sm);

  auto e_range = linspace(0.1,20,100);
  for(double e: e_range){
    std::cout << e << " " << sop.RadialIntegratedFluxes(e*units.MeV) << " " << sop.PeeCuadradito(e*units.MeV)<< std::endl;
  }

  return 0;
}
