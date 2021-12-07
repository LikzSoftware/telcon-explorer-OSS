/*! @file Typedefs.h
 * 	@author Anatoliy Antonov
 *	@date Dec 13, 2011 (created)
 *  @brief Definitions of commonly used enumerations, stuctures and typedefs
 */

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#include <string> // STL string implementation for strType typedef
#include <vector> // STL vector implementation for std_vector_float_4 typedef

namespace VCGL {

	typedef std::vector< std::vector<float> >	vectorFloat2D;
	typedef std::vector< std::vector< std::vector<float> > > vectorFloat3D;

	/// @brief link between two points
	struct MNCLink {
		int pointIDFrom; 			///< id of the origin
		int pointIDTo;				///< id of the destination
		float correlationValue;		///< value assigned to the link
	};

	/// @name short names for unsigned types
	//@{
		typedef unsigned char	uchar;
		typedef unsigned short	ushort;
		typedef unsigned int 	uint;
		typedef unsigned long 	ulong;
	//@}

	/*! @name string type
	 *
	 * set of typedefs and functions to implement library routines in string-implementation-independent way.
	 * In case it is necessary to use other string processing class, only this lines need to be changed
	 * (supposing that a new string class has a char*-constructor, assignment operator, copy constructor and operator==)
	 */
	//@{
		/*! @brief typedef for used string implementation */
		typedef std::string strType;

		/*! @brief getting C-string from string implementation */
		inline const char* stringToCharArray(const strType& str) {
			return str.c_str();
		}

		/*! @brief extracting name of the file from string implementation */
		inline strType stringExtractFilename( const strType& path ) {
			return path.substr( path.find_last_of( "/\\" ) +1 ); // looks for / (Unix) OR for \ (Windows)
		}

		inline strType stringExtractFilenameNoExt( const strType& path ) {
			size_t fileNameStart = path.find_last_of( "/\\" ) +1;
			size_t fileNameEnd = path.find_first_of('.', fileNameStart);

			return path.substr(fileNameStart, fileNameEnd-fileNameStart);
		}
	//@}

	/*! @brief plain array representation of dataset, with indices (x,y,z,field) */
	typedef std::vector <std::vector< std::vector <std::vector<float> > > > std_vector_float_4;

	/// Enum for definition of the data type for storing data (Apr 4: subject to be removed)
	enum StorageType {
//		INT8,		// uchar
//		INT16,		// ushort
		FLOAT32		///< float
	};

	/// Used for getting a scalar field projection of a vector field
	enum Axis {
		AXIS_X
		, AXIS_Y
		, AXIS_Z
	};

	/// Describes a regular grid
	struct RegularGridInfo {
		unsigned pointCountX;  ///< number of points along X axis
		unsigned pointCountY; ///< number of points along Y axis
		unsigned pointCountZ; ///< number of points along Z axis
		float spacingX; ///< aspect ratio in X dimension
		float spacingY; ///< aspect ratio in Y dimension
		float spacingZ; ///< aspect ratio in Z dimension

		unsigned totalPointsCount() const { return pointCountX * pointCountY * pointCountZ; }
	};

} // namespace VCGL



#endif // TYPEDEFS_H_
