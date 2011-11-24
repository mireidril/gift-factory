#ifndef __SPLINE_HPP__
#define __SPLINE_HPP__

#include "Utils.hpp"
#include "model_obj.h"

class Spline
{
public:

	struct PointSpline{
		GLfloat position[3];
		//rotation around up axis
		GLfloat yaw;
		//rotation around left axis
		GLfloat pitch;
		//rotation around lookAt axis
		GLfloat roll;
		GLuint nbFrames;
	};

	Spline(const std::vector<PointSpline> & vertices, int nbPoints = 10000);
	~Spline();

	double computeCoefSpline(int i, int d, const std::vector<double> & nodes, double t);

	void moveForward();

	PointSpline getCurrentPosition();
	PointSpline getLastPosition();
	PointSpline getNextPosition();

private:
	std::vector<PointSpline> _splinePoints;
	int _nbPoints;
	double _step;

	GLuint _currentPosition;
	GLuint _currentPointFrame;
};

#endif