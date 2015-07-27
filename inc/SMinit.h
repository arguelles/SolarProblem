#ifndef _SMinit_
#define _SMinit_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <string>

#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>

#include "marray.h"
#include "tools.h"

class littlemermaid {
  public:
    enum FluxType {pp = 0, pep = 1, hep = 2, be7 = 3, b8 = 4, n13 = 5, o15 = 6, f17 = 7, Electon = 8, DM = 9};
	private:
    std::string SM;
		void splineinit();

		gsl_interp_accel *Racc[10];
		gsl_spline *Rspline[10];

		gsl_interp_accel *Eacc[6];
		gsl_spline *Espline[6];

	public:
		littlemermaid(std::string solarmodel):SM(solarmodel){
			splineinit();
		}
    nusquids::marray<double,2> fluxNorm;
		double nuFlux(double r, double ee, FluxType) const;
		double eDensity(double r) const;
		double DMDensity(double r) const;

    virtual ~littlemermaid();
};

#endif
