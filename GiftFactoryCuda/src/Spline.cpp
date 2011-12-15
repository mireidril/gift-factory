#include "Spline.hpp"

Spline::Spline(std::string file_spline)
	: _currentPosition(0), _currentPointFrame(0)
{
	//creer le tableau a partir du fichier

	std::stringstream buffer; std::string line;
	std::fstream stream(file_spline.c_str(), std::ios::in);
	int numPoints = 0;
	
	while(std::getline(stream, line)) numPoints++;
	
	stream.clear(); stream.seekg(0, std::ios::beg);	// Rewind the file

	//first and second lines are ignored for the comment
	getline(stream, line);
	getline(stream, line);
	//third line for the nb of points
	getline(stream, line); 
	buffer.clear(); buffer.str(line);
	buffer >> _nbPoints; 
	_step = 1/(double)_nbPoints;

	std::vector<Spline::PointSpline> vertices;

	for(int n = 0; n<numPoints-3; n++){
		getline(stream, line); // get the ligne
		buffer.clear(); buffer.str(line);

		Spline::PointSpline v;
		bool testBoolean;

		buffer >> v.position[0];
		buffer >> v.position[1];
		buffer >> v.position[2];

		v.position[0] /=100.0;
		v.position[1] /=100.0;
		v.position[2] /=100.0;
		//std::cout<<v.position[0]<<" "<<v.position[1]<<" "<<v.position[2]<<std::endl;
		buffer >> v.yaw;
		buffer >> testBoolean;
		if (testBoolean == 0) v.debutRotation = false;
		else v.debutRotation = true;
		buffer >> testBoolean;
		if (testBoolean == 0) v.finRotation = false;
		else v.finRotation = true;

		buffer >> testBoolean;
		if (testBoolean == 0) v.selfRotate = false;
		else v.selfRotate = true;

		buffer >> v.debutSelfRotate;
		buffer >> v.finSelfRotate;

		buffer >> v.nbFrames;

		vertices.push_back(v);

	}

	std::vector<double> nodes;
	nodes.push_back(0.);
	nodes.push_back(0.);
	for (unsigned int i=2  ; i<vertices.size()+1 ; i++){
		nodes.push_back((double)(i-2)/(vertices.size()-2.));
	}
	nodes.push_back(1.);
	nodes.push_back(1.);
	double t=0;

	std::vector<std::pair<int, double> > splineCoefMax;
	for (unsigned int i = 0 ; i<vertices.size() ; i++){
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
		pointSpline.nbFrames = 1;
		_splinePoints.push_back(pointSpline);
		t +=  _step;

	}

	for (unsigned int i=0 ; i<splineCoefMax.size() ; i++){
		_splinePoints[splineCoefMax[i].first].nbFrames = vertices[i].nbFrames;
		_splinePoints[splineCoefMax[i].first].selfRotate = vertices[i].selfRotate;
		_splinePoints[splineCoefMax[i].first].debutSelfRotate = vertices[i].debutSelfRotate;
		_splinePoints[splineCoefMax[i].first].finSelfRotate = vertices[i].finSelfRotate;
		if (i+1<splineCoefMax.size()){
			for (unsigned int j=splineCoefMax[i].first+1 ; j<splineCoefMax[i+1].first+1 ; j++){
				_splinePoints[j].yaw = vertices[i].yaw;
			}
		}
	}

	for (unsigned int i=0 ; i<splineCoefMax.size() ; i++){
		if (vertices[i].debutRotation){
			int j=i;
			while (!vertices[j].finRotation){
				j++;
			}
			GLfloat coefDivise = splineCoefMax[j].first - splineCoefMax[i].first + 1;
			GLfloat yawBegin = vertices[i].yaw;
			GLfloat yawEnd = vertices[j].yaw;
			for (int kPointSpline = 0 ; kPointSpline < coefDivise ; kPointSpline++){
				_splinePoints[splineCoefMax[i].first + kPointSpline].yaw = ((yawEnd - yawBegin) / coefDivise) * kPointSpline + yawBegin;
			}
		}
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


//returns false if we are at the end of the spline
bool Spline::moveForward(){
	if (_currentPointFrame < _splinePoints[_currentPosition].nbFrames-1){
		if (_splinePoints[_currentPosition].selfRotate){
			if (_currentPointFrame >= _splinePoints[_currentPosition].debutSelfRotate && _currentPointFrame <= _splinePoints[_currentPosition].finSelfRotate){
				GLfloat coefDivise =  _splinePoints[_currentPosition].finSelfRotate - _splinePoints[_currentPosition].debutSelfRotate + 1;
				GLfloat yawBegin = _splinePoints[_currentPosition].yaw;
				GLfloat yawEnd = _splinePoints[_currentPosition+1].yaw;
				_currentYaw = ((yawEnd - yawBegin) / coefDivise) * (_currentPointFrame - _splinePoints[_currentPosition].debutSelfRotate) + yawBegin;
			}
			else if (_currentPointFrame < _splinePoints[_currentPosition].debutSelfRotate){
				_currentYaw = _splinePoints[_currentPosition].yaw;
			}
		}
		else {
			_currentYaw = _splinePoints[_currentPosition].yaw;
		}
		_currentPointFrame++;
		return true;
	}
	else {
		if (_currentPosition < _nbPoints-1){
			_currentPosition++;
			_currentPointFrame = 0;
			_currentYaw = _splinePoints[_currentPosition].yaw;
			return true;
		}
		else {
			return false;
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