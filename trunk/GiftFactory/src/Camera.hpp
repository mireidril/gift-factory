#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "Utils.hpp"

class Camera
{
	public :
		Camera();
		~Camera();

		void setProjectionData(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearCam, GLfloat farCam);
		void switchCameraProjection();

		void lookAt(GLfloat * c, GLfloat * aim, GLfloat * up);
		void setPerspectiveFromAngle(GLfloat fovy, GLfloat aspectRatio);

		void updateView();
		void updateProjection();

	private :
		// View Data
		GLfloat _position[3]; // Camera position 
		GLfloat _xAxis[3]; // Camera axis x : right side
		GLfloat _yAxis[3]; // Camera axis y : up
		GLfloat _zAxis[3]; // Camera axis z : backward
		GLfloat _view[16];

		GLfloat _aim[3];

		// Projection data
		bool _perspectiveProjection; // persepective projection:true / orthographic projection:false
		GLfloat _left; // x coord from center to left plane of frustum
		GLfloat _right; // x coord from center to right plane of frustum
		GLfloat _bottom; // y coord from center to bottom plane of frustum
		GLfloat _top; // y coord from center to top plane of frustum
		GLfloat _near; // z coord from c to near plane of frustum
		GLfloat _far; // z coord from c to far plane of frustum
		GLfloat _projection[16];
};

#endif
