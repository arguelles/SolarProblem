#include <SQuIDS/const.h>
#include <iostream>
#include "solar_probabilities.h"
#include "SMinit.h"

using namespace squids;
using namespace nusquids;

int main(){
  Const units;
  std::shared_ptr<littlemermaid> sm = std::make_shared<littlemermaid>("Standard");

  auto r_range = linspace(0.3,0.49,100);
  for(double r: r_range){
    std::cout << r << " " << sm->nuFlux(r,.4*units.MeV,littlemermaid::pp)<<"  " << sm->nuFlux(r,10*units.MeV,littlemermaid::hep)<<"  " << sm->nuFlux(r,10*units.MeV,littlemermaid::pep)<<"  " << sm->nuFlux(r,10*units.MeV,littlemermaid::be7)<<"  " << sm->nuFlux(r,10*units.MeV,littlemermaid::b8)<<"  " << sm->nuFlux(r,1*units.MeV,littlemermaid::n13)<<"  " << sm->nuFlux(r,1*units.MeV,littlemermaid::o15)<<"  "<< sm->nuFlux(r,1*units.MeV,littlemermaid::f17)<< std::endl;
  }

  return 0;
}
