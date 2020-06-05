/*!	@file progressbar.h
 *	@author anantonov
 *	@date	Aug 23, 2013 (created)
 *	@brief	Console progress bar implementation by Ross Hemsley
 *	@link http://www.rosshemsley.co.uk/2011/02/creating-a-progress-bar-in-c-or-any-other-console-app/
 */

#ifndef PROGRESSBAR_H_
#define PROGRESSBAR_H_

#include <iostream>
#include <iomanip>

inline void progress_bar(unsigned int x, unsigned int n, unsigned int w = 50)
{
    if ( (x != n) && (x % (n/100) != 0) ) return;

    float ratio  =  x/(float)n;
    unsigned   c      =  ratio * w;

    std::cout << "\r";
    std::cout << std::setw(3) << (int)(ratio*100) << "% [";
    for (unsigned x=0; x<c; x++) std::cout << "=";
    for (unsigned x=c; x<w; x++) std::cout << " ";
    std::cout << "]" << std::flush;
}


#endif // PROGRESSBAR_H_
