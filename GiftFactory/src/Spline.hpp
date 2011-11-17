#ifndef __SPLINE_HPP__
#define __SPLINE_HPP__

#include "Utils.hpp"
#include "model_obj.h"

class Spline
{
public:
	Spline(const std::vector<ModelOBJ::Vertex> & vertices, int nbPoints = 10000);
	~Spline();

	double computeCoefSpline(int i, int d, const std::vector<double> & nodes, double t);


private:
	std::vector<ModelOBJ::Vertex> _splinePoints;
	int _nbPoints;
	double _step;
};

#endif