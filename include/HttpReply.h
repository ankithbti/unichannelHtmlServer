/* 
 * File:   HttpReply.h
 * Author: dev
 *
 * Created on 21 January, 2015, 1:00 AM
 */

#ifndef HTTPREPLY_H
#define	HTTPREPLY_H

#include <HttpHeader.h>
#include <vector>
#include <boost/asio.hpp>

namespace unichannel {

    const char name_value_separator[] = {':', ' '};
    const char crlf[] = {'\r', '\n'};

    class HttpReply {
    public:
        /// The status of the reply.

        enum status_type {
            ok = 200,
            created = 201,
            accepted = 202,
            no_content = 204,
            multiple_choices = 300,
            moved_permanently = 301,
            moved_temporarily = 302,
            not_modified = 304,
            bad_request = 400,
            unauthorized = 401,
            forbidden = 403,
            not_found = 404,
            internal_server_error = 500,
            not_implemented = 501,
            bad_gateway = 502,
            service_unavailable = 503
        } status_;

        /// The headers to be included in the reply.
        std::vector<HttpHeader> headers_;

        /// The content to be sent in the reply.
        std::string content_;

        /// Convert the reply into a vector of buffers. The buffers do not own the
        /// underlying memory blocks, therefore the reply object must remain valid and
        /// not be changed until the write operation has completed.
        std::vector<boost::asio::const_buffer> to_buffers();

        /// Get a stock reply.
        static HttpReply stock_reply(status_type status);

    };
}

#endif	/* HTTPREPLY_H */

