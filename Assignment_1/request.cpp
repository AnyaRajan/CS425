#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <map>
#include <sstream>
#include <iostream>
#include <fstream>
using namespace std;
std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    for (char ch : str) {
        if (ch == delimiter) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else {
            token += ch;
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }
    return tokens;
}
class HttpRequest {
public:
    std::string method;                          // Command or HTTP method
    std::string target;                            // Target (user, group, or file)
 string headers;

    void parseRequest(const std::string& rawRequest);
    std::string readHtmlFile(const std::string& path);
    std::string getMimeType(const std::string& path);
};

void HttpRequest::parseRequest(const std::string& rawRequest) 
{
    std::vector<std::string> parts = split(rawRequest, ' ');
    method = parts[0];

    std::cout << "Debug: Parsed Method: " << method << "\n";
if (method == "/msg" || method == "/group_msg") {
    target = parts[1];
    for(long unsigned i=2;i<parts.size();i++)
    {
        headers += parts[i];
        headers += " ";
    }

    } else if (method == "/broadcast") 
    {
        headers = parts[1];
       
    } else if (method == "/create_group" || method == "/join_group" || method == "/leave_group") 
    {
        target = parts[1];
    } else {
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
