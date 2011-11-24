#include "Spline.hpp"

Spline::Spline(const std::vector<PointSpline> & vertices, int nbPoints)
	:_nbPoints(nbPoints), _step(1/(double)nbPoints), _currentPosition(0), _currentPointFrame(0)
{
	std::vector<double> nodes;
	nodes.push_back(0.);
	nodes.push_back(0.);
	for (unsigned int i=2  ; i<vertices.size()+1 ; i++){
		nodes.push_back((double)(i-2)/(vertices.size()-2.));
	}
	nodes.push_back(1.);
	nodes.push_back(1.);
	double t=0;

	std::vector<std::pair<int, double>> splineCoefMax;
	for (int i = 0 ; i<vertices.size() ; i++){
		std::pair<int, double> pair;
		pair.first = 0;
		pair.second = 0;
		splineCoefMax.push_back(pair);
	}

	while (t<1){
		int nbIte = 0;
		double posX = 0;
		double posY = 0;
		double posZ = 0;
		std::vector<PointSpline>::const_iterator it;
		for (it=vertices.begin() ; it!=vertices.end() ; it++){
			double splineCoef = computeCoefSpline(nbIte, 2, nodes, t);
			posX += splineCoef * it->position[0];
			posY += splineCoef * it->position[1];
			posZ += splineCoef * it->position[2];
			if (splineCoef > splineCoefMax[nbIte].second){
				std::pair<int, double> pair;
				pair.first = _splinePoints.size();
				pair.second = splineCoef;
				splineCoefMax[nbIte] = pair;
			}
			++nbIte;
		}
		PointSpline pointSpline;
		pointSpline.position[0] = posX;
		pointSpline.position[1] = posY;
		pointSpline.position[2] = posZ;
		pointSpline.yaw = 0;
		pointSpline.roll = 0;
		pointSpline.pitch = 0;
		pointSpline.nbFrames = 1;

		_splinePoints.push_back(pointSpline);

		t +=  _step;
	}
	for (int i=0 ; i<splineCoefMax.size() ; i++){
		_splinePoints[splineCoefMax[i].first].nbFrames = vertices[i].nbFrames;
	}
}


Spline::~Spline()
{
}

/*
i : current index in the node array
d :  degree of the spline
t : current position (step)
*/
double Spline::computeCoefSpline(int i, int d, const std::vector<double> & nodes, double t){
	if (d==0){
		if (nodes[i]<=t && t<=nodes[i+1]){
			return 1;
		}
		else {
			return 0;
		}
	}
	else {
		float coef1, coef2;
		if (nodes[i+d] == nodes[i]){
			coef1 = 0;
		}
		else {
			coef1 = ((t - nodes[i]) /  (nodes[i+d] - nodes[i])) * computeCoefSpline(i, d-1, nodes, t);
		}
		if (nodes[i+d+1] == nodes [i+1]){
			coef2 = 0;
		}
		else{
			coef2 = ((nodes [i+d+1] - t) / (nodes[i+d+1] - nodes[i+1])) * computeCoefSpline(i+1, d-1, nodes, t);
		}
		return coef1 + coef2;
	}
}

void Spline::moveForward(){
	if (_currentPointFrame < _splinePoints[_currentPosition].nbFrames-1){
		_currentPointFrame++;
	}
	else {
		if (_currentPosition < _nbPoints-1){
			_currentPosition++;
			_currentPointFrame = 0;
		}
	}
}

Spline::PointSpline Spline::getCurrentPosition(){
	return _splinePoints[_currentPosition];
}

Spline::PointSpline Spline::getLastPosition(){
	if (_currentPosition>100){
		return _splinePoints[_currentPosition-100];
	}
	else{
		return _splinePoints[0];
	}
}
	
Spline::PointSpline Spline::getNextPosition(){
	if (_currentPosition<_nbPoints-101){
		return _splinePoints[_currentPosition+100];
	}
	else{
		return _splinePoints[_nbPoints-1];
	}
}