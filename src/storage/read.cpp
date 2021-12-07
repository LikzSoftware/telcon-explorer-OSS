/* Based on a part of the netCDF package.
 * ( Copyright 2006 University Corporation for Atmospheric Research/Unidata.)
   $Id: pres_temp_4D_rd.c,v 1.5 2006/06/26 20:37:31 russ Exp $
*/

#include "read.h"

#include <stdio.h>
#include <string.h>
#include <netcdf.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <cassert>

#include "typedefs.h"

#include <QPointF>


// /* This is the name of the data file we will read. */
//#define FILE_NAME "geopoth500hPa.nc"

/* We are reading 4D data, a 2 x 6 x 12 lvl-lat-lon grid, with 2
   timesteps of data. */
#define NDIMS 4
#define NLON 96
#define NLAT 48
#define NLEV 1
#define NTIME 100

#define LON_NAME "lon"
#define LAT_NAME "lat"
#define LEV_NAME "lev"
#define TIME_NAME "time"

#define VAR_NAME "geopoth"

/* Handle errors by printing an error message and exiting with a
 * non-zero status. */
#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); return 2;}

int fileStat(const char* dataFileName) {
 /* Error handling. */
   int retval;

   int ncid;
   /* Open the file. */
   if ((retval = nc_open(dataFileName, NC_NOWRITE, &ncid)))
	  ERR(retval);

   int numdims = 0;
   if ((retval = nc_inq_ndims(ncid, &numdims)))
		  ERR(retval);

   for (int i=0; i<numdims; i++) {
	   char dimname [NC_MAX_NAME+1];
	   if ((retval = nc_inq_dimname(ncid, i, dimname)))
				  ERR(retval);

	   size_t dimlen = 0;
	   if ((retval = nc_inq_dimlen(ncid, i, &dimlen)))
				  ERR(retval);

	   std::cout << "dim " << i << ": " << dimname << ", " << dimlen << std::endl;
   }
   if ((retval = nc_close(ncid)))
         ERR(retval);
   return 0;
}

int
loadData(const char* dataFileNameIN, std::vector< std::vector< std::vector<float> > >& data, int* pnlon, int* pnlat)
{

   /* Error handling. */
   int retval;

   int ncid;
   /* Open the file. */
   if ((retval = nc_open(dataFileNameIN, NC_NOWRITE, &ncid)))
      ERR(retval);

//   int lat_varid, lon_varid;
//   /* Get the varids of the latitude and longitude coordinate
//    * variables. */
//   if ((retval = nc_inq_varid(ncid, LAT_NAME, &lat_varid)))
//      ERR(retval);
//   if ((retval = nc_inq_varid(ncid, LON_NAME, &lon_varid)))
//      ERR(retval);
//
//   /* These program variables hold the latitudes and longitudes. */
//   float lats[NLAT], lons[NLON];
//
//   /* Read the coordinate variable data. */
//   if ((retval = nc_get_var_float(ncid, lat_varid, &lats[0])))
//      ERR(retval);
//   if ((retval = nc_get_var_float(ncid, lon_varid, &lons[0])))
//      ERR(retval);


   /* Loop indexes. */
//   int lvl, lat, lon, rec, i = 0;

//   /* Check the coordinate variable data. */
//   std::cout << "latitudes: " << std::endl;
//   for (lat = 0; lat < NLAT; lat++) {
//	   std::cout << lats[lat] << ' ';
//   }
//   std::cout << std::endl;
//
//   std::cout << "longitudes: " << std::endl;
//   for (lon = 0; lon < NLON; lon++) {
//	   std::cout << lons[lon] << ' ';
//   }
//   std::cout << std::endl;


   int var_varid;
   /* Get the varids of the pressure and temperature netCDF
    * variables. */
   if ((retval = nc_inq_varid(ncid, VAR_NAME, &var_varid)))
      ERR(retval);

   /* data array */
   float var_in[NTIME][NLEV][NLAT][NLON];

   /* The start and count arrays will tell the netCDF library where to
      read our data. */
   size_t start[NDIMS], count[NDIMS];

   count[0] = NTIME;
   count[1] = NLEV;
   count[2] = NLAT;
   count[3] = NLON;
   start[0] = 0;
   start[1] = 0;
   start[2] = 0;
   start[3] = 0;

   if ((retval = nc_get_vara_float(ncid, var_varid, start, count, &var_in[0][0][0][0])))
	  ERR(retval);

   //take the required level
   assert(NLEV==1);

   //rearrange data to time-series
   const std::vector<float> ts_init(NTIME, 0);
   const std::vector< std::vector<float> > par_init(NLAT, ts_init);

   data.clear();
   data.resize(NLON, par_init);

   for (unsigned itime = 0; itime < NTIME; itime++) {
	   for (unsigned ilat = 0; ilat < NLAT; ilat++) {
		   for (unsigned ilon = 0; ilon < NLON; ilon++) {
			   data[ilon][ilat][itime] = var_in[itime][0][ilat][ilon];
		   }
	   }
   }
   *pnlon = NLON;
   *pnlat = NLAT;

   /* Close the file. */
   if ((retval = nc_close(ncid)))
      ERR(retval);
   return 0;
}



void readCorrelationWithLags(
		const char* corrFileName,
		const char* lagsFileName,
		int* pnlat,
		int* pnlon,
		std::vector< std::vector<float> > & minCorrelations,
		std::vector< std::vector<int> > & lags ) {

	const int NPOINTS = NLAT * NLON;

	*pnlat = NLAT;
	*pnlon = NLON;
	minCorrelations.clear();
	minCorrelations.resize(NPOINTS, std::vector<float>(NPOINTS) );
	lags.clear();
	lags.resize(NPOINTS, std::vector<int>(NPOINTS, -1));

	std::ifstream fin(corrFileName);
	for (int x = 0; x<NPOINTS; x++) {
	   for (int y = 0; y<NPOINTS; y++) {
		   fin >> minCorrelations[x][y];
	  }
	}
	fin.close();

	std::ifstream lfin(lagsFileName);
		for (int x = 0; x<NPOINTS; x++) {
		   for (int y = 0; y<NPOINTS; y++) {
			   lfin >> lags[x][y];
		  }
		}
	lfin.close();

}

void readContours(const char* contoursFileName, std::vector< std::vector<QPointF> >& contours) {
	std::ifstream fin(contoursFileName);

	int numContours = 0;
	contours.clear();

	fin >> numContours;
	contours.resize(numContours);
	for (int i=0; i<numContours; i++) {
		int numPointsInContour = 0;
		fin >> numPointsInContour;
		contours[i].resize(numPointsInContour);

		for (int j=0; j<numPointsInContour; j++) {
			float lon = 0.0;
			float lat = 0.0;
			fin >> lon >> lat;
			contours[i][j] = QPointF(lon, lat);
		}
	}

//	std::ofstream fout("land_contours.test");
//		fout << contours.size() << std::endl;
//		for (unsigned i=0; i<contours.size(); i++) {
//			fout << contours[i].size() << std::endl;
//			for (unsigned j=0; j<contours[i].size(); j++) {
//				fout << contours[i][j].x() << "\t\t" << contours[i][j].y() << std::endl;
//			}
//		}

}
