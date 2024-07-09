#ifndef STREAMSCANNER_H
#define STREAMSCANNER_H
#include <fstream>
#include "Utils/endianess.h"

enum class Encoding {
	ASCII,
	NATIVE_BINARY,	// File and system have the same endianness
	EXTERNAL_BINARY,	// Data must be swapped to comply with different endianness
	};

/***
*  @brief File parsing utility, based on std:stream interface
*
*  The following class was created with the intent to decouple file specific
*  implementation details from the model loading classes, in particular this class
*  allows a loader to parse a file using the >> operator regardless of its internal
*  encoding and endianness, eliminating the necessity to create boilerplate code
*  for each encoding (Ex: readPolygons instead of readBinaryPolygon (Little and
*  Big Endian), readASCIIPolygon for each file format). The usage of the >> operator
*  allow us to chain variables with different types, reducing the original readX
*  chains from the previous implementation.
*
*  The class must be capable of changing its encoding during runtime, as certain
*  file formats combine ASCII headers with Binary data (PLY, VisF). At the same time
*  the interface should let the user query if an EOF was triggered to prevent memory
*  corruption.
*
*  @note While its tempting to separate the implementation of each encoding into
*  subclasses using inheritance, note that the >> operator requires the usage of
*  templates to keep an unique interface. C++11 prohibits the usage of abstract
*  methods in conjunction with templates, which is the reason why all encodings
*  are represented inside this object instead of having separate implementations.
*  (The state pattern also violates this restriction)
*
*  An alternate implementation using the "Policy-based class design" idiom was
*  considered during development (Mentioned in the book "Modern C++ Design"
*  by Andrei Alexandrescu), creating a policy for each encoding while having the
*  StreamScanner as a template class that inherits from this policies. While the
*  separation was possible, the information about the encoding type must be known
*  during compile-time for it to work, this information as mentioned before is
*  only available during run-time, thus making it useless for certain files.
*  While type erasure was also considered, the complexity of the pattern would
*  hinder the mantainability of the solution, so it was discarded as well.
*
*  @note Internally, both ASCII and Binary encodings are opened using the
*  std::ios::binary option, this is due to the fact that in the case that a file
*  has multiple encodings, changing the option to a different encoding will not
*  preserve the position of the caret. At the same time the ios::binary setting
*  is able to read ASCII characters in most cases (except if the type of the variable
*  is a char (See note below)
*
*  @todo Find a way to determine if the endianness of a file matches the system
*  endianness inside this class. (Currently loaders must do this check before
*  creating a StreamScanner instance)
*/

class StreamScanner{
	public:
		StreamScanner();
		~StreamScanner();
		void prepareNextLine(int offset = 0);
		void setCurrentEncoding(Encoding current);
		Encoding getCurrentEncoding();
		bool openFile(std::string filename);
		void closeFile();
		int invalidState();

		template<typename TYPE>
		friend StreamScanner& operator>>(StreamScanner& o, TYPE& target);

		template<typename TYPE>
		void toNumericValue (TYPE&);

	private:
		Encoding fileType;
		std::ifstream file;
	};

/***
*  @note When reading to char/uchar types from an ASCII file, the stored value
* will contain the ASCII character code instead of the numeric value it represents.
* (Ex: Text: 4 -> ifstream -> 52 (code for '4')), to obtain the correct value we
* must substract the character code of '0' (48) to the target value.
*
* To address this edge case we use template specialization of a char/uchar type
* in the method below. Other types dont have this behavior, so the method for the
* rest of the available types acts as a noop instruction.
*
* Further edge cases can be covered by adding more specializations to this method.
*
*/

template<typename TYPE>
void StreamScanner::toNumericValue (TYPE&){}

template<>
inline void StreamScanner::toNumericValue<unsigned char>(unsigned char& target){
	target -= '0';
	}

template<>
inline void StreamScanner::toNumericValue<char>(char& target){
	target -= '0';
	}

//Read operations
template <typename TYPE>
StreamScanner& operator>>(StreamScanner& o, TYPE& target){
   switch(o.fileType){
		case Encoding::ASCII: {
			o.file >> target;
			o.toNumericValue(target);
			break;
			}
		case Encoding::NATIVE_BINARY: {
			o.file.read((char*)&target, sizeof(TYPE));
			break;
			}
		case Encoding::EXTERNAL_BINARY: {
			o.file.read((char*)&target, sizeof(TYPE));
			target = Endianess::reverseBytes(target);
			break;
			}
		}
	return o;
	}

///@todo Write operations (For proper std::endl management see https://stackoverflow.com/questions/3662654/conditional-debug-output-class-with-templated-operator/3662786#3662786)

#endif //STREAMSCANNER_H
