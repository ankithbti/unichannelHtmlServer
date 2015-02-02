/* 
 * File:   HttpClientConnection.h
 * Author: dev
 *
 * Created on 20 January, 2015, 9:59 PM
 */

#ifndef HTTPCLIENTCONNECTION_H
#define	HTTPCLIENTCONNECTION_H

#include <Transport.h>
#include <boost/array.hpp>
#include <HttpReqParser.h>
#include <HttpReply.h>
#include <boost/enable_shared_from_this.hpp>
#include <HttpReqHandler.h>

namespace unichannel {
    
    class HttpClientConnectionManager ;

    class HttpClientConnection : public Transport, public boost::enable_shared_from_this<HttpClientConnection> {
    private:

        boost::asio::ip::tcp::socket socket_;
        boost::array<char, 8192> buffer_ ;
        HttpReqParser reqParser_ ;
        HttpRequest req_ ;
        HttpReply reply_ ;
        std::string clientId_ ;
        HttpClientConnectionManager& manager_ ;
        HttpReqHandler::Ptr reqHandler_ ;
        

        /// Perform an asynchronous read operation.
        void do_read();

        /// Perform an asynchronous write operation.
        void do_write();

    public:

        typedef boost::shared_ptr<HttpClientConnection> Ptr;

        HttpClientConnection(boost::asio::ip::tcp::socket socket, HttpClientConnectionManager& manager, HttpReqHandler::Ptr reqHanlder) ;

        void start();
        void stop();
        void send(const Buffer&) {
            
        }

        ~HttpClientConnection() {

        }
    };

}

#endif	/* HTTPCLIENTCONNECTION_H */

