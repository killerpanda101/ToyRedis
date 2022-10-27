#include <cstring>
#include <iostream>

#include <arpa/inet.h>
#include "Messages.h"

// ----------------------------Customer Request Class functions--------------------------

CustomerRequest::CustomerRequest() {
	customer_id = -1;
	order_number = -1;
	request_type = -1;
}

void CustomerRequest::SetRequest(int cid, int order_num, int type) {
	customer_id = cid;
	order_number = order_num;
	request_type = type;
}

int CustomerRequest::GetCustomerId() { return customer_id; }
int CustomerRequest::GetOrderNumber() { return order_number; }
int CustomerRequest::GetRequestType() { return request_type; }

int CustomerRequest::Size() {
	return sizeof(customer_id) + sizeof(order_number) + sizeof(request_type);
}

void CustomerRequest::Marshal(char *buffer) {
	int net_customer_id = htonl(customer_id);
	int net_order_number = htonl(order_number);
	int net_request_type = htonl(request_type);
	int offset = 0;
	memcpy(buffer + offset, &net_customer_id, sizeof(net_customer_id));
	offset += sizeof(net_customer_id);
	memcpy(buffer + offset, &net_order_number, sizeof(net_order_number));
	offset += sizeof(net_order_number);
	memcpy(buffer + offset, &net_request_type, sizeof(net_request_type));
}

void CustomerRequest::Unmarshal(char *buffer) {
	int net_customer_id;
	int net_order_number;
	int net_request_type;
	int offset = 0;
	memcpy(&net_customer_id, buffer + offset, sizeof(net_customer_id));
	offset += sizeof(net_customer_id);
	memcpy(&net_order_number, buffer + offset, sizeof(net_order_number));
	offset += sizeof(net_order_number);
	memcpy(&net_request_type, buffer + offset, sizeof(net_request_type));

	customer_id = ntohl(net_customer_id);
	order_number = ntohl(net_order_number);
	request_type = ntohl(net_request_type);
}

bool CustomerRequest::IsValid() {
	return (customer_id != -1);
}

void CustomerRequest::Print() {
	std::cout << "id " << customer_id << " ";
	std::cout << "num " << order_number << " ";
	std::cout << "type " << request_type << std::endl;
}

// ----------------------------Laptop Info Class functions--------------------------

LaptopInfo::LaptopInfo() {
	customer_id = -1;
	order_number = -1;
	request_type = -1;
	engineer_id = -1;
    admin_id = -1;
}

void LaptopInfo::SetInfo(int id, int number, int type, int engid, int expid) {
	customer_id = id;
	order_number = number;
	request_type = type;
	engineer_id = engid;
    admin_id = expid;
}

void LaptopInfo::CopyRequest(CustomerRequest request) {
	customer_id = request.GetCustomerId();
	order_number = request.GetOrderNumber();
	request_type = request.GetRequestType();
}
void LaptopInfo::SetEngineerId(int id) { engineer_id = id; }
void LaptopInfo::SetAdminId(int id) { admin_id = id; }

int LaptopInfo::GetCustomerId() { return customer_id; }
int LaptopInfo::GetOrderNumber() { return order_number; }
int LaptopInfo::GetRequestType() { return request_type; }
int LaptopInfo::GetEngineerId() { return engineer_id; }
int LaptopInfo::GetAdminId() { return admin_id; }

int LaptopInfo::Size() {
	return sizeof(customer_id) + sizeof(order_number) + sizeof(request_type)
		+ sizeof(engineer_id) + sizeof(admin_id);
}

void LaptopInfo::Marshal(char *buffer) {
	int net_customer_id = htonl(customer_id);
	int net_order_number = htonl(order_number);
	int net_request_type = htonl(request_type);
	int net_engineer_id = htonl(engineer_id);
	int net_admin_id = htonl(admin_id);
	int offset = 0;

	memcpy(buffer + offset, &net_customer_id, sizeof(net_customer_id));
	offset += sizeof(net_customer_id);
	memcpy(buffer + offset, &net_order_number, sizeof(net_order_number));
	offset += sizeof(net_order_number);
	memcpy(buffer + offset, &net_request_type, sizeof(net_request_type));
	offset += sizeof(net_request_type);
	memcpy(buffer + offset, &net_engineer_id, sizeof(net_engineer_id));
	offset += sizeof(net_engineer_id);
	memcpy(buffer + offset, &net_admin_id, sizeof(net_admin_id));

}

