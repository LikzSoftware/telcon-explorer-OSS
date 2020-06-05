/*!	@file read.h
 *	@author anantonov
 *	@date	May 27, 2013 (created)
 *	@brief	TODO <file description>
 */

#ifndef READ_H_
#define READ_H_
#include <vector>
#include <QPointF>

int fileStat(const char* dataFileName);

int loadData(const char* dataFileNameIN, std::vector< std::vector< std::vector<float> > >& data, int* pnlon, int* pnlat);

void readCorrelationWithLags(
		const char* corrFileName,
		const char* lagsFileName,
		int* pnlat,
		int* pnlon,
		std::vector< std::vector<float> > & minCorrelations,
		std::vector< std::vector<int> > & lags );

void readContours(const char* contoursFileName, std::vector< std::vector<QPointF> >& contours);


#endif // READ_H_
