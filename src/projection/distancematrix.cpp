/*!	@file distancematrix.cpp
 *	@author anantonov
 *	@date	Oct 22, 2012 (created)
 *	@brief	Implementation of distance matrix class
 */

#include "distancematrix.h"
#include <iostream>
#include <cstdio>
#include <cassert>
#include <cmath>
#include <cstring>

namespace VCGL {

DistanceMatrix::DistanceMatrix(const strType& matrixID, const std::vector<strType>& objectIDs)
: dmatID(matrixID), objIDs(objectIDs) {
	objClasses.resize(objIDs.size(), 0.0f);
	//rows in 1 .. N-1, cols 0 .. (row-1)
	distances.resize( objIDs.size()-1, std::vector<float>() );
	for (unsigned j=1; j<objIDs.size(); j++) {
		distances[j-1].resize(j);
	}
}

void DistanceMatrix::findObjectIndices(std::vector<strType> objIDArray, std::vector<unsigned>& outObjIndices) const {

	outObjIndices.clear();
	outObjIndices.resize(objIDArray.size(), 0);

	for (unsigned i=0; i<objIDArray.size(); i++) {
		bool found = false;
		for (unsigned j=0; !found && j<objIDs.size(); j++ ) {
			if (objIDs[j] == objIDArray[i]) {
				found = true;
				outObjIndices[i] = j;
			}
		}
	}
}

float DistanceMatrix::getDistanceByIndices(unsigned objIndex, unsigned otherObjIndex ) const {
	unsigned id1 = objIndex;
	unsigned id2 = otherObjIndex;
	unsigned row = 0;
	unsigned col = 0;
	if (id1 > id2) {
		row = id1 - 1;
		col = id2;
	}
	else {
		row = id2 - 1;
		col = id1;
	}
	return distances[row][col];
}

float
DistanceMatrix::getDistance(const strType& objID, const strType& otherObjID ) const {
	unsigned row = 0;
	unsigned col = 0;
	findIndices(objID, otherObjID, row,col);
	return distances[row][col];
}

void
DistanceMatrix::setDistance(const strType& objID, const strType& otherObjID, float distance) {
	unsigned row = 0;
	unsigned col = 0;
	findIndices(objID, otherObjID, row, col);
	distances[row][col] = distance;
}

strType
DistanceMatrix::id() const {
	return dmatID;
}
void
DistanceMatrix::printDMAT(std::ostream& output) const {
	const unsigned n = objIDs.size();

	// first line: number of objects
	output << n << std::endl;

	// second line: objects' IDs
	for (unsigned j=0; j<n; j++) {
		output << objIDs[j];
		if (j<n-1) {
			output << ';';
		}
		else {
			output << std::endl;
		}
	}

	//third line: objects' classes
	for (unsigned j=0; j<n; j++) {
		output << objClasses[j];
		if (j<n-1) {
			output << ';';
		}
		else {
			output << std::endl;
		}
	}

	//the matrix itself
	for (unsigned row=0; row<distances.size(); row++) {
		const unsigned ncols =  distances[row].size();
		for (unsigned col=0; col<ncols; col++) {
			output << distances[row][col];
			if (col<ncols-1) {
				output << ';';
			}
			else {
				output << std::endl;
			}
		}
	}

}

void
DistanceMatrix::readDMAT(std::istream& input, strType matrixID, DistanceMatrix** ppOutMatrix) {
	*ppOutMatrix = 0;

	unsigned n = 0;
	// first line: number of objects
	input >> n;
	input.ignore(1);

	// second line: objects' IDs
	std::vector<std::string> objIDs;
	objIDs.resize(n, "");
	const int bufSize = 100;
	char buffer[bufSize];
	for (unsigned j=0; j<n; j++) {
		if (j<n-1) {
			input.getline(buffer, bufSize,';');
		}
		else {
			input.getline(buffer, bufSize);
		}
		objIDs[j] = buffer;
	}

	DistanceMatrix* dOut = new DistanceMatrix(matrixID, objIDs);

	//third line: objects' classes
	for (unsigned j=0; j<n; j++) {
		if (j<n-1) {
			input.getline(buffer, bufSize,';');
		}
		else {
			input.getline(buffer, bufSize);
		}
		sscanf(buffer, "%f", &dOut->objClasses[j]);
	}


	//the matrix itself
	for (unsigned row=0; row< dOut->distances.size(); row++) {
		const unsigned ncols =  dOut->distances[row].size();
		for (unsigned col=0; col<ncols; col++) {
			if (col<ncols-1) {
				input.getline(buffer, bufSize,';');
			}
			else {
				input.getline(buffer, bufSize);
			}
			sscanf(buffer, "%f", &dOut->distances[row][col]);
		}
	}
	*ppOutMatrix = dOut;
}

void DistanceMatrix::fromCorrelationMatrixArray(const std::vector< std::vector<float> >& correlations,
			int nx,
			int ny,
			strType matrixID,
			DistanceMatrix** ppOutMatrix) {

	*ppOutMatrix = 0;

	// first line: number of objects
	unsigned n = nx*ny;

	assert(correlations.size() == n);
	assert(correlations[0].size() == n);

	// second line: objects' IDs
	std::vector<std::string> objIDs;
	objIDs.resize(n, "");

	const int bufSize = 100;
	char buffer[bufSize];
	for (unsigned j=0; j<n; j++) {
		int ptY = j / nx;
		int ptX = j % nx;
		sprintf(buffer, "(%d,%d)", ptX, ptY);
		objIDs[j] = buffer;
	}

	DistanceMatrix* dOut = new DistanceMatrix(matrixID, objIDs);

	//third line: objects' classes
	for (unsigned j=0; j<n; j++) {
		dOut->objClasses[j] = 1.0;
	}


	//the matrix itself
	for (unsigned row=0; row< dOut->distances.size(); row++) {
		const unsigned ncols =  dOut->distances[row].size();
		for (unsigned col=0; col<ncols; col++) {
			//dOut->distances[row][col] = 1-fabs(correlations[row][col]); // differentiates only between strong/weak correlation, ignores the sign
			dOut->distances[row][col] = (1.0-correlations[row][col])/2.0; // puts positively correlated points closer and negatively correlated farther
		}
	}
	*ppOutMatrix = dOut;

}

void
DistanceMatrix::getObjectIDs(std::vector<strType>& outObjIDs) const {
	outObjIDs = objIDs;
}

float
DistanceMatrix::getObjectClass(const strType& objID) const {
	float result = 0.0f;
	for (unsigned j=0; j<objIDs.size(); j++) {
		if (objIDs[j] == objID) {
			result = objClasses[j];
			break;
		}
	}
	return result;
}

void
DistanceMatrix::setObjectClass(const strType& objID, float newClass) {
	for (unsigned j=0; j<objIDs.size(); j++) {
		if (objIDs[j] == objID) {
			objClasses[j] = newClass;
			break;
		}
	}
}

void
DistanceMatrix::findIndices(const strType& objID, const strType& otherObjID, unsigned& row, unsigned& col) const {
	int id1 = -1;
	int id2 = -1;
	for (unsigned j=0; (id1 < 0 || id2 < 0 ) && j<objIDs.size(); j++ ) {
		if (id1 < 0 && objIDs[j] == objID) {
			id1 = j;
		}
		if (id2 < 0 && objIDs[j] == otherObjID) {
			id2 = j;
		}
	}
	if (id1 > id2) {
		row = id1 - 1;
		col = id2;
	}
	else {
		row = id2 - 1;
		col = id1;
	}
}

} // namespace VCGL
