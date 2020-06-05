/*!	@file sammon.h
 *	@author anantonov
 *	@date	Apr 9, 2012 (created)
 *	@brief	Sammon mapping for LSP extracted from LSP implementation
 */

#ifndef SAMMON_H_
#define SAMMON_H_

#include <QVector>
#include "tspoint.h"

#include <vector>
#include "typedefs.h"


namespace VCGL {
	class DistanceMatrix;
}

namespace LSP {
struct ILSPData;

/*
 *
 */
class Sammon {
public:

    /**
     * @brief 	Perform Sammon's Mapping on given points
     *
     * Sammon [Sam69] supposes a steepest-descent iterative process. This is a computationally
     * heavy task and quite complicated one. Kohonen [Koh01] provides a heuristic
     * that works reasonably well and is quite easy to implement.
     *
     * @param inPoints		Set of given points to perform Sammon's Mapping
     * @param outPointsProjection (output) mapping of given points into 2D QPoints
     */
    static void performSammon(const QVector<ILSPData*>& inPoints, QVector<TSPoint>& outPointsProjection);

    static void performSammonDMAT(const std::vector<VCGL::strType>& ids, const VCGL::DistanceMatrix& dmat, QVector<TSPoint>& outPointsProjection);

    /**
     * @brief	Return a vector of random permutation of number 0 to size-1
     */
    static void randomPermutationVector(int size, QVector<int>& outVector);
private:
    //forbid creating instances of Sammon class
    Sammon();
};

} // namespace LSP

#endif // SAMMON_H_
