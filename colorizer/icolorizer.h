/*! @file icolorizer.h
 * 	@author Anatoliy Antonov
 *  @brief IColorizer interface
 */

#ifndef ICOLORIZER_H_
#define ICOLORIZER_H_

namespace VCGL{

/*! @brief Interface for colorizers
 */
struct IColorizer {

	///types of predefined colorizers
	enum ColorizerType {
		BLUE_WHITE_RED, 	///< Three-point transfer function.
		HEAT,				///< Heat, or black body radiance transfer function
		FLAME,				///< Flame-like transfer function.
		CUSTOM				///< Custom type
	};

	enum ColorizerMode {
		CONTINUOUS = 0,		///< Linearly interpolated color between enclosing records
		BANDED_MIDPOINT,	///< Nearest neighbor color
		BANDED_SIDES		///< Color of the lower neigbor
	};

	/*! @brief Get type of colorizers.
	 * @return Value of enum ColorizerType corresponding to the current colorizer.
	 */
	virtual ColorizerType getType() const = 0;

	/*! @brief Colorize a single value.
	 * @pre -1 <= val <= 1
	 * @param[in] val Floating-point value in range [0,1]
	 * @param[out] pr Pointer to float variable receiving red channel value (in [0,1]).
	 * @param[out] pg Pointer to float variable receiving green channel value (in [0,1]).
	 * @param[out] pb Pointer to float variable receiving blue channel value (in [0,1]).
	 */
	virtual void colorize(float val, float* pr, float *pg, float *pb) const = 0;

	/// Virtual destructor for proper memory deallocation.
	virtual ~IColorizer() {}

	/// Factory method to create colorizers
	static IColorizer* createColorizer(ColorizerType type);
};

} //namespace VCGL
#endif /* ICOLORIZER_H_ */
