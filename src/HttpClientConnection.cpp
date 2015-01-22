#include <HttpClientConnection.h>
#include <boost/asio/basic_socket.hpp>
#include <boost/asio/basic_stream_socket.hpp>
#include <iostream>
#include <HttpClientConnectionManager.h>
#include <ctemplate/template.h>

namespace unichannel {

    HttpClientConnection::HttpClientConnection(boost::asio::ip::tcp::socket socket, HttpClientConnectionManager& manager, HttpReqHandler& reqHanlder) :
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
        std::cout << " Going to read the HTML Req. " << std::endl;
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(buffer_),
                [this, self](boost::system::error_code ec, std::size_t bytes_transferred) {

                    std::cout << " Bytes Transferred : " << bytes_transferred << std::endl;
                    if (!ec) {
                        HttpReqParser::result_type result;

                                std::tie(result, std::ignore) = reqParser_.parse(
                                req_, buffer_.data(), buffer_.data() + bytes_transferred);

                        if (result == HttpReqParser::good) {
                            // Handle the req_ - whatever in the URL asked from user
                            std::cout << " req was good. URI: " << req_.uri_ << std::endl;                            
                            reqHandler_.handle_request(req_, reply_);
                                    //Creating a reply

                                    //                                    reply_.status_ = HttpReply::status_type::ok;
                                    //
                                    //                                    // Using ctemplates
                                    //                                    //ctemplate feature
                                    //                                    ctemplate::TemplateDictionary rootDict("example");
                                    //                                    rootDict.SetValue("TITLE", "HTML Page from CTemplates");
                                    //
                                    //                                    // Set CSS Files
                                    //                                    ctemplate::TemplateDictionary* sectionCSSDict1 = rootDict.AddSectionDictionary("CSS");
                                    //                                    sectionCSSDict1->SetValue("CSSFILE", "/var/www/html/quickNote/bootstrap/css/bootstrap.min.css");
                                    //
                                    //                                    ctemplate::TemplateDictionary* sectionCSSDict2 = rootDict.AddSectionDictionary("CSS");
                                    //                                    sectionCSSDict2->SetValue("CSSFILE", "/var/www/html/quickNote/bootstrap/css/bootstrap-responsive.min.css");
                                    //
                                    //                                    rootDict.SetValue("HEADING", "Hurraayyyy!!!!!");
                                    //                                    rootDict.SetValue("FOOTER", "We are awesome!!!!!");
                                    //
                                    //                                    std::string output;
                                    //                                    ctemplate::ExpandTemplate("../templates/myhtml.tpl", ctemplate::DO_NOT_STRIP, &rootDict, &output);
                                    //
                                    //                                    reply_.content_ = output;
                                    //                                    reply_.headers_.resize(2);
                                    //                                    reply_.headers_[0].name = "Content-Length";
                                    //                                    reply_.headers_[0].value = std::to_string(reply_.content_.size());
                                    //                                    reply_.headers_[1].name = "Content-Type";
                                    //                                    reply_.headers_[1].value = "text/html";



                                    do_write();
                        } else if (result == HttpReqParser::bad) {
                            std::cout << " req was bad. URI:  " << req_.uri_ << std::endl;
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
