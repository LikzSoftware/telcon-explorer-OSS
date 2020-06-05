/*! @file cppunitextras.cpp
 * @author anantonov
 * @date Created on May 18, 2014
 *
 * @brief TODO: Add brief description of the file contents
 */

#include "cppunitextras.h"
#include "exploration/maps/mrect.h"

SimpleString StringFrom (const QPoint& value)
{
	const int DEFAULT_SIZE = 20;
	char buffer [2*DEFAULT_SIZE];
	sprintf (buffer, "(%d,%d)", value.x(), value.y());

	return SimpleString(buffer);
}

SimpleString StringFrom(const VCGL::MRect& value) {
	const int BUFFER_SIZE = 100;
	char buffer [BUFFER_SIZE];
	sprintf (buffer, "[%d, %d, %d, %d]", value.left, value.top, value.right, value.bottom);

	return SimpleString(buffer);
}
