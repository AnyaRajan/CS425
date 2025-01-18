#include"server.cpp"
#include <string.h>
#include"request.cpp"
#include"response.cpp"
#include<thread>
#include"client.cpp"
#include"load_users.cpp"
void handleClient(int client_socket_fd, const std::unordered_map<std::string, std::string>& users) {
    char buffer[1024];
    std::string username, password;

    // Step 1: Prompt the client for a username
    std::string username_prompt = "Enter your username: ";
    send(client_socket_fd, username_prompt.c_str(), username_prompt.length(), 0);

    ssize_t bytes_received = recv(client_socket_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
        std::cerr << "Failed to receive username from client." << std::endl;
        close(client_socket_fd);
        return;
    }
    buffer[bytes_received] = '\0'; // Null-terminate the received data
    username = std::string(buffer);

    // Step 2: Prompt the client for a password
    std::string password_prompt = "Enter your password: ";
    send(client_socket_fd, password_prompt.c_str(), password_prompt.length(), 0);

    bytes_received = recv(client_socket_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
        std::cerr << "Failed to receive password from client." << std::endl;
        close(client_socket_fd);
        return;
    }
    buffer[bytes_received] = '\0'; // Null-terminate the received data
    password = std::string(buffer);

    // Step 3: Authenticate the user
    if (users.find(username) == users.end() || users[username] != password) {
        std::string auth_failed = "Authentication failed. Disconnecting.\n";
        send(client_socket_fd, auth_failed.c_str(), auth_failed.length(), 0);
        std::cerr << "Authentication failed for client: " << username << std::endl;
        close(client_socket_fd);
        return;
    }

    // Step 4: Proceed with the authenticated client
    std::cout << "Client authenticated: " << username << std::endl;

    // Add the client to the list
    sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    getpeername(client_socket_fd, (struct sockaddr*)&client_addr, &addr_len);
    addClient(client_socket_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), username);

    // Step 5: Handle the client request
    read(client_socket_fd, buffer, sizeof(buffer) - 1);
    HttpRequest req = HttpRequest();
    req.parseRequest(buffer);

    std::string mimetype = req.getMimeType(req.path);
    HttpResponse res = HttpResponse();
    std::string body = req.readHtmlFile(req.path);
    std::string response = res.frameHttpResponse("200", "OK", req.headers, body, mimetype);

    write(client_socket_fd, response.c_str(), response.length());

    // Step 6: Clean up
    removeClient(client_socket_fd);
    close(client_socket_fd);
}

int main(int argc, char* argv[]){
    Server server = Server(argv[1]);
    struct sockaddr_in client_addr;
    socklen_t client_addr_size;
    int s_fd;
    s_fd = server.start();
    client_addr_size = sizeof(struct sockaddr_in); 
  
    std::unordered_map<std::string, std::string> users = loadUsers("users.txt");



    while(1){
        //handle client requests.
        //Accept system call .
      
        int client_socket_fd = accept(s_fd, (struct sockaddr*)&client_addr, &client_addr_size);
        if(client_socket_fd < 0){
            std::cerr << "Failed to accept client request." << std::endl;
            exit(1);
        }
       
        // Create a new thread to handle the client
        std::thread clientThread(handleClient, client_socket_fd);
        clientThread.detach(); // Detach the thread to allow it to run independently
       
      


    }


    return 0;
}