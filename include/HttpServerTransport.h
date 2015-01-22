/* 
 * File:   HttpServerTransport.h
 * Author: dev
 *
 * Created on 20 January, 2015, 9:53 PM
 */

#ifndef HTTPSERVERTRANSPORT_H
#define	HTTPSERVERTRANSPORT_H

#include <Transport.h>
#include <boost/asio.hpp>
#include <map>
#include <HttpClientConnection.h>
#include <HttpClientConnectionManager.h>
#include <HttpReqHandler.h>

namespace unichannel {

    class HttpServerTransport : private Transport {
    private:

        std::string serverPort_;
        boost::asio::io_service ioService_;
        boost::asio::ip::tcp::acceptor acceptor_;
        boost::asio::ip::tcp::socket socket_;
        HttpClientConnectionManager clientConnManager_;
        /// The signal_set is used to register for process termination notifications.
        boost::asio::signal_set signals_;
        HttpReqHandler reqHandler_ ;
        void do_accept();
        void do_await_stop();


    public:

        typedef boost::shared_ptr<HttpServerTransport> Ptr;

        HttpServerTransport(const std::string& port, const std::string& docRoot);

        void start();
        void stop();

        void send(const Buffer&) {

        }

        ~HttpServerTransport() {

        }
    };

}

#endif	/* HTTPSERVERTRANSPORT_H */

