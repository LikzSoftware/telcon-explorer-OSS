/*! @file ncfiledatastorage.cpp
 * @author anantonov
 * @date Created on Nov 26, 2014
 *
 * @brief Implementation of the class for reading data stored in NetCDF files
 */

#include "ncfiledatastorage.h"

#include <iostream>
#include "typedefs.h"
#include <netcdf.h>
#include <string.h>
#include <math.h>
#include <cassert>

namespace {
	int nc_retval = 0;
	const char* LON_NAME = "lon";
	const char* LAT_NAME = "lat";
	const char* LEV_NAME = "lev";
	const char* LEV_NAME_2 = "level";
	const char* TIME_NAME = "time";
}

namespace VCGL {

struct NCFileHelper {
	/*! load 1D array variable
	 *
	 * @param ncid	Open NetCDF file descriptor
	 * @param varid	ID of the variable to be read
	 * @param size	The length of the array variable
	 * @return
	 */
	static std::vector<float> loadArray(int ncid, int varid, size_t size) {
		nc_retval = 0;

		std::vector<float> data;
		if (size > 0 && varid != -1) {
			data.resize(size, 0.0);
			nc_retval = nc_get_var_float(ncid, varid, &data[0]);
			if (nc_retval) {
				data.clear();
			}
		}
		return data;
	}

	/*! get attributes used for packed data representation
	 *
	 * @param ncid	Open NetCDF file descriptor
	 * @param varID	ID of the variable for which to get attributes
	 * @param[out] pScaleFactor Pointer receiving value of the scaling factor
	 * @param[out] pAddOffset Pointer receiving value of the offset
	 */
	static void getPackedDataAttributes(int ncid, int varID, float* pScaleFactor, float* pAddOffset) {
		//these variables are used for the packed data (NCEP example dataset)
		//in case of any errors we assume no-packed values
		if (NC_NOERR != nc_get_att_float(ncid, varID, "scale_factor", pScaleFactor)) {
			*pScaleFactor = 1.0f;
		}
		if (NC_NOERR != nc_get_att_float(ncid, varID, "add_offset", pAddOffset)) {
			*pAddOffset = 0.0f;
		}
	}

	/*! get length of a dimension
	 *
	 * @param ncid	Open NetCDF file descriptor
	 * @param dimname Textual name of the dimension
	 * @param[out] pdimlen Pointer receiving the length of the dimension
	 * @return
	 */
	static bool getDimLen(int ncid, const char* dimname, size_t* pdimlen) {
		nc_retval = 0;

		bool OK = true;

		int dimid = -1;
		if ((nc_retval = nc_inq_dimid(ncid, dimname, &dimid))) { OK = false; }
		if (OK && (nc_retval = nc_inq_dimlen(ncid, dimid, pdimlen))) { OK = false; }

		return OK;
	}

	/*! check if the dimension name equals the given name
	 *
	 * @param ncid	Open NetCDF file descriptor
	 * @param dimid Dimension ID
	 * @param name	Expected name of the dimension
	 * @return
	 */
	static bool check_dimname(int ncid, int dimid, const char* name) {
		nc_retval = 0;

		char dimname[NC_MAX_NAME+1];
		if ((nc_retval = nc_inq_dimname(ncid, dimid, dimname))) {
			return false;
		}
		return (0 == strcmp(dimname, name));
	}

	/*! check if the dimension name equals one of the level dimension names
	 *
	 * @param ncid	Open NetCDF file descriptor
	 * @param dimid Dimension ID
	 * @return
	 */
	static bool check_levdimname(int ncid, int dimid) {
		if (check_dimname(ncid, dimid, LEV_NAME)) {
			return true;
		}
		else return check_dimname(ncid, dimid, LEV_NAME_2);
	}

	/*! Find the index of the given level value
	 *
	 * @param ncid			Open NetCDS file descriptor
	 * @param lev_dimid		ID of the level dimesnion
	 * @param level			Desired level value
	 * @param[out] piLev	Pointer receiving the index of
	 * @return
	 */
	static bool assign_ilev(int ncid, int lev_dimid, int level, size_t *piLev) {
		nc_retval = 0;
		bool OK = true;

		*piLev = 0;

		size_t nlev = 0;
		if ((nc_retval = nc_inq_dimlen(ncid, lev_dimid, &nlev))) { OK = false; }

//		//nLev > 1?
//		if (OK && (nlev == 1)) {
//			*piLev = 0;
//		}

		if (OK && (nlev > 1)) {
			//yes -> find iLev for the given level in the lev variable values
			int lev_varid = -1;
			if (OK && (nc_retval = nc_inq_varid(ncid, LEV_NAME, &lev_varid))) { OK = false; }

			float levs[nlev];
			if (OK && (nc_retval = nc_get_var_float(ncid, lev_varid, &levs[0]))) { OK = false; }

			if (OK) {
				size_t i=0;
				while(i<nlev && fabs(levs[i]-level)<0.1) { i++; }
				if (i==nlev) { OK = false; }
				else { *piLev = i; }
			}
		}
		return OK;
	}

