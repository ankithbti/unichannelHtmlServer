/* 
 * File:   HttpClientConnectionManager.h
 * Author: dev
 *
 * Created on 21 January, 2015, 9:46 PM
 */

#ifndef HTTPCLIENTCONNECTIONMANAGER_H
#define	HTTPCLIENTCONNECTIONMANAGER_H

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <set>
#include <HttpClientConnection.h>

namespace unichannel {
    
    class HttpClientConnectionManager : private boost::noncopyable {
        
        std::set<HttpClientConnection::Ptr> clientConnections_;
        
    public:
        
        HttpClientConnectionManager();
        
        void start(HttpClientConnection::Ptr clientConn) ;
        void stop(HttpClientConnection::Ptr clientConn) ;
        void stop_all();
        

    };

}

#endif	/* HTTPCLIENTCONNECTIONMANAGER_H */

