/* 
 * File:   HttpRequest.h
 * Author: dev
 *
 * Created on 21 January, 2015, 12:09 AM
 */

#ifndef HTTPREQUEST_H
#define	HTTPREQUEST_H

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <HttpHeader.h>
#include <vector>

namespace unichannel {
    
    class HttpRequest : private boost::noncopyable {
        
    public:
        
        typedef boost::shared_ptr<HttpRequest> Ptr ;
        
        std::string method_ ;
        std::string uri_ ;
        int httpMajorVer_ ;
        int httpMinorVer_ ;
        std::vector<HttpHeader> headers_ ;
        
    };
    
}

#endif	/* HTTPREQUEST_H */