	/*! assure that the order of dimensions in the file matches expectations
	 *
	 * @param ncid			Open NetCDF file descriptor
	 * @param dimids		Array of the dimension IDs for the desired variable
	 * @param var_numdim	Number of valid dimension IDs in the array
	 * @return
	 */
	static bool checkDimensionOrder(int ncid, int dimids[], int var_numdim) {
		bool OK = true;

		if (OK && var_numdim == 4) {
				//check that the order is time, lev, lat, lon
				if (OK && !check_dimname(ncid, dimids[0], TIME_NAME)) { OK = false; }
				if (OK && !check_levdimname(ncid, dimids[1])) { OK = false; }
				if (OK && !check_dimname(ncid, dimids[2], LAT_NAME)) { OK = false; }
				if (OK && !check_dimname(ncid, dimids[3], LON_NAME)) { OK = false; }
		}
		if (OK && var_numdim == 3) {
				//check that the order is time, lat, lon
				if (OK && !check_dimname(ncid, dimids[0], TIME_NAME)) { OK = false; }
				if (OK && !check_dimname(ncid, dimids[1], LAT_NAME)) { OK = false; }
				if (OK && !check_dimname(ncid, dimids[2], LON_NAME)) { OK = false; }
		}
		return OK;
	}

	/*! Rearrange the data from (time, (level), lat, lon) to (lat, lon, time)
	 *
	 * @param var_in	The data array with the (time, (level), lat, lon) indices
	 * @param ntime		Length of the time dimension for var_in array
	 * @param nlat		Length of the latitude dimension for var_in array
	 * @param nlon		Length of the longitude dimension for var_in array
	 * @param scale_factor	Scaling factor (in case of packed data, otherwise 1.0)
	 * @param add_offset	Offset (in case of packed data, otherwise 0.0)
	 * @param[out] data			Output 3D vector with the (lat, lon, time) indices
	 */
	static void rearrangeData(const std::vector<float>& var_in,
				size_t ntime,
				size_t nlat,
				size_t nlon,
				float scale_factor,
				float add_offset,
				vectorFloat3D& data) {

			data.clear();
			const std::vector<float> ts_init(ntime, 0);
			const std::vector< std::vector<float> > par_init(nlon, ts_init);
			data.resize(nlat, par_init);


			//diagnostic variables - for user eyes only
			float dataMin;
		   float dataMax;
		   dataMin = data[0][0][0];
		   dataMax = data[0][0][0];

		   for (size_t itime = 0; itime < ntime; itime++) {
			   for (size_t ilat = 0; ilat < nlat; ilat++) {
				   for (size_t ilon = 0; ilon < nlon; ilon++) {
					   size_t index = itime*nlat*nlon + ilat*nlon + ilon;

					   float dataValue = var_in[index] * scale_factor + add_offset;
					   data[ilat][ilon][itime] = dataValue;

					   if (dataMin > dataValue) {
						   dataMin = dataValue;
					   }
					   if (dataMax < dataValue) {
						   dataMax = dataValue;
					   }
				   }
			   }
		   }

		   std::cerr << " dataMin = " << dataMin
				    << ", dataMax = " << dataMax << std::endl;

		}
};

NCFileDataStorage::NCFileDataStorage(const char* filename)
: ncid(-1),
  nlat(0),
  nlon(0),
  ntime(0),
  varID(-1),
  var_numdim(0),
  iLev(0),
  lat_varid(-1),
  lon_varid(-1)
{

	bool OK = true;

	//open file
	if (OK && (nc_retval = nc_open(filename, NC_NOWRITE, &ncid))) { OK = false; }

	//get ntime, nlat, nlon

	if (OK && !NCFileHelper::getDimLen(ncid, LAT_NAME, &nlat)) { OK = false; }
	if (OK && !NCFileHelper::getDimLen(ncid, LON_NAME, &nlon)) { OK = false; }
	if (OK && !NCFileHelper::getDimLen(ncid, TIME_NAME, &ntime)) { OK = false; }

	if (OK && (nc_retval = nc_inq_varid(ncid, LAT_NAME, &lat_varid))) { OK = false; }
	if (OK && (nc_retval = nc_inq_varid(ncid, LON_NAME, &lon_varid))) { OK = false; }


	assert(OK);
	//TODO: exception if not OK
}

NCFileDataStorage::~NCFileDataStorage() {
	if (ncid != -1) {
		nc_close(ncid);
	}
}

int NCFileDataStorage::initVariable(
			const char* varname,
			int level) {

	nc_retval = 0;
	bool OK = true;

	// get & check numdim for variable
	if (OK && (nc_retval = nc_inq_varid(ncid, varname, &varID))) { OK = false; }


	int dimids[NC_MAX_VAR_DIMS];
	if (OK && (nc_retval = nc_inq_var(ncid, varID, 0, 0, &var_numdim, dimids, 0))) { OK = false; }

	if (OK) {
		OK = NCFileHelper::checkDimensionOrder(ncid, dimids, var_numdim);
	}

	if (OK && var_numdim == 4) {
		//iLev = 0 if there is only one level, otherwise index of appropriate level
		if (!NCFileHelper::assign_ilev(ncid, dimids[1], level, &iLev)) { OK = false; }
	}

	return OK;

}

void NCFileDataStorage::releaseVariable() {
	varID = -1;
	var_numdim = 0;
	iLev = 0;
}

std::size_t NCFileDataStorage::getNTime() {
	return ntime;
}

std::size_t NCFileDataStorage::getNLon() {
	return nlon;
}

std::size_t NCFileDataStorage::getNLat() {
	return nlat;
}

std::vector<float> NCFileDataStorage::loadLons() {
	return NCFileHelper::loadArray(ncid, lon_varid, nlon);
}
std::vector<float> NCFileDataStorage::loadLats() {
	return NCFileHelper::loadArray(ncid, lat_varid, nlat);

}
vectorFloat3D NCFileDataStorage::loadData(std::size_t latStart, std::size_t latCount) {
	nc_retval = 0;
	bool OK = true;
	std::vector<float> var_in(ntime*nlat*nlon);

	//numdim = 4
	if (OK && var_numdim == 4) {
		OK = read4DVar(var_in, latStart, latCount);
	}
	//numdim = 3
	if (OK && var_numdim == 3) {
		OK = read3DVar(var_in, latStart, latCount);
	}

	//at this point we have 3D data = an array of 2D maps for each timestep

	vectorFloat3D data;

	if (OK) {
		float scaleFactor = 1.0f;
		float addOffset = 0.0f;
		NCFileHelper::getPackedDataAttributes(ncid, varID, &scaleFactor, &addOffset);

		//rearrange data from (time, lat, lon) to (lat, lon, time)
		NCFileHelper::rearrangeData(var_in, ntime, latCount, nlon, scaleFactor, addOffset, data);
	}
	return data;
}

bool NCFileDataStorage::read4DVar(std::vector<float>& var_in, std::size_t latStart, std::size_t latCount) {
	//read variable: (0,iLev,0,0) -> (ntime, iLev+1, nLat, nLon)
	size_t start[4], count[4];

	start[0] = 0;
	start[1] = iLev;
	start[2] = latStart;
	start[3] = 0;

	count[0] = ntime;
	count[1] = 1;
	count[2] = latCount;
	count[3] = nlon;

	std::cout << "reading 4D var, start=("
		   << start[0] << ',' << start[1] << ',' << start[2] << ',' << start[3] <<
		   "), count=("
		   << count[0] << ',' << count[1] << ',' << count[2] << ',' << count[3] << ")" << std::endl;
	bool OK = true;

	if ((nc_retval = nc_get_vara_float(ncid, varID, start, count, &var_in[0]))) { OK = false; }

	return OK;
}

bool NCFileDataStorage::read3DVar(std::vector<float>& var_in, std::size_t latStart, std::size_t latCount) {
	//read variable: (0,0,0) -> (ntime,nlat,nlon)
	size_t start[3], count[3];

	start[0] = 0;
	start[1] = latStart;
	start[2] = 0;

	count[0] = ntime;
	count[1] = latCount;
	count[2] = nlon;

	std::cerr << "reading 3D var, start=("
			   << start[0] << ',' << start[1] << ',' << start[2] <<  "), count=("
			   << count[0] << ',' << count[1] << ',' << count[2] <<  ")" << std::endl;

	bool OK = true;

	if ((nc_retval = nc_get_vara_float(ncid, varID, start, count, &var_in[0]))) { OK = false; }

	return OK;
}

} /* namespace VCGL */
