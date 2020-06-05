/*!	@file precomputeddata.cpp
 *	@author anantonov
 *	@date	Mar 27, 2017 (created)
 *	@brief	TODO <file description>
 */

#include "precomputeddata.h"

#include <cassert>
#include <fstream>
#include <sstream>
#include <vector>

#include "projection/projectedpointinfo.h"

void storeCorrelationsTriangle(const std::vector< std::vector<float> >& correlationMatrix, const std::string& corrFileNameOUT, bool binary) {
	const size_t npoints = correlationMatrix.size();
	std::ios_base::openmode mode = std::ofstream::trunc;
	std::ofstream fout;
	if (!binary) {
		fout.open(corrFileNameOUT, mode);
		fout << npoints << std::endl;

		for (size_t x = 0; x<npoints; x++) {
			for (size_t y = 0; y<x; y++) {
				fout << correlationMatrix[x][y] << ' ';
			}
			fout << std::endl;
		}
	}
	else {
		fout.open(corrFileNameOUT, mode | std::ofstream::binary);
		fout.write(reinterpret_cast<const char*>(&npoints), sizeof(size_t));

		for (size_t x = 0; x<npoints; x++) {
			fout.write(reinterpret_cast<const char*>(correlationMatrix[x].data()), x*sizeof(float));
		}
	}
	fout.close();
}

void readCorrelationTriangle(const std::string& fileName, std::vector< std::vector<float> > & correlationMatrix, bool binary) {
		size_t npoints = 0;
		std::ifstream fin;
	if (!binary) {
		fin.open(fileName);
		fin >> npoints;

		correlationMatrix.clear();
		correlationMatrix.resize(npoints, std::vector<float>(npoints) );

		for (size_t x = 0; x<npoints; x++) {
			for (size_t y = 0; y<x; y++) {
				fin >> correlationMatrix[x][y];
				correlationMatrix[y][x] = correlationMatrix[x][y];
			}
			correlationMatrix[x][x] = 1.0;
		}
	}
	else {
		fin.open(fileName, std::ofstream::binary);
		fin.read(reinterpret_cast<char*>(&npoints), sizeof(size_t));

		correlationMatrix.clear();
		correlationMatrix.resize(npoints, std::vector<float>(npoints) );

		for (size_t x = 0; x<npoints; x++) {
			fin.read(reinterpret_cast<char*>(correlationMatrix[x].data()), x*sizeof(float));
			for (size_t y = 0; y<x; y++) {
				correlationMatrix[y][x] = correlationMatrix[x][y];
			}
			correlationMatrix[x][x] = 1.0;
		}
	}
	fin.close();
}

void storeAutocorrelations(const std::vector<float>& autocorrelations, const std::string& autocorrFileNameOUT, bool binary) {
	const size_t npoints = autocorrelations.size();
	std::ios_base::openmode mode = std::ofstream::trunc;
	std::ofstream fout;
	if (!binary) { 
		fout.open(autocorrFileNameOUT, mode);
		fout << npoints << std::endl;
		for (size_t x = 0; x<npoints; x++) {
			fout << autocorrelations[x] << ' ';
		}
	}
	else {
		fout.open(autocorrFileNameOUT, mode | std::ofstream::binary);
		fout.write(reinterpret_cast<const char*>(&npoints), sizeof(size_t));
		fout.write(reinterpret_cast<const char*>(autocorrelations.data()), sizeof(float)*npoints);
	}
	fout.close();
}

void readAutocorrelations(const std::string& fileName, std::vector<float>& autocorrelations, bool binary) {
	size_t npoints = 0;
	std::ifstream fin;
	if (!binary) {
		fin.open(fileName);
		fin >> npoints;

		autocorrelations.clear();
		autocorrelations.resize(npoints);

		for (size_t x = 0; x<npoints; x++) {
			fin >> autocorrelations[x];
		}
	}
	else 
	{
		fin.open(fileName, std::ofstream::binary);
		fin.read(reinterpret_cast<char*>(&npoints), sizeof(size_t));

		autocorrelations.clear();
		autocorrelations.resize(npoints);

		fin.read(reinterpret_cast<char*>(autocorrelations.data()), sizeof(float)*npoints);
	}
	fin.close();
}

void storeProjectionResults(const std::string& fileName, std::vector<VCGL::ProjectedPointInfo>& results, bool binary) {
	const size_t numPoints = results.size();
	std::ios_base::openmode mode = std::ofstream::trunc;
	std::ofstream fout;

	if (!binary) {
		fout.open(fileName, mode);
		fout << numPoints << std::endl;
		for (size_t j=0; j<numPoints; j++) {
			fout << results[j].pt.getX() << ' ' << results[j].pt.getY() << std::endl;
		}
	}
	else {
		fout.open(fileName, mode | std::ofstream::binary);
		fout.write(reinterpret_cast<const char*>(&numPoints), sizeof(size_t));
		std::vector<double> data;
		data.reserve(2*numPoints);
		for (size_t j=0; j<numPoints; j++) {
			data.push_back(results[j].pt.getX());
			data.push_back(results[j].pt.getY());
		}
		fout.write(reinterpret_cast<const char*>(data.data()), sizeof(double)*data.size());
	}
	fout.close();
}

void loadProjectionLonLat(const std::string& fnProjection, int nlon, int nlat, std::vector<VCGL::ProjectedPointInfo>& projection, bool binary) {
	projection.clear();
	size_t numPoints = 0;
	std::ifstream fin;
	if (!binary) {
		fin.open(fnProjection);
		fin >> numPoints;

		assert(numPoints == static_cast<size_t>(nlon*nlat));

		projection.resize(numPoints);

		for (size_t j=0; j<numPoints; j++) {
			double x;
			double y;
			fin >> x >> y;
			projection[j].pt = LSP::TSPoint(x,y);
		}
	}
	else {
		fin.open(fnProjection, std::ofstream::binary);
		fin.read(reinterpret_cast<char*>(&numPoints), sizeof(size_t));

		assert(numPoints == static_cast<size_t>(nlon*nlat));

		projection.resize(numPoints);
		std::vector<double> data(2*numPoints);

		fin.read(reinterpret_cast<char*>(data.data()), sizeof(double)*data.size());

		for (size_t j=0; j<numPoints; j++) {
			projection[j].pt = LSP::TSPoint(data[2*j], data[2*j+1]);
		}
	}

	for (size_t j=0; j<projection.size(); j++) {
		int lon = static_cast<int>(j % nlon);
		int lat = static_cast<int>(j / nlon);
		std::ostringstream str;
		str << '(' << lon << ',' << lat << ')';
		projection[j].name = str.str();
	}
	fin.close();
}
