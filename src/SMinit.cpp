#include <SMinit.h>

using namespace std;
using namespace nusquids;

void littlemermaid::splineinit(){
	string rad = SM + "/radial.dat";
	string norm = SM + "/norm.dat";

	marray<double,2> fluxR = quickread(rad);
	marray<double,2> fluxNorm = quickread(norm);

	unsigned int arraysize = fluxR.extent(0);
  double XfluxRarr[arraysize];
  for(unsigned int j = 0; j < arraysize; j++){
    XfluxRarr[j] = fluxR[j][0];
  }
	for(unsigned int i = 0; i <= 8; i++){
		Racc[i] = gsl_interp_accel_alloc ();
    Rspline[i] = gsl_spline_alloc (gsl_interp_cspline, arraysize);
    double YfluxRarr[arraysize];
    for(unsigned int j = 0; j < arraysize; j++){
      if (i == 8) {
        YfluxRarr[j] = fluxR[j][2];
      } else {
        YfluxRarr[j] = fluxR[j][i+4]*fluxNorm[i][0];
      }
    }
    gsl_spline_init (Rspline[i], XfluxRarr, YfluxRarr, arraysize);
	}

	marray<double,2> ppSpec = quickread("SM/NuSpec/pp.dat");
	marray<double,2> hepSpec = quickread("SM/NuSpec/hep.dat");
	marray<double,2> b8Spec = quickread("SM/NuSpec/b8.dat");
	marray<double,2> n13Spec = quickread("SM/NuSpec/n13.dat");
	marray<double,2> o15Spec = quickread("SM/NuSpec/o15.dat");
	marray<double,2> f17Spec = quickread("SM/NuSpec/f17.dat");

	for(unsigned int i = 0; i < 6; i++){
		Eacc[i] = gsl_interp_accel_alloc ();
	}

	arraysize = ppSpec.extent(0);
  Espline[0] = gsl_spline_alloc (gsl_interp_cspline, arraysize);
	double ppE[arraysize], ppP[arraysize];
	for (unsigned int i=0; i < arraysize;i++){
		ppE[i] = ppSpec[i][0]*1e6;
		ppP[i] = ppSpec[i][1];
	}
	gsl_spline_init (Espline[0], ppE, ppP, arraysize);

	arraysize = hepSpec.extent(0);
   	Espline[1] = gsl_spline_alloc (gsl_interp_cspline, arraysize);
	double hepE[arraysize], hepP[arraysize];
	for (unsigned int i=0; i < arraysize;i++){
		hepE[i] = hepSpec[i][0]*1e6;
		hepP[i] = hepSpec[i][1];
	}
	gsl_spline_init (Espline[1], hepE, hepP, arraysize);


	arraysize = b8Spec.extent(0);
   	Espline[2] = gsl_spline_alloc (gsl_interp_cspline, arraysize);
	double b8E[arraysize], b8P[arraysize];
	for (unsigned int i=0; i < arraysize;i++){
		b8E[i] = b8Spec[i][0]*1e6;
		b8P[i] = b8Spec[i][1];
	}
	gsl_spline_init (Espline[2], b8E, b8P, arraysize);

	arraysize = n13Spec.extent(0);
   	Espline[3] = gsl_spline_alloc (gsl_interp_cspline, arraysize);
	double n13E[arraysize], n13P[arraysize];
	for (unsigned int i=0; i < arraysize;i++){
		n13E[i] = n13Spec[i][0]*1e6;
		n13P[i] = n13Spec[i][1];
	}
	gsl_spline_init (Espline[3], n13E, n13P, arraysize);

	arraysize = o15Spec.extent(0);
   	Espline[4] = gsl_spline_alloc (gsl_interp_cspline, arraysize);
	double o15E[arraysize], o15P[arraysize];
	for (unsigned int i=0; i < arraysize;i++){
		o15E[i] = o15Spec[i][0]*1e6;
		o15P[i] = o15Spec[i][1];
	}
	gsl_spline_init (Espline[4], o15E, o15P, arraysize);

	arraysize = f17Spec.extent(0);
   	Espline[5] = gsl_spline_alloc (gsl_interp_cspline, arraysize);
	double f17E[arraysize], f17P[arraysize];
	for (unsigned int i=0; i < arraysize;i++){
		f17E[i] = f17Spec[i][0]*1e6;
		f17P[i] = f17Spec[i][1];
	}
	gsl_spline_init (Espline[5], f17E, f17P, arraysize);

	free(ppE);
	free(ppP);
	free(hepE);
	free(hepP);
	free(b8E);
	free(b8P);
	free(n13E);
	free(n13P);
	free(o15E);
	free(o15P);
	free(f17E);
	free(f17P);
}

