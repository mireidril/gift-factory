#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "Utils.hpp"

class Spline;
class Application;

class Camera
{
	public :
		Camera(Application* parentApp);
		~Camera();

		void lookAt(GLfloat * c, GLfloat * aim, GLfloat * up);

		void updateView();

		void moveForward();

		inline GLfloat* getView () {return _view;};

		void setPosition (GLfloat* position);
		inline GLfloat * getPosition () {return _position;};
		inline GLfloat * getAim () {return _aim;};
		
		static const float focalDistance;
		static const float focalRange;

		inline Application* getParentApp() {return m_parentApp;};

	private :
		// View Data
		GLfloat _position[3]; // Camera position 
		GLfloat _xAxis[3]; // Camera axis x : right side
		GLfloat _yAxis[3]; // Camera axis y : up
		GLfloat _zAxis[3]; // Camera axis z : backward
		GLfloat _view[16];

		GLfloat _aim[3];

		Application* m_parentApp;

		
};

#endif
