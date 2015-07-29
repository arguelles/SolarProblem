#ifndef _solar_probabilities_
#define _solar_probabilities_

#include <SQuIDS/const.h>
#include <SQuIDS/SUNalg.h>
#include <gsl/gsl_complex_math.h>
#include <gsl/gsl_integration.h>
#include "marray.h"
#include "SMinit.h"

template<typename FunctionType>
double integrate(FunctionType f, double a, double b){
	double (*wrapper)(double,void*)=[](double x, void* params){
		FunctionType& f=*static_cast<FunctionType*>(params);
		return(f(x));
	};

	gsl_integration_workspace* ws=gsl_integration_workspace_alloc(10000);
	double result, error;
	gsl_function F;
	F.function = wrapper;
	F.params = &f;

	gsl_integration_qags(&F, a, b, 0, 1e-2, 10000, ws, &result, &error);
	gsl_integration_workspace_free(ws);

	return(result);
}

class SOP {
  private:
    /// \brief number of neutrino flavors.
    const unsigned int numneu = 3;
    /// \brief flavors
    enum flavors {nue = 0,numu = 1,nutau = 2};
    /// \brief mixing parameters
    std::shared_ptr<squids::Const> params = nullptr;
    /// \brief solar model object
    std::shared_ptr<littlemermaid> solar_model = nullptr;
    /// \brief CC interaction prefactor
    double CC_prefactor = -1;
    /// \brief Set std CC prefactor
    void SetPotentialPrefactor(){
      CC_prefactor = params->sqrt2*params->GF*params->Na*pow(params->cm,-3);
    }
    /// \brief PMNS matrix
    std::shared_ptr<gsl_matrix_complex> UPMNS = nullptr;
  protected:
    squids::SU_vector DM2;
    /// \details The i-entry corresponds to the projector in the ith mass eigenstate.
    nusquids::marray<squids::SU_vector,1> b0_proj;
    /// \brief Flavor basis projectors.
    /// \details The first dismension corresponds to the neutrino type. When NeutrinoType = both, then
    /// the first dimension equal 0 means neutrinos and 1 antineutrinos. The second dimension corresponds
    /// to the flavor eigenstates where 0 corresponds to nu_e, 1 to nu_mu, 2 to nu_tau, and the others
    /// to sterile neutrinos.
    nusquids::marray<squids::SU_vector,1> b1_proj;
    /// \brief Resets the vacuum hamiltonian square mass difference matrix
    void SetVacuumHamiltonian(){
      DM2 = squids::SU_vector(numneu);
      for(unsigned int i = 1; i < numneu; i++){
          DM2 += (b0_proj[i])*params->GetEnergyDifference(i);
      }
    }
    /// \brief Resets the flavor projectors
    void SetFlavorProjectors(){
      b1_proj.resize(std::vector<size_t>{numneu});
      for(unsigned int flv = 0; flv < numneu; flv++){
        b1_proj[flv] = squids::SU_vector::Projector(numneu,flv);
        b1_proj[flv].RotateToB1(*params);
      }
    }
    squids::SU_vector Hamiltonian(double E, double r) const;
  public:
    SOP(){
      b0_proj.resize(std::vector<size_t>{numneu});
        for(unsigned int flv = 0; flv < numneu; flv++){
          b0_proj[flv] = squids::SU_vector::Projector(numneu,flv);
        }
    }

    void SetMixingParameters(std::shared_ptr<squids::Const> params_){
      params = params_;
      UPMNS = params->GetTransformationMatrix(numneu);
      //gsl_matrix_complex_print(UPMNS.get());
      SetPotentialPrefactor();
      SetVacuumHamiltonian();
      SetFlavorProjectors();
    }

    void SetMixingParameters(){
      if (params == nullptr)
        throw std::runtime_error("No oscillation parameters set");
      UPMNS = params->GetTransformationMatrix(numneu);
      SetPotentialPrefactor();
      SetVacuumHamiltonian();
      SetFlavorProjectors();
    }

    std::shared_ptr<squids::Const> GetMixingParameters() const{
      if (params == nullptr)
        throw std::runtime_error("No oscillation parameters set");
      return params;
    }

    void SetSolarModel(std::shared_ptr<littlemermaid> solar_model_){
      solar_model = solar_model_;
    }

    std::shared_ptr<littlemermaid> GetSolarModel(){
      if (solar_model == nullptr)
        throw std::runtime_error("No solar model set");
      return solar_model;
    }

    double SolarOscillationProbability(double E,double r) const;
    double PeeCuadradito(double E) const;
    double RadialIntegratedFluxes(double E) const;
};

#endif
