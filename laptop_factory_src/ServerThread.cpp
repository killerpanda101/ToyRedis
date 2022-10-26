#include <iostream>
#include <memory>

#include "ServerThread.h"
#include "ServerStub.h"

//-------------------------Init/Setup-----------------------------
void LaptopFactory::setFactoryID(int id) {
    factory_id = id;
}

void LaptopFactory::setPeerInfo(std::vector<peer_info> info) {
    peer_info_vector = info;
}

void LaptopFactory::setNumberOfPeers(int num_peers) {
    number_of_peers = num_peers;
}

//-------------------------EngineerPFA and Server Threads-----------------------------

void LaptopFactory::EngineerIFAThread(std::unique_ptr<ServerSocket> socket, int id) {
    ServerStub stub;
    stub.Init(std::move(socket));
    int role;
    role = stub.ReceiveIdentifier();

    // request for engineer
    if(role==1)
    {
        int engineer_id = id;
        int engineer_request_type;
        CustomerRequest customer_req;
        LaptopInfo laptop;
        CustomerRecord record;

        while (true) {
            customer_req = stub.ReceiveEngineerRequest();
            if (!customer_req.IsValid()) {break;}
            engineer_request_type = customer_req.GetRequestType();
            switch (engineer_request_type) {
                case 1:
                    laptop = CreateRegularLaptop(customer_req, engineer_id);
                    stub.SendLaptop(laptop);
                    break;
                case 2:
                    record = ReadCustomerRecord(customer_req.GetCustomerId());
                    stub.SendRecord(record);
                    break;
                default:
                    std::cout << "Undefined request type: "<< engineer_request_type << std::endl;
            }
        }
    }

    // request for IFA
    else{
        ReplicationRequest replication_req;
        replication_req = stub.ReceiveReplicationRequest();
        primary_id = replication_req.getPrimaryId();

        // update primary
        int primary_lastIDX = replication_req.getPrimaryId();

        // append operation to smr_log
        AddOppToLog(replication_req.getMapOP(),replication_req.getLastIndex());

        // apply committed operation
        ApplyOpp(replication_req.getCommittedIndex());

        // respond back to primary
        stub.SendReplicationResponse(1);
    }
}

void LaptopFactory::PFAThread(int id) {
    std::unique_lock<std::mutex> ul_erq(erq_lock, std::defer_lock);

    ServerStub peer_stubs[number_of_peers];
    ReplicationRequest request;
    ReplicationResponse response;

    while (true) {
        // get operation from queue
        ul_erq.lock();

        if (erq.empty()) {
            erq_cv.wait(ul_erq, [this]{ return !erq.empty(); });
        }

        auto req = std::move(erq.front());
        erq.pop();

        ul_erq.unlock();

        // primary changes
        if(primary_id!=factory_id){
            if(committed_index!=-1){
                ApplyOpp(last_index);
            }
            primary_id = factory_id;
        }

        // append request to log
        AddOppToLog(req->operation, last_index+1);


        // send replication request to peers one by one
        for(int i=0; i<number_of_peers; i++){

            // establish connection, if already connected does not modify it.
            peer_stubs[i].Init(peer_info_vector[i].ip,
                               peer_info_vector[i].port, peer_info_vector[i].id);

            // Send identifier stating that the request if from PFA
            peer_stubs[i].SendIdentifier(2);

            // send replication request
            request.setInfo(factory_id, committed_index,
                            last_index, req->operation);
            peer_stubs[i].SendReplicationRequest(request);

            // receive response
            response = peer_stubs[i].ReceiveReplicationResponse();
        }

        // update map
        ApplyOpp(last_index);

        // send admin id back
        req->prom.set_value(id);
    }
}

//----------------------Engineer helper methods----------------------

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


//----------------------Helpers to add to smr_log and apply to map------------------------

void LaptopFactory::AddOppToLog(MapOp op, int idx) {
    smr_log[idx] = op;
    last_index = idx;
}

void LaptopFactory::ApplyOpp(int idx) {
    std::unique_lock<std::mutex> ul_records(records_lock, std::defer_lock);
    MapOp opp = smr_log[idx];
    ul_records.lock();
    customer_record[opp.arg1] = opp.arg2;
    ul_records.unlock();
    committed_index = idx;
}






