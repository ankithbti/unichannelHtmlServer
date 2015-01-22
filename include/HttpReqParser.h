/* 
 * File:   HttpReqParser.h
 * Author: dev
 *
 * Created on 21 January, 2015, 12:21 AM
 */

#ifndef HTTPREQPARSER_H
#define	HTTPREQPARSER_H

#include <HttpRequest.h>

namespace unichannel {

    class HttpReqParser {
        
    public:
        
        enum result_type {
            good, bad, indeterminate
        };
        
    private:
               
        /// The current state of the parser.

        enum state {
            method_start,
            method,
            uri,
            http_version_h,
            http_version_t_1,
            http_version_t_2,
            http_version_p,
            http_version_slash,
            http_version_major_start,
            http_version_major,
            http_version_minor_start,
            http_version_minor,
            expecting_newline_1,
            header_line_start,
            header_lws,
            header_name,
            space_before_header_value,
            header_value,
            expecting_newline_2,
            expecting_newline_3
        } state_;

        /// Handle the next character of input.
        result_type consume(HttpRequest& req, char input);

        /// Check if a byte is an HTTP character.
        static bool is_char(int c);

        /// Check if a byte is an HTTP control character.
        static bool is_ctl(int c);

        /// Check if a byte is defined as an HTTP tspecial character.
        static bool is_tspecial(int c);

        /// Check if a byte is a digit.
        static bool is_digit(int c);

    public:


        HttpReqParser();
        void reset();

        

        template <typename InputIterator>
        std::tuple<result_type, InputIterator> parse(HttpRequest& req,
                InputIterator begin, InputIterator end) {
            while (begin != end) {
                result_type result = consume(req, *begin++);
                if (result == good || result == bad)
                    return std::make_tuple(result, begin);
            }
            return std::make_tuple(indeterminate, begin);
        }



    };

}



#endif	/* HTTPREQPARSER_H */

