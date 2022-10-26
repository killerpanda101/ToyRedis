#include "ClientThread.h"
#include "Messages.h"

#include <iostream>
#include <utility>

ClientThreadClass::ClientThreadClass() = default;

void ClientThreadClass::
ThreadBody(std::string ip, int port, int id, int orders, int type) {
	customer_id = id;
	num_orders = orders;
	request_type = type;
	if (!stub.Init(std::move(ip), port)) {
		std::cout << "Thread " << customer_id << " failed to connect" << std::endl;
		return;
	}
    if(!stub.SendIdentifier(1)){
        std::cout << "Thread " << customer_id << " failed to send customer identifier" << std::endl;
        return;
    }
	for (int i = 0; i < num_orders; i++) {
        CustomerRequest request;
        request.SetRequest(customer_id, i, request_type);

		timer.Start();

        // Request Type 1
        if(request_type==1){
            LaptopInfo laptop;
            laptop = stub.OrderLaptop(request);
            if (!laptop.IsValid()) {
                std::cout << "Invalid laptop " << customer_id << std::endl;
                return;
            }
        }

        // Request Type 2
        else if(request_type==2){
            CustomerRecord record;
            record = stub.ReadRecord(request);
            if(!record.IsValid()){
                std::cout << "Invalid record " << customer_id << std::endl;
                break;
            }
        }

        // Request Type 3
        else if(request_type==3){
            CustomerRecord record;
            request.SetRequest(i,-1, 2);
            record = stub.ReadRecord(request);
            if(record.GetCustomerId()!=-1){
                std::cout << record.GetCustomerId() << "\t" << record.GetLastOrderId() << std::endl;
            }
        }


        // Invalid request type
        else{
            std::cout << "Invalid Customer Request Type" << std::endl;
        }

		timer.EndAndMerge();


	}
}

ClientTimer ClientThreadClass::GetTimer() {
	return timer;	
}

