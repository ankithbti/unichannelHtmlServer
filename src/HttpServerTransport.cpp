#include <HttpServerTransport.h>
#include <iostream>
#include <signal.h>
#include <utility>
#include <boost/thread/detail/thread.hpp>

namespace unichannel {

    HttpServerTransport::HttpServerTransport(const std::string& port, LatencyStatManager::Ptr statManager, const std::string& docRoot) :
    serverPort_(port),
    ioService_(),
    acceptor_(ioService_),
    socket_(ioService_),
    signals_(ioService_),
    reqHandler_(new HttpReqHandler(docRoot)),
    statManager_(statManager),
    isAlive_(false) {
        // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
        boost::asio::ip::tcp::resolver resolver(ioService_);
        boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({"127.0.0.1", serverPort_});
        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();

        // Register to handle the signals that indicate when the server should exit.
        // It is safe to register for the same signal multiple times in a program,
        // provided all registration for the specified signal is made through Asio.
        signals_.add(SIGINT);
        signals_.add(SIGTERM);
#if defined(SIGQUIT)
        signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)

        do_await_stop();
        
        statManager_->setReqHandler(reqHandler_);

        do_accept();
    }

    void HttpServerTransport::do_await_stop() {
        signals_.async_wait(
                [this](boost::system::error_code /*ec*/, int signo) {
                    // The server is stopped by cancelling all outstanding asynchronous
                    // operations. Once all operations have finished the io_service::run()
                    // call will exit.
                    std::cout << " Signal Comes: " << signo << std::endl;
                    stop();
                });
    }

    void HttpServerTransport::do_accept() {

        std::cout << " Ready to accept connections on " + serverPort_ << std::endl;

        acceptor_.async_accept(socket_, [this](boost::system::error_code ec) {
            if (!acceptor_.is_open()) {
                std::cout << " Acceptor is not open to accept any new connection. " << std::endl;
                return;
            }

            if (!ec) {
                // Create new HttpClientConnection using the above socket
                std::cout << " New Client connection created. " << std::endl;
                HttpClientConnection::Ptr clientConn(new HttpClientConnection(std::move(socket_), clientConnManager_, reqHandler_));
                clientConnManager_.start(clientConn);

            } else {
                std::cout << " Error while accepting : " << ec.message() << std::endl;
            }

            // Ready to accept new request
            do_accept();
        });
    }

    void HttpServerTransport::start() {
        {
            boost::unique_lock<boost::mutex> lock(startStopMutex_);
            isAlive_ = true;
        }

        ioServiceRunThread_ = boost::shared_ptr<boost::thread>(new boost::thread(&HttpServerTransport::runIoService, this));
    }

    void HttpServerTransport::stop() {

        {
            boost::unique_lock<boost::mutex> lock(startStopMutex_);
            isAlive_ = false;
        }

        ioServiceRunThread_->interrupt();

        std::cout << " Stopped IoServceRun Thread. Now Closing Acceptor. " << std::endl;

        if (acceptor_.is_open()) {
            acceptor_.close();
        }

        clientConnManager_.stop_all();
    }

    void HttpServerTransport::runIoService() {
        std::cout << " Starting IoServceRun Thread. " << std::endl;
        ioService_.run();
        std::cout << " Stopping IoServceRun Thread. " << std::endl;
    }

}
