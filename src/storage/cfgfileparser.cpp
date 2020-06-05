/*! @file cfgfileparser.cpp
 *  @author Petar Dobrev, Tran Van Long
 *  @date Oct 4, 2012 (added to VCGLShared)
 *  @brief Implementation of class for parsing configuration files
 */

#include <iostream>
#include <stdio.h>
#include <string.h>

#include "cfgfileparser.h"

using namespace std;

namespace VCGL {

CfgFileParser::CfgFileParser() {}

CfgFileParser::CfgFileParser(const char *fname)
{
     readConfig(fname);
}

string CfgFileParser::removeComments(string st)
{
     int loc = st.find("//", 0);
     if (loc == (int)string::npos)
	  return st;
     else
     {
	  return st.substr(0, loc);
     }
}

string CfgFileParser::killWhitespaces(string st)
{
     int loc = st.find_first_of(" \n\t");
     while (loc != (int)string::npos)
     {
	  st.erase(loc, 1);
	  loc = st.find_first_of(" \n\t");
     }
     return st;
}


bool CfgFileParser::readConfig(const char *fname)
{
     char buf[255];
     string line;
     int line_num=0;
     string first, second;

     if(fname != NULL) { // is it enough only to check zero pointer to string? (Anatoliy Antonov, 04.10.2012)
       	 FILE *fp;
         fp = fopen(fname, "rt");
         if (fp == NULL) {
        	 cerr << "Parser::readConfig: cannot open configuration file" << endl;
         }
         else {
        	 while (!feof(fp)) {
				if(fgets(buf, 255, fp)==NULL) {
//					cout << "Empty line" << endl;
				}
				line_num++;
				line = string(buf);
				line = removeComments(line);
				line = killWhitespaces(line);

                if (line=="") continue;
                int loc = line.find("=");
                if (loc==(int)string::npos) {
                	cout << "Syntax error at line " << line_num << " of " << fname << ": missing =" << endl;
                    return false;
                }
                first = line.substr(0, loc);
                second = line.substr(loc+1);
                attr[first] = second;
        	 }
        	 fclose(fp);
        	 return true;
         }
     }
     else {
    	 cout << "Empty file name" << endl;
     }
     return false;
}


bool CfgFileParser::getAttr(std::string name, char type, size_t size, void* ptr)
{
	bool result = true;

    if (attr[name] == "") {
    	 cerr << "Couldn't find attribute " << name << endl;
    	 result = false;
    }

    if (result) {
		switch(type) {
		case 's': //string
			strncpy((char*)ptr, attr[name].c_str(), size);
			break;
		case 'i': //integer
			if (size != sizeof(int) ||  1 != sscanf(attr[name].c_str(), "%d", (int*)ptr)) {
				 cerr << "Couldn't read integer value for " << name << endl;
				 result = false;
			}
			break;
		case 'f': //float
			if (size != sizeof(float) || 1 != sscanf(attr[name].c_str(), "%f", (float*)ptr)) {
				 cerr << "Couldn't read value for " << name << endl;
				 result = false;
			}
			break;
		default:
			result = false;
			break;
		}
    }
    return result;
}

} // namespace VCGL
