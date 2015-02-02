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
#include <boost/thread.hpp>

#include "LatencyStatManager.h"

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
        HttpReqHandler::Ptr reqHandler_;
        LatencyStatManager::Ptr statManager_ ;
        boost::shared_ptr<boost::thread> ioServiceRunThread_;
        mutable boost::mutex startStopMutex_;
        volatile bool isAlive_;
        void do_accept();
        void do_await_stop();
        void runIoService();


    public:

        typedef boost::shared_ptr<HttpServerTransport> Ptr;

        HttpServerTransport(const std::string& port, LatencyStatManager::Ptr statManager, const std::string& docRoot);

        void start();
        void stop();

        bool isAlive() const {
            boost::unique_lock<boost::mutex> lock(startStopMutex_);
            return isAlive_;
        }

        void send(const Buffer&) {

        }

        ~HttpServerTransport() {
            stop();
        }
    };

}

#endif	/* HTTPSERVERTRANSPORT_H */