void LaptopInfo::Unmarshal(char *buffer) {
	int net_customer_id;
	int net_order_number;
	int net_laptop_type;
	int net_engineer_id;
	int net_admin_id;
	int offset = 0;

	memcpy(&net_customer_id, buffer + offset, sizeof(net_customer_id));
	offset += sizeof(net_customer_id);
	memcpy(&net_order_number, buffer + offset, sizeof(net_order_number));
	offset += sizeof(net_order_number);
	memcpy(&net_laptop_type, buffer + offset, sizeof(net_laptop_type));
	offset += sizeof(net_laptop_type);
	memcpy(&net_engineer_id, buffer + offset, sizeof(net_engineer_id));
	offset += sizeof(net_engineer_id);
	memcpy(&net_admin_id, buffer + offset, sizeof(net_admin_id));

	customer_id = ntohl(net_customer_id);
	order_number = ntohl(net_order_number);
	request_type = ntohl(net_laptop_type);
	engineer_id = ntohl(net_engineer_id);
	admin_id = ntohl(net_admin_id);
}

bool LaptopInfo::IsValid() {
	return (customer_id != -1);
}

void LaptopInfo::Print() {
	std::cout << "id " << customer_id << " ";
	std::cout << "num " << order_number << " ";
	std::cout << "type " << request_type << " ";
	std::cout << "engid " << engineer_id << " ";
	std::cout << "adminid " << admin_id << std::endl;
}

// ----------------------------Customer Record Class functions--------------------------

CustomerRecord::CustomerRecord() {
    customer_id = -1;
    last_order_id = -1;
}

void CustomerRecord::SetInfo(int cid, int last_id) {
    customer_id = cid;
    last_order_id = last_id;
}

void CustomerRecord::CopyRecord(CustomerRecord info) {
    customer_id = info.GetCustomerId();
    last_order_id = info.GetLastOrderId();
}

void CustomerRecord::SetCustomerId(int id) {
    customer_id=id;
}

void CustomerRecord::SetLastOrderId(int id) {
    last_order_id=id;
}

int CustomerRecord::GetCustomerId() {
    return customer_id;
}

int CustomerRecord::GetLastOrderId() {
    return last_order_id;
}

int CustomerRecord::Size() {
    return sizeof(customer_id)+sizeof(last_order_id);
}

void CustomerRecord::Marshal(char *buffer) {
    int net_customer_id = htonl(customer_id);
    int net_last_order_id = htonl(last_order_id);
    int offset = 0;

    memcpy(buffer + offset, &net_customer_id, sizeof(net_customer_id));
    offset += sizeof(net_customer_id);
    memcpy(buffer + offset, &net_last_order_id, sizeof(net_last_order_id));
    offset += sizeof(net_last_order_id);
    memcpy(buffer + offset, &net_last_order_id, sizeof(net_last_order_id));
}

void CustomerRecord::Unmarshal(char *buffer) {
    int net_customer_id;
    int net_last_order_id;
    int offset = 0;

    memcpy(&net_customer_id, buffer + offset, sizeof(net_customer_id));
    offset += sizeof(net_customer_id);
    memcpy(&net_last_order_id, buffer + offset, sizeof(net_last_order_id));

    customer_id = ntohl(net_customer_id);
    last_order_id = ntohl(net_last_order_id);
}

bool CustomerRecord::IsValid() {
    return (customer_id!=-1);
}

void CustomerRecord::Print() {
    std::cout << "id " << customer_id << " ";
    std::cout << "last_order " << last_order_id << std::endl;
}

// ----------------------------Identifier Message Class functions--------------------------

IdentificationMessage::IdentificationMessage() {
    identifier = -1;
}

void IdentificationMessage::SetIdentifier(int val) {
    identifier = val;
}

int IdentificationMessage::GetIdentifier() {
    return identifier;
}

int IdentificationMessage::Size() {
    return sizeof(identifier);
}

void IdentificationMessage::Marshal(char *buffer) {
    int net_identifier = htonl(identifier);
    int offset = 0;

    memcpy(buffer + offset, &net_identifier, sizeof(net_identifier));
}

void IdentificationMessage::Unmarshal(char *buffer) {
    int net_identifier;
    int offset = 0;

    memcpy(&net_identifier, buffer + offset, sizeof(net_identifier));

    identifier = ntohl(net_identifier);
}

bool IdentificationMessage::IsValid() {
    if(identifier==1 || identifier==2){
        return true;
    }
    return false;
}

void IdentificationMessage::Print() {
    std::cout << "identifier: " << identifier << std::endl;
}

// ----------------------------Replication request Class functions--------------------------

ReplicationRequest::ReplicationRequest() {
    factoryId=-1;
    committed_index=-1;
    last_index=-1;
    opcode=-1;
    arg1=-1;
    arg2=-1;
}

