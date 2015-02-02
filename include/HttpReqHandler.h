/* 
 * File:   HttpReqHandler.h
 * Author: dev
 *
 * Created on 23 January, 2015, 12:58 AM
 */

#ifndef HTTPREQHANDLER_H
#define	HTTPREQHANDLER_H

#include <boost/noncopyable.hpp>
#include <HttpRequest.h>
#include <HttpReply.h>

#include <queue>

#include "LatencyStat.h"
#include <boost/thread.hpp>

namespace unichannel {

    class HttpReqHandler : private boost::noncopyable {
        
        std::queue<LatencyStat::Ptr> statQ_ ;
        boost::mutex statQMutex_ ;
        
    public:

        struct mapping {
            const char* extension;
            const char* mime_type;
        } mimemaps[5] ;
        
        

        typedef boost::shared_ptr<HttpReqHandler> Ptr;
        
        /// Construct with a directory containing files to be served.
        explicit HttpReqHandler(const std::string& doc_root);


        /// Handle a request and produce a reply.
        void handle_request(const HttpRequest& req, HttpReply& rep);
        
        void addStat(LatencyStat::Ptr stat);
        
    private:
        /// The directory containing the files to be served.
        std::string doc_root_;
        
        std::string getLatestPoint();

        std::string extension_to_type(const std::string& extension) {
            for (mapping m : mimemaps) {
                if (m.extension == extension) {
                    return m.mime_type;
                }
            }

            return "text/plain";
        }

        /// Perform URL-decoding on a string. Returns false if the encoding was
        /// invalid.
        static bool url_decode(const std::string& in, std::string& out);
    };

}


#endif	/* HTTPREQHANDLER_H */

