#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "ServerSocket.h"
#include "ServerThread.h"

// process the command line arguments.
server_info check_args(int argc, char *argv[]){
    // we can have zero peers.
    if (argc < 4+(atoi(argv[3])*3)) {
        std::cout << "not enough arguments" << std::endl;
        std::cout << argv[0] << "[port #] [unique ID] [# peers] (repeat [ID] [IP] [port #])" << std::endl;
    }
    struct server_info server_details;
    server_details.port = atoi(argv[1]);
    server_details.factory_id = atoi(argv[2]);
    server_details.number_of_peers = atoi(argv[3]);

    // adding peer details
    for(int i; i<server_details.number_of_peers; i++){
        struct peer_info peer_details;
        int idx = i*3+4;
        peer_details.id = atoi(argv[idx]);
        peer_details.ip = argv[idx+1];
        peer_details.port = atoi(argv[idx+2]);
        server_details.peer_info_vector.push_back(peer_details);
    }

    return server_details;
}

int main(int argc, char *argv[]) {
	ServerSocket socket;
	LaptopFactory factory;
	std::unique_ptr<ServerSocket> new_socket;
	std::vector<std::thread> thread_vector;

    int engineer_cnt = 0;

    // Send the laptop factory information about its peers
    struct server_info server_details = check_args(argc, argv);
    factory.setFactoryID(server_details.factory_id);
    factory.setPeerInfo(server_details.peer_info_vector);
    factory.setNumberOfPeers(server_details.number_of_peers);

    // just one PFA per factory
	for (int i = 0; i < 1; i++) {
		std::thread admin_thread(&LaptopFactory::PFAThread,
				&factory, server_details.factory_id);
		thread_vector.push_back(std::move(admin_thread));
	}
    std::cout << thread_vector.size()<<std::endl;

	if (!socket.Init(server_details.port)) {
		std::cout << "Socket initialization failed" << std::endl;
		return 0;
	}

	while ((new_socket = socket.Accept())) {
        // Engineer ID's will not be liner anymore.
		std::thread request_thread(&LaptopFactory::EngineerIFAThread,
				&factory, std::move(new_socket), 
				engineer_cnt++);
		thread_vector.push_back(std::move(request_thread));
	}

	return 0;
}


