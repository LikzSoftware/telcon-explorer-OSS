/*! @file rgb.h
 * @date Created on Apr 30, 2014
 * @author anantonov
 *
 * @brief Simple value struct to hold a color
 */

#ifndef RGB_H_
#define RGB_H_

namespace VCGL {
	/// Simple value struct to hold a color
	struct RGBF {
		float r; ///< red channel value in [0,1]
		float g; ///< green channel value in [0,1]
		float b; ///< blue channel value in [0,1]
	};

	/// Compare two colors.
	inline bool operator==(const RGBF& a, const RGBF& b) {
		return (a.r == b.r && a.g == b.g && a.b == b.b);
	}
}

#endif /* RGB_H_ */
