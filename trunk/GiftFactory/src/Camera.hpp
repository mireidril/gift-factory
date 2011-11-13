#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "Utils.hpp"

class Camera
{
	public :
		Camera();
		~Camera();

		/*void setProjectionData(Camera * camera, GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far);
		void switchCameraProjection(Camera * camera);

		void lookAt(Camera * camera, GLfloat * c, GLfloat * aim, GLfloat * up);
		void setPerspectiveFromAngle(Camera * camera, GLfloat fovy, GLfloat aspectRatio);

		void updateView(Camera * camera);
		void updateProjection(Camera * camera);

		void deleteCamera(Camera * camera);
		

	private :
		// View Data
		GLfloat c[3]; // Camera position 
		GLfloat x[3]; // Camera axis x : right side
		GLfloat y[3]; // Camera axis y : up
		GLfloat z[3]; // Camera axis z : backward
		GLfloat view[16];

		GLfloat aim[3];

		// Projection data
		bool perspectiveProjection; // persepective projection:true / orthographic projection:false
		GLfloat left; // x coord from center to left plane of frustum
		GLfloat right; // x coord from center to right plane of frustum
		GLfloat bottom; // y coord from center to bottom plane of frustum
		GLfloat top; // y coord from center to top plane of frustum
		GLfloat near; // z coord from c to near plane of frustum
		GLfloat far; // z coord from c to far plane of frustum
		GLfloat projection[16];*/
};

#endif
