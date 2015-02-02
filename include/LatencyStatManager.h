/* 
 * File:   LatencyStatManager.h
 * Author: dev
 *
 * Created on 1 February, 2015, 3:47 PM
 */

#ifndef LATENCYSTATMANAGER_H
#define	LATENCYSTATMANAGER_H

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <queue>

#include "LatencyStat.h"



namespace unichannel {
    
    class HttpReqHandler ;

    class LatencyStatManager : private boost::noncopyable {
        
        std::queue<LatencyStat::Ptr> latencyStatQ_ ;
        boost::thread_group producerThreadGroup_ ;
        boost::thread_group consumerThreadGroup_ ;
        boost::condition_variable latencyStatQCondVar_ ;
        boost::mutex latencyStatQMutex_ ;
        mutable boost::mutex startStopMutex_ ;
        int numOfConsumerThreads_ ;
        volatile bool isAlive_ ;
        boost::shared_ptr<HttpReqHandler> reqHandler_ ;
        
        void consumerThread();
        
    public:
        
        typedef boost::shared_ptr<LatencyStatManager> Ptr ;
        
        
        LatencyStatManager(int consumerThreadCount = 1) ;
        ~LatencyStatManager() {
            stop();
        }
        
        void setReqHandler(boost::shared_ptr<HttpReqHandler> handler) {
            if(handler){
                reqHandler_ = handler ;
            }else{
                throw std::runtime_error(" LatencyStatManager::setReqHandler() - Null Handler passed. ");
            }
        }
        
        void start() ;        
        void stop() ;
        bool isAlive() const {
            
            boost::unique_lock<boost::mutex> lock(startStopMutex_);
            return isAlive_ ;
            
        }
        
        // Some Callback function that we need to register to receive updates about latency from some other Library
        // This function will actually work as producer
        // We will register this as a callback function
        void onLatencyUpdate(std::string id, long long dateTime, long latencyInMicro);
    };

}


#endif	/* LATENCYSTATMANAGER_H */

