#include "Camera.hpp"
#include "Spline.hpp"

Camera::Camera()
{
	// View Data
    // Camera position and orientation  
	GLfloat position[] = {0.0, 0.0, 3.0};
	std::cout<<"init camera"<<std::endl;
	GLfloat aim[] = {0.0, 0.0, 0.0};

    GLfloat up[] = {0.0, 1.0, 0.0}; // Vector pointing over the camera
    lookAt(position, aim, up);

	//tests spline 
	std::vector<ModelOBJ::Vertex> vertices;
	ModelOBJ::Vertex v1;
	v1.position[0] = 1;
	v1.position[1] = 1;
	v1.position[2] = 1;
	vertices.push_back(v1);
	ModelOBJ::Vertex v2;
	v2.position[0] = 3;
	v2.position[1] = 3;
	v2.position[2] = 3;
	vertices.push_back(v2);
	ModelOBJ::Vertex v3;
	v3.position[0] = 4;
	v3.position[1] = 6;
	v3.position[2] = 4;
	vertices.push_back(v3);
	ModelOBJ::Vertex v4;
	v4.position[0] = 9;
	v4.position[1] = 9;
	v4.position[2] = 12;
	vertices.push_back(v4);
	ModelOBJ::Vertex v5;
	v5.position[0] = 11;
	v5.position[1] = 11;
	v5.position[2] = 11;
	vertices.push_back(v5);
	_spline = new Spline(vertices, 20);
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
}

// Updates view
void Camera::updateView()
{
	std::cout<<"position "<<_position[0]<<" "<<_position[1]<<" "<<_position[2]<<std::endl;
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




