/*!	@file transferfunctionobject.h
 *	@author anantonov
 *	@date	Feb 18, 2014 (created)
 *	@brief	Definition of the class which implements the transfer function
 */

#ifndef TRANSFERFUNCTIONOBJECT_H_
#define TRANSFERFUNCTIONOBJECT_H_

#include "icolorizer.h"
#include <vector>
#include <string>

#include <QColor>

namespace VCGL {

/// Element of transfer function
struct TFRecord
{
    float pos;     	///< position [0,1]
    QColor qclr; 	///< color

    TFRecord(): pos(0), qclr(Qt::black) {}
    TFRecord(float pos, float r, float g, float b) {
    	this->pos = pos;
    	this->qclr.setRgbF(r,g,b,1.0);
    }
    TFRecord(float pos, float r, float g, float b, float a) {
		this->pos = pos;
		this->qclr.setRgbF(r,g,b,a);
	}
};


/// Implementation of the transfer function
class TransferFunctionObject: public IColorizer {
public:
	/// @name Predefined transfer functions (for [0,1] data range)
	//@{
		static TransferFunctionObject HeatTFO(); ///< Heat, or black body radiance transfer function
		static TransferFunctionObject BlueWhiteRedTFO(); ///< Three-point transfer function.
		static TransferFunctionObject FlameTFO(); ///< Flame-like transfer function.
	//@}

	/// Constructor
	TransferFunctionObject();
	/// Copy constructor
	TransferFunctionObject(const TransferFunctionObject& other);
	/// Assignment operator
	const TransferFunctionObject& operator= (const TransferFunctionObject& other);

	/// (deprecated) Return the type of the transfer function
	virtual ColorizerType getType() const override { return CUSTOM; }

	/*! @brief Find an appropriate color for the given value
	 * @attention According to the mode, could be a color at a neighboring node,
	 * 				or a combination of colors in two enclosing nodes
	 *
	 * @param val Data value to colorize
	 * @param pr,pg,pb Pointers that receive respective color channel values
	 */
	virtual void colorize(float val, float* pr, float *pg, float *pb) const override;
	/// Virtual destructor for proper memory deallocation.
	virtual ~TransferFunctionObject() {}

	/// Access the record at the specified index.
	const TFRecord& getTFRecord(unsigned i) const;

	/// Add a new record to the end of the current transfer function
	void addTFRecord(const TFRecord& tfr);
	/// Insert the given record at the specified index
	void insertTFRecord(unsigned index, const TFRecord& tfr);
	/// Remove a record at the specified index
	void removeTFRecord(unsigned index);
	/// Get number of records in this transfer function
	unsigned getRecordsCount() const;
	/// Access the record color at the specified index
	void getRecordColor(unsigned i, float* pr, float* pg, float* pb) const;
	/// Write the given color to the record at the specified index
	void setRecordColor(unsigned i, float r, float g, float b);
	/// Acess the record position at the specified index
	float getRecordPos(unsigned i) const;
	/// Write the given position to the
	void setRecordPos(unsigned i, float newPos);

	/// Get the mode of the transfer function (@see VCGL::IColorizer::ColorizerMode)
	enum VCGL::IColorizer::ColorizerMode getMode() const;
	/// Set the mode of the transfer function (@see VCGL::IColorizer::ColorizerMode)
	void setMode(enum VCGL::IColorizer::ColorizerMode newMode);

	/// Get the name of the transfer function
	const std::string& getName() const;
	/// Set the name of the transfer function
	void setName(const std::string& newName);

	/// Compare this transfer function to another
	bool operator==(const VCGL::TransferFunctionObject& other) const;
private:
	std::vector< TFRecord > transferFunction; ///< Collection of transfer function records
	enum VCGL::IColorizer::ColorizerMode mode; ///< Mode of the transfer function
	std::string name;	///< Name of the transfer function.
};

} //namespace VCGL



#endif // TRANSFERFUNCTIONOBJECT_H_
