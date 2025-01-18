#include <string>
#include <vector>
#include <mutex>

struct ClientInfo {
    int socket_fd;
    std::string ip_address;
    int port;
    std::string username;
};


std::vector<ClientInfo> connected_clients;
std::mutex clients_mutex;
void addClient(int socket_fd, const std::string& ip_address, int port, const std::string& username) {
    std::lock_guard<std::mutex> guard(clients_mutex);
    connected_clients.push_back({socket_fd, ip_address, port, username});
}
void removeClient(int socket_fd) {
    std::lock_guard<std::mutex> guard(clients_mutex);
    connected_clients.erase(
        std::remove_if(connected_clients.begin(), connected_clients.end(),
                       [socket_fd](const ClientInfo& client) { return client.socket_fd == socket_fd; }),
        connected_clients.end());
}
 ClientInfo* findClientByUsername(const std::string& username) 
           {
           std::lock_guard<std::mutex> guard(clients_mutex);
           for (auto& client : connected_clients) 
           {
             if (client.username == username) 
               {
                return &client;
               }
            }
              return nullptr;
            }
