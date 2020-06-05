/*! @file regionconnectivity.h
 * @author anantonov
 * @date Created on May 18, 2014
 *
 * @brief TODO: Add brief description of the file contents
 */

#ifndef REGIONCONNECTIVITY_H_
#define REGIONCONNECTIVITY_H_

#include <map>
#include <iostream>
#include <vector>

namespace VCGL {
struct Link;

class RegionConnectivity {
public:
	/*! @brief get a link
	 *
	 * @param[in] regionFrom 	starting region of a link
	 * @param[in] regionTo 		ending region of a link
	 * @param[out] outLink 		reference to the object to which the requested link is stored
	 * @return true if the object contains a requested link, false otherwise
	 */
	bool getLink(int regionFrom, int regionTo, Link& outLink) const;

	void getLinks(std::vector<Link>& outLinks) const;

	/*! @brief suggest a link
	 *
	 * stores the link if either there was no link between the specified regions
	 * or if the stored link's weight was less than that of the suggested one
	 *
	 * @param regionFrom 	starting region of a link
	 * @param regionTo		ending region of a link
	 * @param link 			suggested link
	 */
	void suggestLink(int regionFrom, int regionTo, const Link& link);

	friend std::ostream& operator<< (std::ostream& out, const RegionConnectivity& rc);
protected:
	std::map< std::pair<int,int>, Link> links;
};

} /* namespace VCGL */

#endif /* REGIONCONNECTIVITY_H_ */
