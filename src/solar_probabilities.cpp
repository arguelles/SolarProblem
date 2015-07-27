#include <solar_probabilities.h>

using namespace squids;
using namespace nusquids;

double SOP::SolarOscillationProbability(double E,double r) const {
  if (params == nullptr)
    throw std::runtime_error("No oscillation parameters set");
  if (solar_model == nullptr)
    throw std::runtime_error("No solar model set");

  SU_vector h = Hamiltonian(E,r);
  auto eigensyst = h.GetEigenSystem();
  auto UPMNS = params->GetTransformationMatrix(numneu);

  double osc_prob = 0;
  for(unsigned int i = 0; i < numneu; i++){
    osc_prob += gsl_complex_abs2(gsl_matrix_complex_get(eigensyst.second.get(),nue,i))*\
                gsl_complex_abs2(gsl_matrix_complex_get(UPMNS.get(),nue,i));
  }
  return osc_prob;
}

SU_vector SOP::Hamiltonian(double E, double r) const {
  double electron_number_density = solar_model->eDensity(r);

  SU_vector H = DM2*(1./(2.*E));
  //double density = 1.;
  //double ye = 0.5;
  //double CC = CC_prefactor*density*ye;
  double CC = CC_prefactor*electron_number_density;
  H += CC*b1_proj[nue];

  return std::move(H);
}

