/*! @file cppunitextras.h
 * @author anantonov
 * @date Created on May 18, 2014
 *
 * @brief TODO: Add brief description of the file contents
 */

#ifndef CPPUNITEXTRAS_H_
#define CPPUNITEXTRAS_H_

#include <CppUnitLite/SimpleString.h>
#include <QPoint>

#include <vector>
#include <sstream>
#include <string>

SimpleString StringFrom (const QPoint& value);

namespace VCGL {
	struct MRect;
}

SimpleString StringFrom(const VCGL::MRect& value);

/// output of 1D vector
template<typename T>
SimpleString StringFrom(const std::vector<T>& vec) {
	std::ostringstream oss;
	oss << '[';
	for (size_t i=0; i<vec.size(); i++) {
		oss << vec[i];
		if (i<vec.size()-1) {
			oss << ", ";
		}
	}
	oss << ']';
	return StringFrom(oss.str().c_str());
}

/// output of 2D vector
template<typename T>
SimpleString StringFrom(const std::vector< std::vector<T> >& vec) {
	std::ostringstream oss;
	oss << '[' << std::endl;
	for(auto x: vec) {
		oss << '[';
		for (size_t i=0; i<x.size(); i++) {
			oss << x[i];
			if (i<x.size()-1) {
				oss << ", ";
			}
		}
		oss << ']' << std::endl;
	}
	oss << ']';
	return StringFrom(oss.str().c_str());
}

/// output of 3D vector
template<typename T>
SimpleString StringFrom(const std::vector< std::vector<std::vector<T> > >& vec) {
	std::ostringstream oss;
	oss << '[' << std::endl;

	for(auto x: vec) {
		oss << '[';

		for (size_t j=0; j<x.size(); j++) {
			oss << '[';

			for (size_t i=0; i<x[j].size(); i++) {
				oss << x[j][i];
				if (i<x[j].size()-1) {
					oss << ", ";
				}
			}

			oss << ']';
			if (j<x.size()-1) {
				oss << "; ";
			}
		}

		oss << ']' << std::endl;
	}

	oss << ']';
	return StringFrom(oss.str().c_str());
}

#endif /* CPPUNITEXTRAS_H_ */
