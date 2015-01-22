/* 
 * File:   HttpHeader.h
 * Author: dev
 *
 * Created on 21 January, 2015, 12:12 AM
 */

#ifndef HTTPHEADER_H
#define	HTTPHEADER_H

#include <string>

namespace unichannel {
    
    class HttpHeader {
    public:
        std::string name ;
        std::string value ;
    };
    
}

#endif	/* HTTPHEADER_H */

