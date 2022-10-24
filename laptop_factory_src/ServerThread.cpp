#include <iostream>
#include <memory>

#include "ServerThread.h"
#include "ServerStub.h"

LaptopInfo LaptopFactory::
CreateRegularLaptop(CustomerRequest request, int engineer_id) {
	LaptopInfo laptop;
	laptop.CopyRequest(request);
	laptop.SetEngineerId(engineer_id);

    // admin handoff
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    std::unique_ptr<AdminRequest> req =
            std::unique_ptr<AdminRequest>(new AdminRequest);
    MapOp opp = *new MapOp();
    opp.opcode=1;
    opp.arg1 = laptop.GetCustomerId();
    opp.arg2 = laptop.GetOrderNumber();
    req->operation = opp;
    req->prom = std::move(prom);

    // push to queue
    erq_lock.lock();
    erq.push(std::move(req));
    erq_cv.notify_one();
    erq_lock.unlock();

    // set admin id and return
    int admin_id = fut.get();
    laptop.SetAdminId(admin_id);

    return laptop;
}


void LaptopFactory::
EngineerThread(std::unique_ptr<ServerSocket> socket, int id) {
	int engineer_id = id;
	int request_type;
	CustomerRequest request;
	LaptopInfo laptop;
    CustomerRecord record;

	ServerStub stub;

	stub.Init(std::move(socket));

	while (true) {
		request = stub.ReceiveRequest();
		if (!request.IsValid()) {
			break;	
		}
		request_type = request.GetRequestType();
		switch (request_type) {
			case 1:
				laptop = CreateRegularLaptop(request, engineer_id);
                stub.SendLaptop(laptop);
				break;
			case 2:
				record = ReadCustomerRecord(request.GetCustomerId());
                stub.SendRecord(record);
				break;
			default:
				std::cout << "Undefined request type: "
					<< request_type << std::endl;

		}

	}
}

void LaptopFactory::AdminThread(int id) {
	std::unique_lock<std::mutex> ul_erq(erq_lock, std::defer_lock);
    std::unique_lock<std::mutex> ul_records(records_lock, std::defer_lock);
    std::vector<MapOp> smr_log;
	while (true) {
        // get operation from queue
		ul_erq.lock();

		if (erq.empty()) {
			erq_cv.wait(ul_erq, [this]{ return !erq.empty(); });
		}

		auto req = std::move(erq.front());
		erq.pop();

		ul_erq.unlock();

        // process operation
        MapOp opp = req->operation;
        smr_log.push_back(opp);

        // update map
        ul_records.lock();
        customer_record[opp.arg1] = opp.arg2;
        ul_records.unlock();

        // send admin id back
		req->prom.set_value(id);
	}
}

CustomerRecord LaptopFactory::ReadCustomerRecord(int customer_id) {
    CustomerRecord result = *new CustomerRecord();
    std::unique_lock<std::mutex> ul_records(records_lock, std::defer_lock);

    // find customer in customer_record map.
    ul_records.lock();
    auto search = customer_record.find(customer_id);
    if(search!=customer_record.end()){
        result.SetCustomerId(customer_id);
        result.SetLastOrderId(search->second);
    }
//    for(const auto& elem : customer_record)
//    {
//        std::cout << elem.first << " " << elem.second << "\n";
//    }
    ul_records.unlock();

    return result;
}



