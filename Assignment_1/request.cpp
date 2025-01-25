#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <map>
#include <sstream>
#include <iostream>
#include <fstream>

class HttpRequest {
public:
    std::string method;                          // Command or HTTP method
    std::string path;                            // Target (user, group, or file)
    std::map<std::string, std::string> headers; // Additional details (e.g., message, group name)

    void parseRequest(const std::string& rawRequest);
    std::string readHtmlFile(const std::string& path);
    std::string getMimeType(const std::string& path);
};

void HttpRequest::parseRequest(const std::string& rawRequest) {
    std::istringstream stream(rawRequest);
    std::string firstWord;

    // Extract the first word (command or method)
    stream >> firstWord;
    method = firstWord;

    if (method == "/msg" || method == "/group_msg") {
        // Parse private or group message
        std::string target, message;
        stream >> target;               // Username or group name
        std::getline(stream, message); // Message content
        headers["target"] = target;
        headers["message"] = message.substr(1); // Remove leading space
    } else if (method == "/broadcast") {
        // Parse broadcast message
        std::string message;
        std::getline(stream, message);
        headers["message"] = message.substr(1); // Remove leading space
    } else if (method == "/create_group" || method == "/join_group" || method == "/leave_group") {
        // Parse group-related commands
        std::string groupName;
        stream >> groupName;
        headers["group_name"] = groupName;
    } else {
        // Unknown command
        std::cerr << "Unknown command: " << method << std::endl;
    }
}

std::string HttpRequest::readHtmlFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "File not found: " << path << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string HttpRequest::getMimeType(const std::string& path) {
    std::map<std::string, std::string> mimeTypes = {
        {"html", "text/html"},
        {"css", "text/css"},
        {"js", "text/javascript"},
        {"jpg", "image/jpeg"},
        {"png", "image/png"}
    };
    std::string ext = path.substr(path.find_last_of(".") + 1);
    return mimeTypes.count(ext) ? mimeTypes[ext] : "text/plain";
}

#endif
