#include <HttpServerTransport.h>
#include <iostream>
#include <signal.h>
#include <utility>

namespace unichannel {

    HttpServerTransport::HttpServerTransport(const std::string& port, const std::string& docRoot) :
    serverPort_(port),
    ioService_(),
    acceptor_(ioService_),
    socket_(ioService_),
    signals_(ioService_),
    reqHandler_(docRoot) {
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

        do_accept();
    }

    void HttpServerTransport::do_await_stop() {
        signals_.async_wait(
                [this](boost::system::error_code /*ec*/, int signo) {
                    // The server is stopped by cancelling all outstanding asynchronous
                    // operations. Once all operations have finished the io_service::run()
                    // call will exit.
                    std::cout << " Signal Comes: " << signo << std::endl;
                    acceptor_.close();
                            clientConnManager_.stop_all();
                });
    }

    void HttpServerTransport::do_accept() {

        std::cout << " Ready to accept connections on " + serverPort_ << std::endl;

        acceptor_.async_accept(socket_, [this](boost::system::error_code ec) {
            if (!acceptor_.is_open()) {
                std::cout << " Acceptor is not open to accept ant new connection. " << std::endl;
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
        ioService_.run();
    }

    void HttpServerTransport::stop() {
        if (acceptor_.is_open()) {
            acceptor_.close();
        }
    }

}
