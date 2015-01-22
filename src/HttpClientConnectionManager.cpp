#include <HttpClientConnectionManager.h>
#include <boost/foreach.hpp>

namespace unichannel {

    HttpClientConnectionManager::HttpClientConnectionManager() {

    }

    void HttpClientConnectionManager::stop(HttpClientConnection::Ptr clientConn) {
        
        clientConnections_.erase(clientConn);
        clientConn->stop();
    }

    void HttpClientConnectionManager::stop_all() {

        BOOST_FOREACH(HttpClientConnection::Ptr cc, clientConnections_){
            cc->stop();
        }
        clientConnections_.clear();

    }

    void HttpClientConnectionManager::start(HttpClientConnection::Ptr clientConn) {
        clientConnections_.insert(clientConn);
        clientConn->start();
    }



}
