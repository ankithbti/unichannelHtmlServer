#include "LatencyStatManager.h"
#include "HttpReqHandler.h"
#include <boost/bind.hpp>
#include <tuple>

namespace unichannel {

    LatencyStatManager::LatencyStatManager(int consumerThreadCount) : numOfConsumerThreads_(consumerThreadCount),
    isAlive_(false) {

    }

    void LatencyStatManager::consumerThread() {

        while (isAlive_) {

            LatencyStat::Ptr latencyStat;
            {
                boost::unique_lock<boost::mutex> lock(latencyStatQMutex_);
                while (latencyStatQ_.size() == 0) {
                    latencyStatQCondVar_.wait(lock);
                }
                latencyStat = latencyStatQ_.front();
                latencyStatQ_.pop();
            }
            // Got LatencyStat - We can do whatever we want
            std::string id;
            long long dateTime;
            long latencyInMicro;
            std::tie(id, dateTime, latencyInMicro) = latencyStat->getLatencyStat();
            std::cout << " LatencyStatManager::consumerThread() Going to Add - [ " << boost::this_thread::get_id() << " ] - We got LatencyStat with Id: " << id << " , dateTime: " << dateTime << " , latencyInMicro: " << latencyInMicro << std::endl;
            
            // Add this to the HttpReqHandler Q
            reqHandler_->addStat(latencyStat);
            
            
        }

        std::cout << " LatencyStatManager::consumerThread() [ " << boost::this_thread::get_id() << " ] - Coming out of Consumer Thread, " << std::endl;


    }

    void LatencyStatManager::onLatencyUpdate(std::string id, long long dateTime, long latencyInMicro) {

        if (!isAlive()) {
            throw std::runtime_error(" LatencyStatManager::onLatencyUpdate() - Warning : Manager is not started yet. Can not add Latency Stats right now. ");
            return;
        }

        LatencyStat::Ptr latencyStat(new LatencyStat(id, dateTime, latencyInMicro));

        {
            boost::unique_lock<boost::mutex> lock(latencyStatQMutex_);
            latencyStatQ_.push(latencyStat);
            latencyStatQCondVar_.notify_one();
        }


    }

    void LatencyStatManager::start() {
        
        {
            boost::unique_lock<boost::mutex> lock(startStopMutex_);
            isAlive_ = true;
        }
        
        for (int i = 0; i < numOfConsumerThreads_; ++i) {
            consumerThreadGroup_.create_thread(boost::bind(&LatencyStatManager::consumerThread, this));
        }
    }

    void LatencyStatManager::stop() {
        
        {
            boost::unique_lock<boost::mutex> lock(startStopMutex_);
            isAlive_ = false;
        }

        consumerThreadGroup_.interrupt_all();
        consumerThreadGroup_.join_all();
    }

}
