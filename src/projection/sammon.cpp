/*!	@file sammon.cpp
 *	@author anantonov
 *	@date	Apr 9, 2012 (created)
 *	@brief	Sammon mapping for LSP extracted from LSP implementation
 */

#include <QVector>
#include "tspoint.h"
#include "ilspdata.h"
#include <cmath>
#include <cassert>
#include <vector>
#include "distancematrix.h"
#include "typedefs.h"

#include "sammon.h"

namespace LSP {

void
Sammon::performSammon(const QVector<ILSPData*>& inPoints, QVector<TSPoint>& outPointsProjection)
{
	outPointsProjection.clear();
	const int inPointsCount = inPoints.size();

	/* initialize the algorithm */
	for (int i = 0; i < inPointsCount; i++) {
		double x = qrand() / (double) RAND_MAX;
		double y = qrand() / (double) RAND_MAX;
		TSPoint tmp(x, y);
		outPointsProjection.push_back(tmp);
	}

	TSPoint delta(0.0, 0.0);
	int maxIterations	= 100;
	double lambda		= 1;
	double d_ij			= 0.0;
	double D_ij			= 0.0;

	/* initialize the random seed */
	qsrand(0);

	for ( int iteration = 0; iteration <= maxIterations; ++iteration ) {
		//  cout << "Iteration: " << iteration << endl;
		/* calculate the lambda value for each iteration */
		double ratio = (double)iteration / (double)maxIterations;
		lambda = pow(0.01, ratio);
		//cout << "Lambda: " << lambda << endl;
		/* create a random vectors to make sure that the values are randomly tackled */
		QVector<int> indexesI;
		randomPermutationVector(inPointsCount, indexesI);
		QVector<int> indexesJ;
		randomPermutationVector(inPointsCount, indexesJ);

		/* perform the minimization */
		for (int i_itt = 0; i_itt < inPointsCount; i_itt++) {
			int i = indexesI[i_itt];
			assert(i<inPointsCount);

			for (int j_itt = 0; j_itt < inPointsCount; j_itt++) {
				int j = indexesJ[j_itt];
				assert(j<inPointsCount);

				if ( i != j ) {
					/* calculate the distance in original space */
					d_ij = distance(inPoints.at(i),inPoints.at(j));

					/* calculate the distance of the projection points */
					D_ij = outPointsProjection[i].distance(outPointsProjection[j]);

					/* avoid devision with 0 */
					if (D_ij == 0) {
							D_ij = 1e-10;
					}

					/* calculate the delta correction */
					delta = (outPointsProjection[i] - outPointsProjection[j]) * (lambda * (d_ij - D_ij) / D_ij);

					// cout << (lambda * (d_ij - D_ij) / D_ij) << " ";

					//					cout << "i: " << CMDP[i].getX() << " " << CMDP[j].getY() << endl;
					//					cout << "j: " << CMDP[j].getX() << " " << CMDP[j].getY() << endl;
					//					cout << "---" << delta.getX() << " " << delta.getY() << endl;

					/* perform the correction */
					outPointsProjection[i] = outPointsProjection[i] + delta;
					outPointsProjection[j] = outPointsProjection[j] - delta;

					//					cout << "i: " << CMDP[i].getX() << " " << CMDP[j].getY() << endl;
					//					cout << "j: " << CMDP[j].getX() << " " << CMDP[j].getY() << endl;
					//					cout << endl;

				}
			}
		}

	}
}

void
Sammon::performSammonDMAT(
		const std::vector<VCGL::strType>& ids,
		const VCGL::DistanceMatrix& dmat,
		QVector<TSPoint>& outPointsProjection)
{
	outPointsProjection.clear();
	std::vector<unsigned> indices;
	dmat.findObjectIndices(ids, indices);

	const int inPointsCount =indices.size();

	/* initialize the algorithm */
	for (int i = 0; i < inPointsCount; i++) {
		double x = qrand() / (double) RAND_MAX;
		double y = qrand() / (double) RAND_MAX;
		TSPoint tmp(x, y);
		outPointsProjection.push_back(tmp);
	}

	TSPoint delta(0.0, 0.0);
	int maxIterations	= 100;
	double lambda		= 1;
	double d_ij			= 0.0;
	double D_ij			= 0.0;

	/* initialize the random seed */
	qsrand(0);

	for ( int iteration = 0; iteration <= maxIterations; ++iteration ) {
		//  cout << "Iteration: " << iteration << endl;
		/* calculate the lambda value for each iteration */
		double ratio = (double)iteration / (double)maxIterations;
		lambda = pow(0.01, ratio);
		//cout << "Lambda: " << lambda << endl;
		/* create a random vectors to make sure that the values are randomly tackled */
		QVector<int> indexesI;
		randomPermutationVector(inPointsCount, indexesI);
		QVector<int> indexesJ;
		randomPermutationVector(inPointsCount, indexesJ);

		/* perform the minimization */
		for (int i_itt = 0; i_itt < inPointsCount; i_itt++) {
			int i = indexesI[i_itt];
			assert(i<inPointsCount);

			for (int j_itt = 0; j_itt < inPointsCount; j_itt++) {
				int j = indexesJ[j_itt];
				assert(j<inPointsCount);

				if ( i != j ) {
					/* calculate the distance in original space */
					d_ij = dmat.getDistanceByIndices(indices[i], indices[j]);

					/* calculate the distance of the projection points */
					D_ij = outPointsProjection[i].distance(outPointsProjection[j]);

					/* avoid devision with 0 */
					if (D_ij == 0) {
							D_ij = 1e-10;
					}

					/* calculate the delta correction */
					delta = (outPointsProjection[i] - outPointsProjection[j]) * (lambda * (d_ij - D_ij) / D_ij);

					/* perform the correction */
					outPointsProjection[i] = outPointsProjection[i] + delta;
					outPointsProjection[j] = outPointsProjection[j] - delta;

				}
			}
		}

	}
}


void Sammon::randomPermutationVector(int size, QVector<int>& outVector)
{
        QVector<int> tmp(size);
        //April 13, 2012: added initialization loop
        for (int i=0; i< size; i++) {
        	tmp[i] = i;
        }
        for (int i = 0; i < size; i++) {
                int j = floor(qrand() / (double)RAND_MAX * (size-1));
                //April 13, 2012: changed to pure swapping of elements
                if (i!=j) {
                	int index = tmp[i];
                	assert(index < size);
                	assert(i<size);
                	assert(j<size);
                	assert(tmp[j]<size);
                	tmp[i] = tmp[j];
                	tmp[j] = index;
                }
        }
        outVector = tmp;
}

} // namespace LSP
