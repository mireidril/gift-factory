#include "Camera.hpp"
#include "Spline.hpp"

const float Camera::focalDistance = -0.5;
const float Camera::focalRange = 0.5;
Camera::Camera()
{
	// View Data
    // Camera position and orientation  
	GLfloat position[] = {0.0, 0.0, 3.0};
	std::cout<<"init camera"<<std::endl;

    GLfloat up[] = {0.0, 1.0, 0.0}; // Vector pointing over the camera
    lookAt(position, _aim, up);

	//tests spline 
	std::vector<Spline::PointSpline> vertices;
	Spline::PointSpline v1;
	v1.position[0] = 0;
	v1.position[1] = 0;
	v1.position[2] = 12;
	v1.nbFrames = 1;
	v1.yaw = 0;
	v1.debutRotation = false;
	v1.finRotation = false;
	vertices.push_back(v1);

	Spline::PointSpline v2;
	v2.position[0] = -2;
	v2.position[1] = 0;
	v2.position[2] = 9;
	v2.nbFrames = 1;
	v2.yaw = 0;
	v2.debutRotation = true;
	v2.finRotation = false;
	vertices.push_back(v2);

	Spline::PointSpline v3;
	v3.position[0] = 0;
	v3.position[1] = 0;
	v3.position[2] = 6;
	v3.nbFrames = 1;
	v3.yaw = -90;
	v3.debutRotation = false;
	v3.finRotation = true;
	vertices.push_back(v3);

	Spline::PointSpline v4;
	v4.position[0] = 2;
	v4.position[1] = -1;
	v4.position[2] = 5;
	v4.nbFrames = 1000;
	v4.yaw = 20;
	v4.selfRotate = true;
	v4.debutSelfRotate = 0;
	v4.finSelfRotate = 1000;
	v4.debutRotation = false;
	v4.finRotation = false;
	vertices.push_back(v4);

	Spline::PointSpline v5;
	v5.position[0] = 3;
	v5.position[1] = 0;
	v5.position[2] = 2;
	v5.nbFrames = 1;
	v5.yaw = 20;
	v5.debutRotation = true;
	v5.finRotation = false;
	vertices.push_back(v5);

	Spline::PointSpline v6;
	v6.position[0] = 1;
	v6.position[1] = 0;
	v6.position[2] = 1;
	v6.nbFrames = 1;
	v6.yaw = 45;
	v6.debutRotation = false;
	v6.finRotation = true;
	vertices.push_back(v6);

	_spline = new Spline(vertices, 5000);
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

void Camera::moveForward(){
	_spline->moveForward();
	Spline::PointSpline splinePos = _spline->getCurrentPosition();
	GLfloat cameraPos[] = {splinePos.position[0], splinePos.position[1], splinePos.position[2]};
	_aim[0] = cameraPos[0] + (_spline->getNextPosition().position[0] - _spline->getLastPosition().position[0]);
	//TODO orientation vers le haut géré aussi par la spline ? modifier le up du coup
	_aim[1] = cameraPos[1];
	_aim[2] = cameraPos[2] + (_spline->getNextPosition().position[2] - _spline->getLastPosition().position[2]);
	GLfloat vectDirect[3] = {_aim[0] - cameraPos[0], 0, _aim[2] - cameraPos[2]};
	GLfloat* vectDirectRotated = new GLfloat[3];
	vectDirectRotated[0] = vectDirect[0] * cos(_spline->getCurrentYaw()*3.1416/180) - vectDirect[2] * sin(_spline->getCurrentYaw()*3.1416/180);
	vectDirectRotated[1] = 0;
	vectDirectRotated[2] = vectDirect[0] * sin(_spline->getCurrentYaw()*3.1416/180) + vectDirect[2] * cos(_spline->getCurrentYaw()*3.1416/180);

	_aim[0] = cameraPos[0] + vectDirectRotated[0];
	_aim[1] = cameraPos[1] + vectDirectRotated[1];
	_aim[2] = cameraPos[2] + vectDirectRotated[2];

	lookAt(cameraPos, _aim, _yAxis);
}

void Camera::setPosition(GLfloat* position){
	_position[0] = position[0];
	_position[1] = position[1];
	_position[2] = position[2];
}

