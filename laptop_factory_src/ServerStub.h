#ifndef __SERVER_STUB_H__
#define __SERVER_STUB_H__

#include <memory>

#include "ServerSocket.h"
#include "Messages.h"

class ServerStub {
private:
	std::unique_ptr<ServerSocket> socket;
    ServerSocket send_socket;
    int peer_id;
public:
	ServerStub();

    // methods used by the EngineerIFA thread
	void Init(std::unique_ptr<ServerSocket> socket);
	CustomerRequest ReceiveEngineerRequest();
    ReplicationRequest ReceiveReplicationRequest();
	int SendLaptop(LaptopInfo info);
    int SendRecord(CustomerRecord info);
    int SendReplicationResponse(int i);
    int ReceiveIdentifier();

    // methods used by the PFA thread
    int Init(std::string ip, int port, int peer_id);
    int GetPeerID();
    int SendReplicationRequest(ReplicationRequest info);
    bool SendIdentifier(int i);
    ReplicationResponse ReceiveReplicationResponse();
};

#endif // end of #ifndef __SERVER_STUB_H__