double littlemermaid::nuFlux(double R, double E, FluxType type) const{
	if (R > 1.0 || R < 0.0)
    throw std::runtime_error("Invalid R, must be between 0 and 1");
	if (E < 3.4640e3 || E > 1.8784e07){
		return 0.0;
  }

	switch(type){
		case pp:
			if (E > 0.00504e6 && E < 0.42341e6){
				return gsl_spline_eval(Rspline[0],R,Racc[0])*gsl_spline_eval(Espline[0],E,Eacc[0]);
			}
			else return 0.0;
		case pep:
			if (E == 1.44e6){
				return gsl_spline_eval(Rspline[1],R,Racc[1]);
			}
			else return 0.0;
		case hep:
			if (E > 1.8784e4 && E < 1.8784e7){
				return gsl_spline_eval(Rspline[2],R,Racc[2])*gsl_spline_eval(Espline[1],E,Eacc[1]);
			}
			else return 0.0;
		case be7:
			if (E == .8618e6){
				return gsl_spline_eval (Rspline[3], R, Racc[3]);  //NEED NORM FOR EACH LINE
			}
      else if (E == .3843e6){
				return gsl_spline_eval (Rspline[3], R, Racc[3]);
			}
			else return 0.0;
		case b8:
			if (E > 0.02e6 && E < 16.56e6){
				return gsl_spline_eval(Rspline[4],R,Racc[4])*gsl_spline_eval(Espline[2],E,Eacc[2]);
			}
			else return 0.0;
		case n13:
			if (E > 5.9950e3 && E < 1.1990e6){
				return gsl_spline_eval(Rspline[5],R,Racc[5])*gsl_spline_eval(Espline[3],E,Eacc[3]);
			}
			else return 0.0;
		case o15:
			if (E > 3.4640e3 && E < 1.7320e6){
				return gsl_spline_eval(Rspline[6],R,Racc[6])*gsl_spline_eval(Espline[4],E,Eacc[4]);
			}
			else return 0.0;
		case f17:
			if (E > 3.4800e3 && E < 1.7400e6){
				return gsl_spline_eval(Rspline[7],R,Racc[7])*gsl_spline_eval(Espline[5],E,Eacc[5]);
			}
			else return 0.0;
		default:
      throw std::runtime_error("Incorrect type, pp-0, pep-1, hep-2, be7-3, b8-4, N13-5, O15-6, F17-7");
    }
}

double littlemermaid::eDensity(double R) const {
	if (R > 1.0 || R < 0.0){
    throw std::runtime_error("Invalid R, must be between 0 and 1");
  }
	return exp(gsl_spline_eval(Rspline[8],R,Racc[8]));
}

double littlemermaid::DMDensity(double R) const {
	if (R > 1.0 || R < 0.0){
		throw std::runtime_error("Invalid R, must be between 0 and 1");
  }
  std::cout << "Not implimented yet" << std::endl;
	return 0.0;
}

littlemermaid::~littlemermaid(){
	for(unsigned int i = 0; i < 9; i++){
		gsl_spline_free(Rspline[i]);
  		gsl_interp_accel_free(Racc[i]);
  }
	for(unsigned int i = 0; i < 6; i++){
		gsl_interp_accel_free(Eacc[i]);
		gsl_spline_free(Espline[i]);
	}
	free(Rspline);
	free(Racc);
	free(Espline);
	free(Eacc);
}

