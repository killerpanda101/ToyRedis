#include "ServerStub.h"

#include <utility>


ServerStub::ServerStub() {

}


//----------------Methods used by server to handle requests------------

void ServerStub::Init(std::unique_ptr<ServerSocket> socket) {
	this->socket = std::move(socket);
}

CustomerRequest ServerStub::ReceiveEngineerRequest(){
	char buffer[32];
	CustomerRequest request;
	if (socket->Recv(buffer, request.Size(), 0)) {
        request.Unmarshal(buffer);
	}
	return request;
}

int ServerStub::ReceiveIdentifier() {
    char buffer[32];
    IdentificationMessage request;
    if (socket->Recv(buffer, request.Size(), 0)) {
        request.Unmarshal(buffer);
    }
    return request.GetIdentifier();
}

ReplicationRequest ServerStub::ReceiveReplicationRequest() {
    char buffer[32];
    ReplicationRequest request;
    if(socket->Recv(buffer, request.Size(), 0)){
        request.Unmarshal(buffer);
    }
    return request;
}

int ServerStub::SendLaptop(LaptopInfo info) {
	char buffer[32];
	info.Marshal(buffer);
	return socket->Send(buffer, info.Size(), 0);
}

int ServerStub::SendRecord(CustomerRecord info) {
    char buffer[32];
    info.Marshal(buffer);
    return socket->Send(buffer, info.Size(), 0);
}

int ServerStub::SendReplicationResponse(int i) {
    ReplicationResponse info;
    info.setInfo(i);
    char buffer[32];
    info.Marshal(buffer);
    return socket->Send(buffer, info.Size(), 0);
}

//----------------Methods used by server to communicate with peers------------

int ServerStub::Init(int id, std::string ip, int port) {
    peer_id = id;
    return send_socket.Init(std::move(ip), port);
}

int ServerStub::GetPeerID() const {
    return peer_id;
}

bool ServerStub::SendIdentifier(int i) {
    if(i != 2){
        return false;
    }
    char buffer[32];
    IdentificationMessage request;
    request.SetIdentifier(i);
    request.Marshal(buffer);
    if (send_socket.Send(buffer, request.Size(), 0)) {
        return true;
    }
    return false;
}

int ServerStub::SendReplicationRequest(ReplicationRequest info) {
    char buffer[32];
    info.Marshal(buffer);
    return send_socket.Send(buffer, info.Size(), 0);
}

ReplicationResponse ServerStub::ReceiveReplicationResponse() {
    char buffer[32];
    ReplicationResponse request;
    if (send_socket.Recv(buffer, request.Size(), 0)) {
        request.Unmarshal(buffer);
    }
    return request;
}








