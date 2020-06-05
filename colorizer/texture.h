/*! @file texture.h
 * 	@author Anatoliy Antonov
 * 	@date Sep 22, 2011 (created)
 *  @brief Simple texture class
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "rgb.h"
#include <cstring>

namespace VCGL {

class Texture {
public:
	/// Construct empty object
	Texture(): pTex(0), w(0), h(0) {}

	/// Copy another texture
	Texture(const Texture& o): pTex(0), w(0), h(0) {
		allocateMemory(o.w, o.h);
		std::memcpy(pTex, o.pTex, w*h*sizeof(RGBF));
	}

	/// Assignment operator
	const Texture& operator= (const Texture& o) {
		if (pTex != o.pTex) {
			allocateMemory(o.w, o.h);
			std::memcpy(pTex, o.pTex, w*h*sizeof(RGBF));
		}
		return *this;
	}

	/// Destructor
	virtual ~Texture() {
		freeMemory();
	}

	/// Allocate memory for storing a texture of specified sizes
	void allocateMemory(int width, int height) {
		freeMemory();
		if (width > 0 && height > 0) {
			pTex = new RGBF[width*height];
			w = width;
			h = height;
		}
	}

	/// Free memory
	void freeMemory() {
		if (pTex != 0) {
			delete pTex;
			pTex = 0;
		}
		w = 0;
		h = 0;
	}

	/// Test if the texture object is empty
	bool isValid() { return pTex != 0 && w > 0 && h > 0; }

	/*! @brief Set a pixel at a given position to the specified color
	 *
	 * @param tx horizontal texture coordinate
	 * @param ty vertical texture coordinate
	 * @param r	 red channel value
	 * @param g  green channel value
	 * @param b  blue channel value
	 */
	void setPixel(int tx, int ty, float r, float g, float b) {
		if (pTex != 0 && tx < w && ty < h) {
			pTex[ty*w+tx].r = r;
			pTex[ty*w+tx].g = g;
			pTex[ty*w+tx].b = b;
		}
	}

	/// Access the raw pointer to the data buffer
	void* getBits() const {
		return reinterpret_cast<void*>(pTex);
	}

	/// Get width of the texture
	int getWidth() const {
		return w;
	}

	/// Get height of the texture
	int getHeight() const {
		return h;
	}

private:
	RGBF* pTex; ///< data buffer
	int w;		///< texture width
	int h;		///< texture height
};

} /* namespace VCGL */
#endif /* TEXTURE_H_ */
