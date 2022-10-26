#ifndef __SERVERTHREAD_H__
#define __SERVERTHREAD_H__

#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <map>

#include "Messages.h"
#include "ServerSocket.h"

struct AdminRequest {
	MapOp operation;
	std::promise<int> prom;
};

class LaptopFactory {
private:
    // needed to send a request to admin
	std::queue<std::unique_ptr<AdminRequest>> erq;
	std::mutex erq_lock;
	std::condition_variable erq_cv;

    // needed to process a request by admin
    int factory_id;
    int primary_id = -1;
    int number_of_peers;
    std::vector<peer_info> peer_info_vector;
    std::mutex records_lock;
    std::vector<MapOp> smr_log;
    std::map<int, int> customer_record;
    int last_index = -1;
    int committed_index = -1;

    // Engineer role helper
	LaptopInfo CreateRegularLaptop(CustomerRequest request, int engineer_id);
    CustomerRecord ReadCustomerRecord(int customer_id);

    // srm_log and map helpers
    void AddOppToLog(MapOp op, int idx);
    void ApplyOpp(int idx);

public:
    void setFactoryID(int id);
    void setNumberOfPeers(int num_peers);
    void setPeerInfo(std::vector<peer_info> info);
	void EngineerIFAThread(std::unique_ptr<ServerSocket> socket, int id);
	void PFAThread(int id);
};

#endif // end of #ifndef __SERVERTHREAD_H__

