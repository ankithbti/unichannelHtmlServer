#include <HttpReqHandler.h>
#include <fstream>

namespace unichannel {

    HttpReqHandler::HttpReqHandler(const std::string& doc_root) : doc_root_(doc_root) {
        mimemaps[0].extension = "gif" ;
        mimemaps[0].mime_type = "image/gif" ;
        mimemaps[1].extension = "htm" ;
        mimemaps[1].mime_type = "text/htm" ;
        mimemaps[2].extension = "html" ;
        mimemaps[2].mime_type = "text/html" ;
        mimemaps[3].extension = "jpg" ;
        mimemaps[3].mime_type = "image/jpeg" ;
        mimemaps[4].extension = "png" ;
        mimemaps[4].mime_type = "image/png" ;
    }

    bool HttpReqHandler::url_decode(const std::string& in, std::string& out) {
        out.clear();
        out.reserve(in.size());
        for (std::size_t i = 0; i < in.size(); ++i) {
            if (in[i] == '%') {
                if (i + 3 <= in.size()) {
                    int value = 0;
                    std::istringstream is(in.substr(i + 1, 2));
                    if (is >> std::hex >> value) {
                        out += static_cast<char> (value);
                        i += 2;
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
            } else if (in[i] == '+') {
                out += ' ';
            } else {
                out += in[i];
            }
        }
        return true;
    }

    void HttpReqHandler::handle_request(const HttpRequest& req, HttpReply& rep) {
        // Decode url to path.
        std::string request_path;
        if (!url_decode(req.uri_, request_path)) {
            rep = HttpReply::stock_reply(HttpReply::bad_request);
            return;
        }

        // Request path must be absolute and not contain "..".
        if (request_path.empty() || request_path[0] != '/'
                || request_path.find("..") != std::string::npos) {
            rep = HttpReply::stock_reply(HttpReply::bad_request);
            return;
        }

        // If path ends in slash (i.e. is a directory) then add "index.html".
        if (request_path[request_path.size() - 1] == '/') {
            request_path += "index.html";
        }

        // Determine the file extension.
        std::size_t last_slash_pos = request_path.find_last_of("/");
        std::size_t last_dot_pos = request_path.find_last_of(".");
        std::string extension;
        if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos) {
            extension = request_path.substr(last_dot_pos + 1);
        }

        // Open the file to send back.
        std::string full_path = doc_root_ + request_path;
        std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
        if (!is) {
            rep = HttpReply::stock_reply(HttpReply::not_found);
            return;
        }

        // Fill out the reply to be sent to the client.
        rep.status_ = HttpReply::ok;
        char buf[512];
        while (is.read(buf, sizeof (buf)).gcount() > 0)
            rep.content_.append(buf, is.gcount());
        rep.headers_.resize(2);
        rep.headers_[0].name = "Content-Length";
        rep.headers_[0].value = std::to_string(rep.content_.size());
        rep.headers_[1].name = "Content-Type";
        rep.headers_[1].value = extension_to_type(extension);

    }


}
