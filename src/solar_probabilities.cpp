#include <solar_probabilities.h>

using namespace squids;
using namespace nusquids;

double SOP::SolarOscillationProbability(double E,double r) const {
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
  double density = 1;
  double ye = 0.5;

  SU_vector H = DM2*(1./(2.*E));
  double CC = params->sqrt2*params->GF*params->Na*pow(params->cm,-3)*density*ye;
  H += CC*b1_proj[nue];

  return std::move(H);
}

