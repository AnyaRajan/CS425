#include"request.h"

void HttpRequest::parseRequest(const std::string& rawRequest){
    //Todo 
    //raw request se Get method se path nikalkr map me daalna hai.
    //uss path me rakhi file se html read krna hai.(hope i will make a new method for this.)
    //ab html file se content read krne ke baad string stream me store krna hai.
    //aur sara content ko http response ke format me frame krke browser ko bhejna hai.
    //HAHAHAHA HAHAHAHAHAH
    int currindex=0;
    while(currindex < rawRequest.length()){
        if(rawRequest[currindex] == ' '){
            break;
        }
        method += rawRequest[currindex];
        currindex++;
    }
    // std::cout<<method<<std::endl;
    headers["method"] = method;

    currindex++;
    while(currindex < rawRequest.length()){
        if(rawRequest[currindex] == ' '){
            break;
        }
        path += rawRequest[currindex];
        currindex++;
    }
   
    headers["path"] = path;
    


    //mujhe ab method mil gya hai and path mil gya hai ab mai path me se file ka name nikaalunga aur 
    //html file ko open karke uska content read karunga.
    //iske liye mai ek methon banaunga readhtmlFile().

    

 


}
std::string HttpRequest::readHtmlFile(const std::string &path){
    //TODO
    //ab html file me se content read krna hai.
    //iske liye hume fstream use krna padega.
    std::string filename = path.substr(1,path.length());
   
    std::ifstream file("../"+filename); //ifstream used for reading file if exists.
    int flag=0;
    if(!file){
        flag=1;
        std::cerr<<"File not found."<<std::endl;
      
        
    }

    // std::cout<<"file found"<<std::endl;
    if(flag==0){
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }


    return "";
}
std::string HttpRequest::getMimeType(const std::string &path){
    std::map<std::string, std::string> mimeTypes = {
        {"html","text/html"},
        {"css","text/css"},
        {"js","text/javascript"},
        {"jpg","image/jpeg"},
        {"png","image/png"}
    };
    std::string fileExtension = path.substr(path.find_last_of(".") + 1);
    // std::cout<<fileExtension<<std::endl;
    return mimeTypes[fileExtension];
}