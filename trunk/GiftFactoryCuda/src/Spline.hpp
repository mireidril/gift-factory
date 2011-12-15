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
		//GLfloat pitch;
		//rotation around lookAt axis
		//GLfloat roll;
		bool debutRotation;
		bool finRotation;
		bool selfRotate;
		GLuint debutSelfRotate;
		GLuint finSelfRotate;
		GLuint nbFrames;
	};

	Spline(std::string file_spline);
	virtual ~Spline();

	double computeCoefSpline(int i, int d, const std::vector<double> & nodes, double t);

	bool moveForward();

	PointSpline getCurrentPosition();
	PointSpline getLastPosition();
	PointSpline getNextPosition();

	inline GLfloat getCurrentYaw(){return _currentYaw;}; 

private:
	std::vector<PointSpline> _splinePoints;
	int _nbPoints;
	double _step;

	GLuint _currentPosition;
	GLuint _currentPointFrame;
	GLfloat _currentYaw;
};

#endif