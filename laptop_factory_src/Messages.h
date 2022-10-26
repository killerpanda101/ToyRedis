#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include <string>
#include <vector>

struct MapOp{
    int opcode; // operation code: 1 - update value
    int arg1; // customer_id to apply the operation
    int arg2; // parameter for the operation
};

struct peer_info{
    int id;
    std::string ip;
    int port;
};

struct server_info{
    int factory_id;
    int port;
    int number_of_peers;
    std::vector<peer_info> peer_info_vector;
};



class CustomerRequest {
private:
	int customer_id;
	int order_number;
	int request_type;

public:
    CustomerRequest();
	void operator = (const CustomerRequest &request) {
		customer_id = request.customer_id;
		order_number = request.order_number;
		request_type = request.request_type;
	}
	void SetRequest(int cid, int order_num, int type);
	int GetCustomerId();
	int GetOrderNumber();
	int GetRequestType();

	int Size();
    bool IsValid();
    void Print();

	void Marshal(char *buffer);
	void Unmarshal(char *buffer);


};



class LaptopInfo {
private:
	int customer_id;
	int order_number;
	int request_type;
	int engineer_id;
	int admin_id;

public:
	LaptopInfo();
	void operator = (const LaptopInfo &info) {
		customer_id = info.customer_id;
		order_number = info.order_number;
		request_type = info.request_type;
		engineer_id = info.engineer_id;
		admin_id = info.admin_id;
	}
	void SetInfo(int cid, int order_num, int type, int engid, int adminid);
	void CopyRequest(CustomerRequest order);
	void SetEngineerId(int id);
	void SetAdminId(int id);

	int GetCustomerId();
	int GetOrderNumber();
	int GetRequestType();
	int GetEngineerId();
	int GetAdminId();

	void Marshal(char *buffer);
	void Unmarshal(char *buffer);

	bool IsValid();
    int Size();
	void Print();
};



class CustomerRecord{
private:
    int customer_id;
    int last_order_id;

public:
    CustomerRecord();
    void operator = (const CustomerRecord &info){
        customer_id = info.customer_id;
        last_order_id = info.last_order_id;
    }
    void SetInfo(int cid, int last_id);
    void CopyRecord(CustomerRecord info);
    void SetCustomerId(int id);
    void SetLastOrderId(int id);

    int GetCustomerId();
    int GetLastOrderId();

    void Marshal(char *buffer);
    void Unmarshal(char *buffer);

    bool IsValid();
    int Size();
    void Print();
};



class IdentificationMessage {
private:
    // 1 - customer request
    // 2 - PFA request
    int identifier;

public:
    IdentificationMessage();
    void operator = (const IdentificationMessage &request) {
        identifier = request.identifier;
    }
    void SetIdentifier(int val);
    int GetIdentifier();

    void Marshal(char *buffer);
    void Unmarshal(char *buffer);

    bool IsValid();
    int Size();
    void Print();
};



class ReplicationRequest {
private:
    int factoryId;
    int committed_index;
    int last_index;
    int opcode;
    int arg1;
    int arg2;

public:
    ReplicationRequest();
    void operator = (const ReplicationRequest &request) {
        factoryId = request.factoryId;
        committed_index = request.committed_index;
        last_index = request.last_index;
        opcode = request.opcode;
        arg1 = request.arg1;
        arg2 = request.arg2;
    }

    void setInfo(int fid, int committed_idx, int last_idx, MapOp op);

    int getPrimaryId();
    int getCommittedIndex();
    int getLastIndex();
    MapOp getMapOP();

    void Marshal(char *buffer);
    void Unmarshal(char *buffer);

    bool IsValid();
    int Size();
    void Print();
};



class ReplicationResponse {
private:
    // -1 by default!
    // 1 indicates success :-)
    int status;

public:
    ReplicationResponse();

    void operator=(const ReplicationResponse &request) {
        status = request.status;
    }

    void setInfo(int num);
    int getStatus();

    void Marshal(char *buffer);
    void Unmarshal(char *buffer);

    bool IsValid();
    int Size();
    void Print();
};


#endif // #ifndef __MESSAGES_H__
