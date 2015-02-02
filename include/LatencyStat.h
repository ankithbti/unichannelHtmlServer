/* 
 * File:   LatencyStat.h
 * Author: dev
 *
 * Created on 1 February, 2015, 3:26 PM
 */

#ifndef LATENCYSTAT_H
#define	LATENCYSTAT_H

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace unichannel {
    
    class LatencyStat : private boost::noncopyable {
        
        std::string id_ ;
        long long dateTime_ ;
        long latencyInMicro_ ;
        
    public:
        
        typedef boost::shared_ptr<LatencyStat> Ptr ;
        
        LatencyStat(std::string id, long long dateTime, long latencyInMicro) : id_(id), dateTime_(dateTime), latencyInMicro_(latencyInMicro) {
            
        } 
        
        const long long& getDateTime() const { return dateTime_ ; }
        const long& getLatencyInMicro() const { return latencyInMicro_ ; }
        const std::string& getId() const { return id_ ; }
        std::tuple<std::string, long long, long> getLatencyStat() const { return std::make_tuple(id_, dateTime_, latencyInMicro_) ; } 
        
    };
    
    
}

#endif	/* LATENCYSTAT_H */

