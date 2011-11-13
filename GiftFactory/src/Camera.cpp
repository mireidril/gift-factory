#include "Camera.hpp"

Camera::Camera()
{
	// View Data
    // Camera position and orientation  
    GLfloat position[] = {0.0, 0.0, 1.0};

	GLfloat aim[] = {0.0, 0.0, 0.0};

    GLfloat up[] = {0.0, 1.0, 0.0}; // Vector pointing over the camera
    lookAt(_position, aim, up);

    // Projection data
    // Projection type : perspective:true / orthographic:false
   _perspectiveProjection = true;
    // Projection frustum data
    GLfloat l = 1.0;
    setProjectionData(-l, l, -l, l, 0.1, 250.0);
	setToIdentity(_projection);
    updateProjection();
}

Camera::~Camera()
{
}

void Camera::setProjectionData(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearCam, GLfloat farCam){
	_left = left;
    _right = right;
    _bottom = bottom;
    _top = top;
	_near = nearCam;
	_far = farCam;
}

void Camera::switchCameraProjection(){
	std::cout<<"Camera projection : ";
	if (_perspectiveProjection)
    {
        std::cout<<"orthographic"<<std::endl;
		_perspectiveProjection=false;
    }
	else
    {
        std::cout<<"perspective"<<std::endl;
		_perspectiveProjection=true;
    }

    // Changes the matrix accordingly
    updateProjection();
}

void Camera::lookAt(GLfloat * position, GLfloat * aim, GLfloat * up)
{
    for (GLuint iCoord=0 ; iCoord<3; iCoord++)
    {
        _position[iCoord]=position[iCoord]; // c : camera position
        _yAxis[iCoord]=up[iCoord]; // y : up vector
        _zAxis[iCoord]=position[iCoord]-aim[iCoord]; // z : from aim to camera position
    }
    // Opposite direction of axis z
    GLfloat minusZ[]={-_zAxis[0], -_zAxis[1], -_zAxis[2]};
    // axis x  : from axis y and axis z
    vectorProduct(_yAxis, _zAxis, _xAxis);
    // axis y  : from new axis x and opposite of axis z
    vectorProduct(_xAxis, minusZ, _yAxis);
    // Normalizes all axis vectors
    normalize(_xAxis);
    normalize(_yAxis);
    normalize(_zAxis);

    // Builds the new view matrix
    updateView();
}

// Updates view
void Camera::updateView()
{
    // Rotation to be aligned with correct camera axis
    GLfloat RcInv[]={_xAxis[0], _yAxis[0], _zAxis[0], 0.0,
                     _xAxis[1], _yAxis[1], _zAxis[1], 0.0,
                     _xAxis[2], _yAxis[2], _zAxis[2], 0.0,
                     0.0,          0.0,          0.0,          1.0};

    // Translation to be at the right distance from the scene
    GLfloat TcInv[]={1.0,           0.0,           0.0,           0.0,
                     0.0,           1.0,           0.0,           0.0,
                     0.0,           0.0,           1.0,           0.0,
                     -_position[0], -_position[1], -_position[2], 1.0};

    // Initializes
    setToIdentity(_view);
    // Rotates
    multMatrixBtoMatrixA(_view, RcInv);  
    // Translates
    multMatrixBtoMatrixA(_view, TcInv); 
}

// Set the perspective like gluPerspective
void Camera::setPerspectiveFromAngle(GLfloat fovy, GLfloat aspectRatio)
{
    _top = _near * tan(fovy/2.0);
    _bottom = - _top;
    _left = _bottom * aspectRatio;
    _right = _top * aspectRatio;
    
    updateProjection();
}

// Updates the projection matrix from the data
void Camera::updateProjection()
{  
    GLfloat l =_left;
    GLfloat r =_right;
    GLfloat b =_bottom;
    GLfloat t =_top;
    GLfloat n =_near;
    GLfloat f =_far;

    if (_perspectiveProjection) // Perspective projection
    {
        GLfloat P[]={ (2.0*n)/(r-l), 0.0,           0.0,              0.0,
                      0.0,           (2.0*n)/(t-b), 0.0,              0.0,
                      (r+l)/(r-l),   (t+b)/(t-b),   -(f+n)/(f-n),    -1.0,
                      0.0,           0.0,           -(2.0*f*n)/(f-n), 0.0}; // Perspective projection
        for (int iMatrixCoord=0 ; iMatrixCoord<16 ; iMatrixCoord++)
            _projection[iMatrixCoord]=P[iMatrixCoord];
    }
    else // Orthographic projection
    { 
        GLfloat P[]={ 2.0/(r-l),   0.0,         0.0,         0.0,
                      0.0,         2.0/(t-b),   0.0,         0.0,
                      0.0,         0.0,         -2.0/(f-n),  0.0,
                    -(r+l)/(r-l), -(t+b)/(t-b), -(f+n)/(f-n), 1.0}; // Orthographic projection
        for (int iMatrixCoord=0 ; iMatrixCoord<16 ; iMatrixCoord++)
            _projection[iMatrixCoord]=P[iMatrixCoord];
    }
}

