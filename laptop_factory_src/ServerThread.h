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

struct MapOp{
    int opcode; // operation code: 1 - update value
    int arg1; // customer_id to apply the operation
    int arg2; // parameter for the operation
};

struct AdminRequest {
	MapOp operation;
	std::promise<int> prom;
};

class LaptopFactory {
private:
	std::queue<std::unique_ptr<AdminRequest>> erq;
	std::mutex erq_lock;
	std::condition_variable erq_cv;

    std::mutex records_lock;

    std::map<int, int> customer_record;

	LaptopInfo CreateRegularLaptop(CustomerRequest request, int engineer_id);
    CustomerRecord ReadCustomerRecord(int customer_id);
public:
	void EngineerThread(std::unique_ptr<ServerSocket> socket, int id);
	void AdminThread(int id);
};

#endif // end of #ifndef __SERVERTHREAD_H__

