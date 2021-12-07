/*! @file selectionhull.cpp
 * @author anantonov
 * @date Created on Jun 25, 2014
 *
 * @brief TODO: Add brief description of the file contents
 */

#include "selectionhull.h"
#include <cmath>

namespace {
	inline int min(int a, int b) { return (a<b)? a : b; }
	inline int max(int a, int b) { return (a>b)? a : b; }
	const double eps = 0.000001;
}

namespace VCGL {

void SelectionHull::init(const QPoint& pt) {
	hull.clear();
	hull.push_back(pt);

	minHull = pt;
	maxHull = pt;
}

void SelectionHull::addPoint(const QPoint& pt) {
	hull.push_back(pt);
	minHull.setX(min(pt.x(),minHull.x()));
	maxHull.setX(max(maxHull.x(),pt.x()));
	minHull.setY(min(pt.y(),minHull.y()));
	maxHull.setY(max(pt.y(),maxHull.y()));
}

bool SelectionHull::point_belong_rect(QPoint &q, QPoint &a, QPoint &b)
{
    bool ok;
    if (a.x()<b.x()) {
    	ok = (a.x() <= q.x() && q.x() <= b.x() );
    } else {
    	ok = (b.x() <= q.x() && q.x() <= a.x() );
    }

    if (a.y()<b.y()) {
    	ok *=(a.y() <= q.y() && q.y() <= b.y() );
    } else {
    	ok *=(b.y() <= q.y() && q.y() <= a.y() );
    }
    return(ok);
}


bool SelectionHull::point_belong_hull(QPoint &q)
{
	if(!point_belong_rect(q,minHull,maxHull)) {
		return false;
	}


	std::vector<QPoint> &hl = hull;
    //cout << "Check point " << q.x() << ", " << q.y() << endl;
    if (hl.empty()) {
    	return(false);
    }

    float x1 = q.x();
    float y1 = q.y();
    float x2 = x1 + 1.0f;
    float y2 = y1;

    int result = 0;

    for(unsigned int j=0; j < hl.size(); j++)
    {
        float x3 = hl[j].x();
        float y3 = hl[j].y();
        float x4 = hl[(j+1)%hl.size()].x();
        float y4 = hl[(j+1)%hl.size()].y();

        float denom = (y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1);
        if(!(fabs(denom)<eps)) {
            float nom1 = (x4-x3)*(y1-y3) - (y4-y3)*(x1-x3);
            float nom2 = (x2-x1)*(y1-y3) - (y2-y1)*(x1-x3);
            float ua = nom1/denom;
            float ub = nom2/denom;

            if(ua >= 0) {
                //if(0.0f <= ub && ub <= 1.0f) cout << "Segment " << x3 << ", " << y3 << " -- " << x4 << ", " << y4 << "(-- " <<
                //        hl[(j+2)%hl.size()].x() << ", " << hl[(j+2)%hl.size()].y() << endl;

                if(y1==y4) {
                	unsigned long k = 2;

                    while ( k<=hl.size() ) {
                    	if ((y3-y1) * (hl[(j+k)%hl.size()].y()-y1) < 0) {
                    		//cout << "k=" << k << ", y=" << hl[(j+k)%hl.size()].y() << endl;
                            result++;
                            break;
                    	}
                    	else if ((y3-y1) * (hl[(j+k)%hl.size()].y()-y1) > 0) {
                    		//cout << "k=" << k << ", y=" << hl[(j+k)%hl.size()].y() << ", no res" << endl;
                            break;
                    	};
                        k++;
                    };
                }
                else if(0.0f < ub && ub < 1.0f) {
                	result++;
                }
            };
        };
    };

    //cout << "final result: " << result << endl << endl;
    if (result%2 != 0) {
    	return(true);
    } else {
    	return(false);
    }
}

} /* namespace VCGL */
