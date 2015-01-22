#include <HttpReqParser.h>

namespace unichannel {

    HttpReqParser::HttpReqParser() : state_(method_start) {

    }

    void HttpReqParser::reset() {
        state_ = method_start;
    }

    HttpReqParser::result_type HttpReqParser::consume(HttpRequest& req, char input) {
        switch (state_) {
            case method_start:
                if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
                    return bad;
                } else {
                    state_ = method;
                    req.method_.push_back(input);
                    return indeterminate;
                }
                break;
            case method:
                if (input == ' ') {
                    state_ = uri;
                    return indeterminate;
                } else if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
                    return bad;
                } else {
                    req.method_.push_back(input);
                    return indeterminate;
                }
                break;
            case uri:
                if (input == ' ') {
                    state_ = http_version_h;
                    return indeterminate;
                } else if (is_ctl(input)) {
                    return bad;
                } else {
                    req.uri_.push_back(input);
                    return indeterminate;
                }
                break;
            case http_version_h:
                if (input == 'H') {
                    state_ = http_version_t_1;
                    return indeterminate;
                } else {
                    return bad;
                }
                break;
            case http_version_t_1:
                if (input == 'T') {
                    state_ = http_version_t_2;
                    return indeterminate;
                } else {
                    return bad;
                }
                break;
            case http_version_t_2:
                if (input == 'T') {
                    state_ = http_version_p;
                    return indeterminate;
                } else {
                    return bad;
                }
                break;
            case http_version_p:
                if (input == 'P') {
                    state_ = http_version_slash;
                    return indeterminate;
                } else {
                    return bad;
                }
                break;
            case http_version_slash:
                if (input == '/') {
                    req.httpMajorVer_ = 0;
                    req.httpMinorVer_ = 0;
                    state_ = http_version_major_start;
                    return indeterminate;
                } else {
                    return bad;
                }
                break;
            case http_version_major_start:
                if (is_digit(input)) {
                    req.httpMajorVer_ = req.httpMajorVer_ * 10 + input - '0';
                    state_ = http_version_major;
                    return indeterminate;
                } else {
                    return bad;
                }
                break;
            case http_version_major:
                if (input == '.') {
                    state_ = http_version_minor_start;
                    return indeterminate;
                } else if (is_digit(input)) {
                    req.httpMajorVer_ = req.httpMajorVer_ * 10 + input - '0';
                    return indeterminate;
                } else {
                    return bad;
                }
                break;
            case http_version_minor_start:
                if (is_digit(input)) {
                    req.httpMinorVer_ = req.httpMinorVer_ * 10 + input - '0';
                    state_ = http_version_minor;
                    return indeterminate;
                } else {
                    return bad;
                }
                break;
            case http_version_minor:
                if (input == '\r') {
                    state_ = expecting_newline_1;
                    return indeterminate;
                } else if (is_digit(input)) {
                    req.httpMinorVer_ = req.httpMinorVer_ * 10 + input - '0';
                    return indeterminate;
                } else {
                    return bad;
                }
                break;
            case expecting_newline_1:
                if (input == '\n') {
                    state_ = header_line_start;
                    return indeterminate;
                } else {
                    return bad;
                }
                break;
            case header_line_start:
                if (input == '\r') {
                    state_ = expecting_newline_3;
                    return indeterminate;
                } else if (!req.headers_.empty() && (input == ' ' || input == '\t')) {
                    state_ = header_lws;
                    return indeterminate;
                } else if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
                    return bad;
                } else {
                    req.headers_.push_back(HttpHeader());
                    req.headers_.back().name.push_back(input);
                    state_ = header_name;
                    return indeterminate;
                }
                break;
            case header_lws:
                if (input == '\r') {
                    state_ = expecting_newline_2;
                    return indeterminate;
                } else if (input == ' ' || input == '\t') {
                    return indeterminate;
                } else if (is_ctl(input)) {
                    return bad;
                } else {
                    state_ = header_value;
                    req.headers_.back().value.push_back(input);
                    return indeterminate;
                }
                break;
            case header_name:
                if (input == ':') {
                    state_ = space_before_header_value;
                    return indeterminate;
                } else if (!is_char(input) || is_ctl(input) || is_tspecial(input)) {
                    return bad;
                } else {
                    req.headers_.back().name.push_back(input);
                    return indeterminate;
                }
                break;
            case space_before_header_value:
                if (input == ' ') {
                    state_ = header_value;
                    return indeterminate;
                } else {
                    return bad;
                }
                break;
            case header_value:
                if (input == '\r') {
                    state_ = expecting_newline_2;
                    return indeterminate;
                } else if (is_ctl(input)) {
                    return bad;
                } else {
                    req.headers_.back().value.push_back(input);
                    return indeterminate;
                }
                break;
            case expecting_newline_2:
                if (input == '\n') {
                    state_ = header_line_start;
                    return indeterminate;
                } else {
                    return bad;
                }
                break;
            case expecting_newline_3:
                return (input == '\n') ? good : bad;
                break;
            default:
                return bad;
        }
    }

    bool HttpReqParser::is_char(int c) {
        return c >= 0 && c <= 127;
    }

    bool HttpReqParser::is_ctl(int c) {
        return (c >= 0 && c <= 31) || (c == 127);
    }

    bool HttpReqParser::is_tspecial(int c) {
        switch (c) {
            case '(': case ')': case '<': case '>': case '@':
            case ',': case ';': case ':': case '\\': case '"':
            case '/': case '[': case ']': case '?': case '=':
            case '{': case '}': case ' ': case '\t':
                return true;
            default:
                return false;
        }
    }

    bool HttpReqParser::is_digit(int c) {
        return c >= '0' && c <= '9';
    }




}
