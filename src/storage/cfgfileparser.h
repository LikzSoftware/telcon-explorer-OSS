/*! @file cfgfileparser.h
 *  @author Petar Dobrev, Tran Van Long
 *  @date Oct 4, 2012 (added to VCGLShared)
 *  @brief Class for parsing configuration files
 */

#ifndef __CFGFILEPARSER_H
#define __CFGFILEPARSER_H

#include <iostream>
#include <map>

namespace VCGL {

class CfgFileParser {
public:
     CfgFileParser(const char* fname);
     CfgFileParser();
     bool readConfig(const char* fname);
     /*!
      *
      * @param name Name of the attribute to look up for.
      * @param type Type of the memory ptr is pointing to: 's' for char array, 'i' for integer, 'f' for float.
      * @param size Size of the memory ptr is pointing to.
      * @param ptr Memory, to which the attribute value is stored.
      * @return true if value was retrieved successfully, otherwise false
      */
     bool getAttr(std::string name, char type, size_t size, void* ptr);

private:
     std::map<std::string, std::string> attr;
     std::string killWhitespaces(std::string);
     std::string removeComments(std::string);
}; // class CfgFileParser

} // namespace VCGL

#endif
