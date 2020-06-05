/*!	@file precompute.cpp
 *	@author anantonov
 *	@date	Sep 11, 2013 (created)
 *	@brief	TODO <file description>
 */

#include "precompute.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <cassert>
#include "progressbar.h"

#include "projection/distancematrix.h"
#include "projection/projectedpointinfo.h"
#include "projection/sammon.h"

#include <sstream>


/** @brief Compute correlation matrix of all pairs of time series
 *
 * @param data 3D data array, indices LAT, LON, TIME
 * @param correlationMatrix output - symmetric square matrix with correlations
 */
void computeCorrelations(
		const std::vector< std::vector< std::vector<float> > >& data,
		std::vector< std::vector<float> >& correlationMatrix,
		std::vector< std::vector<bool> >& validityMask) {

	const int nlat = data.size();
	const int nlon = data[0].size();
	const int ntime = data[0][0].size();

   //compute the correlation matrix
   const int npoints = nlat * nlon;

   const int totalNumIterations = npoints*(npoints-1)/2;
   progress_bar(0, totalNumIterations);

   std::vector<float> averages(npoints, 0.0f);

   correlationMatrix.clear();
   correlationMatrix.resize(npoints, std::vector<float>(npoints, 0.0f));

   //compute averages
   for (int pt = 0; pt<npoints; pt++) {
	   int ptLat = pt / nlon;
	   int ptLon = pt % nlon;
	   float sum = 0.0f;
	   if (validityMask[ptLat][ptLon]) {
		   for (int t=0; t<ntime; t++) {
			   sum += data[ptLat][ptLon][t];
		   }
		   averages[pt] = sum / ntime;
	   }
   }

   float minval = 1.0;
   float maxval = 0.0;

   int currIteration = 0;
   for (int x = 0; x<npoints; x++) {
	   correlationMatrix[x][x] = 1.0;
	   for (int y = 0; y<x; y++) {
		   int xLat = x / nlon;
		   int xLon = x % nlon;
		   int yLat = y / nlon;
		   int yLon = y % nlon;

		   float nom = 0.0f;
		   float denomX = 0.0f;
		   float denomY = 0.0f;

		   //compute and assign correlation only if both points are valid
		   //otherwise leave default value (zero correlation)
		   if (validityMask[xLat][xLon] && validityMask[yLat][yLon]) {
			   for (int t=0; t<ntime; t++) {
				   float dx = data[xLat][xLon][t]-averages[x];
				   float dy = data[yLat][yLon][t]-averages[y];
				   nom += dx*dy;
				   denomX += dx*dx;
				   denomY += dy*dy;
			   }
			   float corrValue = nom / sqrt(denomX*denomY);
			   correlationMatrix[x][y] = corrValue;
			   correlationMatrix[y][x] = corrValue;

			   if (minval > correlationMatrix[x][y]) {
				   minval = correlationMatrix[x][y];
			   }
			   if (maxval < correlationMatrix[x][y]) {
				   maxval = correlationMatrix[x][y];
			   }
		   }
		   currIteration++;
		   progress_bar(currIteration, totalNumIterations);
      }
   }
   std::cout << std::endl; // for the progress bar

   std::cout << "min=" << minval << ", max=" << maxval << std::endl;
}

void computeAutocorrelations(const std::vector< std::vector< std::vector<float> > >& data,
				std::vector<float> & autocorrelations,
				std::vector< std::vector<bool> >& validityMask) {

	const int nlat = data.size();
	const int nlon = data[0].size();
	const int ntime = data[0][0].size();

   //compute the correlation matrix
   const int npoints = nlat * nlon;

   const int totalNumIterations = npoints;
   progress_bar(0, totalNumIterations);

   std::vector<float> averages(npoints, 0.0f);

   autocorrelations.clear();
   autocorrelations.resize(npoints, 0.0f);

   //compute averages
   for (int pt = 0; pt<npoints; pt++) {
	   int ptLat = pt / nlon;
	   int ptLon = pt % nlon;
	   float sum = 0.0f;
	   if (validityMask[ptLat][ptLon]) {
		   for (int t=0; t<ntime; t++) {
			   sum += data[ptLat][ptLon][t];
		   }
		   averages[pt] = sum / ntime;
	   }
   }

   float minval = 1.0;
   float maxval = -1.0;

   int currIteration = 0;
   for (int x = 0; x<npoints; x++) {
	   int xLat = x / nlon;
	   int xLon = x % nlon;

	   if (validityMask[xLat][xLon]) {
		   //x starts with timestep 0, y starts with timestep 1, length ntime-1

		   //adjust averages
		   float avX = (averages[x] * ntime - data[xLat][xLon][ntime-1]) / (ntime-1);
		   float avY = (averages[x] * ntime - data[xLat][xLon][0]) / (ntime-1);

		   float nom = 0.0f;
		   float denomX = 0.0f;
		   float denomY = 0.0f;

		   for (int t=0; t<ntime-1; t++) {
			   float dx = data[xLat][xLon][t]-avX;
			   float dy = data[xLat][xLon][t+1]-avY;
			   nom += dx*dy;
			   denomX += dx*dx;
			   denomY += dy*dy;
		   }

		   float corrValue = nom / sqrt(denomX*denomY);
		   autocorrelations[x] = corrValue;

		   if (minval > corrValue) {
			   minval = corrValue;
		   }
		   if (maxval < corrValue) {
			   maxval = corrValue;
		   }
	   }
	   currIteration++;
	   progress_bar(currIteration, totalNumIterations);
  }
   std::cout << std::endl; // for the progress bar
   std::cout << "min=" << minval << ", max=" << maxval << std::endl;
}

void
projectCorrelationMatrix(const std::vector< std::vector<float> >& correlations,
		int nx, int ny, std::vector<VCGL::ProjectedPointInfo>& output) {
	VCGL::DistanceMatrix* pdmat = 0;
	VCGL::DistanceMatrix::fromCorrelationMatrixArray(correlations, nx, ny, "correlation", &pdmat);

	std::vector<VCGL::strType> ptNames;
	pdmat->getObjectIDs(ptNames);

	projectDMAT(ptNames, *pdmat, output);

	delete pdmat;
	pdmat = 0;
}

void
projectDMAT(
		const std::vector<VCGL::strType>& ids,
		const VCGL::DistanceMatrix& dmat,
		std::vector<VCGL::ProjectedPointInfo>& output) {
	output.clear();

	const uint fieldsCount = ids.size();
	output.resize(fieldsCount);

	std::cout << fieldsCount << " points to project..." << std::endl;

	int pointToProjectIndex = 0;
	for (uint i=0; i<fieldsCount; i++) {
		//std::cout << "ids[" << i << "]: " << ids[i].c_str() << std::endl;
		output[pointToProjectIndex].name = ids[i];
		pointToProjectIndex++;
	}


	//project all fields
	QVector<LSP::TSPoint> projectedPoints;
	LSP::Sammon::performSammonDMAT(ids, dmat, projectedPoints);

	const ulong outSize = output.size();
	const ulong projSize = projectedPoints.size();
	assert(outSize == projSize);

	for(uint i=0; i<outSize; i++) {
		output[i].pt = projectedPoints[i];
	}
}
