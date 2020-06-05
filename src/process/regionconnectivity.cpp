/*! @file regionconnectivity.cpp
 * @author anantonov
 * @date Created on May 18, 2014
 *
 * @brief TODO: Add brief description of the file contents
 */

#include "regionconnectivity.h"
#include "link.h"

namespace VCGL {

bool RegionConnectivity::getLink(int regionFrom, int regionTo, Link& outLink) const {
	auto iter = links.find({regionFrom, regionTo});
	bool bFound = (iter != links.end() );
	if (bFound) {
		outLink = (*iter).second;
	}
	return bFound;
}

void RegionConnectivity::getLinks(std::vector<Link>& outLinks) const {
	outLinks.clear();
	outLinks.resize(links.size());
	unsigned i=0;
	for (auto iter = links.begin();
			iter != links.end();
			++iter) {
		outLinks[i++] = (*iter).second;
	}
}

void RegionConnectivity::suggestLink(int regionFrom, int regionTo, const Link& link) {
	Link oldLink;
	bool bOldLink = getLink(regionFrom, regionTo, oldLink);
	if (!bOldLink || oldLink.w < link.w) {
		links[{regionFrom,regionTo}] = link;
	}
}

std::ostream& operator<< (std::ostream& out, const RegionConnectivity& rc) {
	auto iter = rc.links.begin();
	while (iter != rc.links.end()) {
		if (iter != rc.links.begin()) {
			out << std::endl;
		}

		std::pair<int, int> regions = (*iter).first;
		Link ln = (*iter).second;
		out << regions.first << " -> " << regions.second << ": " <<
				'(' << ln.ptA.x() << ',' << ln.ptA.y() << ')' << " to " <<
				'(' << ln.ptB.x() << ',' << ln.ptB.y() << ')' <<
				", teleconnectivity is " << ln.w;
		++iter;
	}
	return out;
}

} /* namespace VCGL */