void ReplicationRequest::setInfo(int fid, int committed_idx, int last_idx, MapOp op) {
    factoryId=fid;
    committed_index=committed_idx;
    last_index=last_idx;
    opcode=op.opcode;
    arg1=op.arg1;
    arg2=op.arg2;
}

int ReplicationRequest::getPrimaryId() {
    return factoryId;
}

int ReplicationRequest::getCommittedIndex() {
    return committed_index;
}

int ReplicationRequest::getLastIndex() {
    return last_index;
}

MapOp ReplicationRequest::getMapOP() {
    MapOp res{};
    res.opcode = opcode;
    res.arg1 = arg1;
    res.arg2 = arg2;
    return res;
}

int ReplicationRequest::Size(){
    return sizeof(factoryId)+sizeof(committed_index)
           +sizeof(last_index)+sizeof(opcode)+sizeof(arg1)+sizeof(arg2);
}


void ReplicationRequest::Marshal(char *buffer) {

    int net_factoryID= htonl(factoryId);
    int net_committedIDX = htonl(committed_index);
    int net_lastIDX = htonl(last_index);
    int net_OpCode = htonl(opcode);
    int net_arg1 = htonl(arg1);
    int net_arg2 = htonl(arg2);
    int offset = 0;

    memcpy(buffer + offset, &net_factoryID, sizeof(net_factoryID));
    offset += sizeof(net_factoryID);
    memcpy(buffer + offset, &net_committedIDX, sizeof(net_committedIDX));
    offset += sizeof(net_committedIDX);
    memcpy(buffer + offset, &net_lastIDX, sizeof(net_lastIDX));
    offset += sizeof(net_lastIDX);
    memcpy(buffer + offset, &net_OpCode, sizeof(net_OpCode));
    offset += sizeof(net_OpCode);
    memcpy(buffer + offset, &net_arg1, sizeof(net_arg1));
    offset += sizeof(net_arg1);
    memcpy(buffer + offset, &net_arg2, sizeof(net_arg2));
}

void ReplicationRequest::Unmarshal(char *buffer) {
    int net_factoryID;
    int net_committedIDX;
    int net_lastIDX;
    int net_OpCode;
    int net_arg1;
    int net_arg2;
    int offset = 0;

    memcpy(&net_factoryID, buffer + offset, sizeof(net_factoryID));
    offset += sizeof(net_factoryID);
    memcpy(&net_committedIDX, buffer + offset, sizeof(net_committedIDX));
    offset += sizeof(net_committedIDX);
    memcpy(&net_lastIDX, buffer + offset, sizeof(net_lastIDX));
    offset += sizeof(net_lastIDX);
    memcpy(&net_OpCode, buffer + offset, sizeof(net_OpCode));
    offset += sizeof(net_OpCode);
    memcpy(&net_arg1, buffer + offset, sizeof(net_arg1));
    offset += sizeof(net_arg1);
    memcpy(&net_arg2, buffer + offset, sizeof(net_arg2));

    factoryId=ntohl(net_factoryID);
    committed_index=ntohl(net_committedIDX);
    last_index=ntohl(net_lastIDX);
    opcode=ntohl(net_OpCode);
    arg1=ntohl(net_arg1);
    arg2=ntohl(net_arg2);
}

bool ReplicationRequest::IsValid() {
    return (factoryId!=-1);
}

void ReplicationRequest::Print() {
    std::cout << "FactorID " << factoryId;
    std::cout << "CommittedIDX " << committed_index;
    std::cout << "LastIDX " << last_index;
    std::cout << "OpCode " << opcode;
    std::cout << "Arg1 " << arg1;
    std::cout << "Arg2 " << arg2 << std::endl;

}

// ----------------------------Replication Response Class functions--------------------------

ReplicationResponse::ReplicationResponse() {
    status = -1;
}

void ReplicationResponse::setInfo(int num) {
    status = num;
}

int ReplicationResponse::getStatus() {
    return status;
}

int ReplicationResponse::Size() {
    return sizeof(status);
}

void ReplicationResponse::Marshal(char *buffer) {
    int net_status = htonl(status);
    int offset = 0;

    memcpy(buffer + offset, &net_status, sizeof(net_status));
}

void ReplicationResponse::Unmarshal(char *buffer) {
    int net_status;
    int offset = 0;

    memcpy(&net_status, buffer + offset, sizeof(net_status));

    status = ntohl(net_status);
}

bool ReplicationResponse::IsValid() {
    return (status==1);
}

void ReplicationResponse::Print() {
    std::cout << "Status: " << std::endl;
}
