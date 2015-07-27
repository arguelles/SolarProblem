#include <SQuIDS/Const.h>
#include <SQuIDS/SUNalg.h>
#include <gsl/gsl_complex_math.h>
#include "marray.h"

class SOP {
  private:
    /// \brief number of neutrino flavors.
    const unsigned int numneu = 3;
    /// \brief flavors
    enum flavors {nue = 0,numu = 1,nutau = 2};
    /// \brief mixing parameters
    std::shared_ptr<squids::Const> params;
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
  public:
    SOP(){
      b0_proj.resize(std::vector<size_t>{numneu});
        for(unsigned int flv = 0; flv < numneu; flv++){
          b0_proj[flv] = squids::SU_vector::Projector(numneu,flv);
        }
    }
    void SetMixingParameters(std::shared_ptr<squids::Const> params_){
      params = params_;
      SetVacuumHamiltonian();
      SetFlavorProjectors();
    }
    void SetMixingParameters(){
      SetVacuumHamiltonian();
      SetFlavorProjectors();
    }
    std::shared_ptr<squids::Const> GetMixingParameters(){
      return params;
    }
    squids::SU_vector Hamiltonian(double E, double r) const;
    double SolarOscillationProbability(double E,double r) const;
};

