/*!	@file distancematrix.h
 *	@author anantonov
 *	@date	Oct 22, 2012 (created)
 *	@brief	Definition of distance matrix class
 */

#ifndef DISTANCEMATRIX_H_
#define DISTANCEMATRIX_H_

#include "typedefs.h"
#include <iostream>
#include <vector>

namespace VCGL {

/*! @brief Distance matrix (to use in projections)
 */
class DistanceMatrix {
public:
	/*! @brief Constructor
	 *
	 * @param matrixID String identifier of matrix (used distance measure)
	 * @param objectIDs String identifiers of objects between which distances were computed
	 */
	DistanceMatrix(const strType& matrixID, const std::vector<strType>& objectIDs);

	void findObjectIndices(std::vector<strType> objIDArray, std::vector<unsigned>& outObjIndices) const;

	float getDistanceByIndices(unsigned objIndex, unsigned otherObjIndex ) const;

	/*! @brief Get stored distance between two given objects
	 *
	 * @param objID Identifier of one object
	 * @param otherObjID Identifier of other object
	 * @return Stored distance for two given objects
	 */
	float getDistance(const strType& objID, const strType& otherObjID ) const;

	/*! @brief Store distance between two objects
	 *
	 * @param objID Identifier of one object
	 * @param otherObjID Identifier of other object
	 * @param distance Distance to be stored
	 */
	void setDistance(const strType& objID, const strType& otherObjID, float distance );

	/// String identifier of matrix (used distance measure)
	strType id() const;

	/*! @brief Print matrix in DMAT format of Projection Explorer/VisPipeline
	 *
	 * @param output Output stream (e.g. file or console stream) to write to
	 */
	void printDMAT(std::ostream& output) const;

	/*! @brief Read distance matrix in DMAT format of Projection Explorer/VisPipeline
	 *
	 * @param input Input stream to read from
	 * @param matrixID String identifier for the matrix
	 * @param ppOutMatrix Pointer to pointer which receives the read matrix
	 */
	static void readDMAT(std::istream& input, strType matrixID, DistanceMatrix** ppOutMatrix);

	/*! @brief Create distance matrix in DMAT format from correlation matrix in two-dimensional array
	 *
	 * @param correlations Correlation matrix in two-dimensional array (sizes: nx*ny, nx*ny)
	 * @param nx Point count in x
	 * @param ny Point count in y
	 * @param matrixID String identifier for the matrix
	 * @param ppOutMatrix Pointer to pointer which receives the distance matrix
	 */
	static void fromCorrelationMatrixArray(const std::vector< std::vector<float> >& correlations,
			int nx,
			int ny,
			strType matrixID,
			DistanceMatrix** ppOutMatrix);

	/*! @brief Get object identifiers for which distances are stored in this matrix
	 *
	 * @param outObjIDs Vector receiving object identifiers
	 */
	void getObjectIDs(std::vector<strType>& outObjIDs) const;

	/*! @brief Get object class
	 *
	 * @param objID Object identifier
	 * @return Class value for given object identifier
	 */
	float getObjectClass(const strType& objID) const;

	/*! @brief Set object class
	 *
	 * @param objID Object identifier
	 * @param newClass New class value to be set for given object identifier
	 */
	void setObjectClass(const strType& objID, float newClass);

private:
	/*! @brief Find indices for given pair of object identifiers
	 *
	 * @param objID Identifier of one object
	 * @param otherObjID Identifier of other object
	 * @param row Reference to unsigned where row index will be stored
	 * @param col Reference to unsigned where column index will be stored
	 */
	void findIndices(const strType& objID, const strType& otherObjID, unsigned& row, unsigned& col) const;

	strType dmatID; ///< String identifier of the matrix
	std::vector<strType> objIDs; ///< String identifiers of respective objects
	std::vector<float> objClasses; ///< Float identifiers of object classes
	std::vector< std::vector<float> > distances; ///< Distances between objects
};

} // namespace VCGL
#endif // DISTANCEMATRIX_H_
