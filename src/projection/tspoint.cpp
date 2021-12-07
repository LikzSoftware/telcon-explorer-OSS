
#include <cmath>

#include "tspoint.h"

namespace LSP {

TSPoint::TSPoint ()
{
        this->type 	= TSPoint::D2;
        this->x 	= 0;
        this->y 	= 0;
        this->z 	= 0;
}

TSPoint::TSPoint (double x, double y)
{
        this->type 	= TSPoint::D2;
        this->x 	= x;
        this->y 	= y;
        this->z 	= 0;
}

TSPoint::TSPoint (double x, double y, double z)
{
        this->type 	= TSPoint::D3;
        this->x 	= x;
        this->y 	= y;
        this->z 	= z;
}

double TSPoint::getX () const
{
        return this->x;
}

double TSPoint::getY () const
{
        return this->y;
}

double TSPoint::getZ () const
{
        return this->z;
}

void TSPoint::setX (double x)
{
	this->x = x;
}

void TSPoint::setY (double y)
{
	this->y = y;
}

void TSPoint::setZ (double z) {
	this->z = z;
}

double TSPoint::distance(const TSPoint& point) const
{
        return sqrt ( pow(x - point.x, 2) + pow(y - point.y, 2) + pow(z - point.z, 2) );
}

TSPoint operator+ (const TSPoint& pt, const TSPoint& c)
{
        TSPoint result = pt;
        result.x += c.x;
        result.y += c.y;
        result.z += c.z;
        return result;
}

TSPoint operator- (const TSPoint& pt, const TSPoint& c)
{
        TSPoint result = pt;
        result.x -= c.x;
        result.y -= c.y;
        result.z -= c.z;
        return result;
}

TSPoint operator* (const TSPoint& pt, double scalar)
{
        TSPoint result = pt;
        result.x *= scalar;
        result.y *= scalar;
        result.z *= scalar;
        return result;
}

TSPoint operator/ (const TSPoint& pt, double scalar)
{
        TSPoint result = pt;
        result.x /= scalar;
        result.y /= scalar;
        result.z /= scalar;
        return result;
}


} // namespace LSP
