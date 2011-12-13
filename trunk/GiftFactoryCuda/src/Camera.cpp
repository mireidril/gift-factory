#include "Camera.hpp"
#include "Spline.hpp"
#include "Application.hpp"
#include "Scene.hpp"

const float Camera::focalDistance = -0.5;
const float Camera::focalRange = 0.5;
Camera::Camera(Application* parentApp)
{
	// View Data
    // Camera position and orientation  
	GLfloat position[] = {0.0, 0.0, 3.0};
	std::cout<<"init camera"<<std::endl;
	_aim[0] = 0.0;
	_aim[1] = 0.0;
	_aim[2] = 0.0;
    GLfloat up[] = {0.0, 1.0, 0.0}; // Vector pointing over the camera
    lookAt(position, _aim, up);

	m_parentApp = parentApp;
}

Camera::~Camera()
{
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
	//std::cout<<_position[0]<<" "<<_position[1]<<" "<<_position[2]<<std::endl;
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

bool Camera::moveForward(){
	Spline* spline = m_parentApp->getRenderedScene()->getSpline();
	bool splineEnded = spline->moveForward();
	Spline::PointSpline splinePos = spline->getCurrentPosition();
	GLfloat cameraPos[] = {splinePos.position[0], splinePos.position[1], splinePos.position[2]};
	_aim[0] = cameraPos[0] + (spline->getNextPosition().position[0] - spline->getLastPosition().position[0]);
	//TODO orientation vers le haut géré aussi par la spline ? modifier le up du coup
	_aim[1] = cameraPos[1];
	_aim[2] = cameraPos[2] + (spline->getNextPosition().position[2] - spline->getLastPosition().position[2]);
	GLfloat vectDirect[3] = {_aim[0] - cameraPos[0], 0, _aim[2] - cameraPos[2]};
	GLfloat* vectDirectRotated = new GLfloat[3];
	vectDirectRotated[0] = vectDirect[0] * cos(spline->getCurrentYaw()*3.1416/180) - vectDirect[2] * sin(spline->getCurrentYaw()*3.1416/180);
	vectDirectRotated[1] = 0;
	vectDirectRotated[2] = vectDirect[0] * sin(spline->getCurrentYaw()*3.1416/180) + vectDirect[2] * cos(spline->getCurrentYaw()*3.1416/180);

	_aim[0] = cameraPos[0] + vectDirectRotated[0];
	_aim[1] = cameraPos[1] + vectDirectRotated[1];
	_aim[2] = cameraPos[2] + vectDirectRotated[2];

	lookAt(cameraPos, _aim, _yAxis);

	return splineEnded;
}

void Camera::setPosition(GLfloat* position){
	_position[0] = position[0];
	_position[1] = position[1];
	_position[2] = position[2];
}

