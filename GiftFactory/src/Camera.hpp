#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "Utils.hpp"

class Spline;

class Camera
{
	public :
		Camera();
		~Camera();

		void lookAt(GLfloat * c, GLfloat * aim, GLfloat * up);

		void updateView();

		inline GLfloat* getView () {return _view;};

	private :
		// View Data
		GLfloat _position[3]; // Camera position 
		GLfloat _xAxis[3]; // Camera axis x : right side
		GLfloat _yAxis[3]; // Camera axis y : up
		GLfloat _zAxis[3]; // Camera axis z : backward
		GLfloat _view[16];

		GLfloat _aim[3];

		Spline* _spline;
};

#endif
