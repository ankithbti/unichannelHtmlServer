#include <HttpClientConnection.h>
#include <boost/asio/basic_socket.hpp>
#include <boost/asio/basic_stream_socket.hpp>
#include <iostream>
#include <HttpClientConnectionManager.h>
#include <ctemplate/template.h>

namespace unichannel {

    HttpClientConnection::HttpClientConnection(boost::asio::ip::tcp::socket socket, HttpClientConnectionManager& manager, HttpReqHandler::Ptr reqHanlder) :
    socket_(std::move(socket)),
    manager_(manager),
    reqHandler_(reqHanlder) {

    }

    void HttpClientConnection::start() {
        do_read();
    }

    void HttpClientConnection::stop() {
        socket_.close();
    }

    void HttpClientConnection::do_read() {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(buffer_),
                [this, self](boost::system::error_code ec, std::size_t bytes_transferred) {
                    if (!ec) {
                        HttpReqParser::result_type result;

                                std::tie(result, std::ignore) = reqParser_.parse(
                                req_, buffer_.data(), buffer_.data() + bytes_transferred);

                        if (result == HttpReqParser::good) {
                            // Handle the req_ - whatever in the URL asked from user
                            reqHandler_->handle_request(req_, reply_);
                                    do_write();
                        } else if (result == HttpReqParser::bad) {
                                    reply_ = HttpReply::stock_reply(HttpReply::bad_request);
                                    do_write();
                        } else {
                            do_read();
                        }
                    } else if (ec != boost::asio::error::operation_aborted) {
                        std::cout << " Error Message " << ec.message() << std::endl;
                                manager_.stop(shared_from_this());
                    } else {
                        std::cout << " Error Message " << ec.message() << std::endl;
                    }

                });
    }

    void HttpClientConnection::do_write() {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, reply_.to_buffers(),
                [this, self](boost::system::error_code ec, std::size_t) {
                    if (!ec) {
                        // Initiate graceful connection closure.
                        boost::system::error_code ignored_ec;
                        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
                                ignored_ec);
                    }

                    if (ec != boost::asio::error::operation_aborted) {
                        manager_.stop(shared_from_this());
                    }
                });
    }

}
