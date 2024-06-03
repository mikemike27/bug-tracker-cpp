#include "database.h"

std::map<string,UserInfo> userDB; //user database, pair user email and userInfo struct

//load user data from csv file
std::map<string,UserInfo> load_userDB(){

    std::map<string,UserInfo> data;

    std::ifstream file(FILENAME);

    if(!file.is_open()){

        write_line("The csv file has failed to load.");

        return data;

    }

    string line;
    vector<string> columns;
    string email;
    UserInfo userInfo;

    while(getline(file, line)){
        
        //write_line(line);
        columns = split(line, ',');

        email = columns[0];
        userInfo.userName = columns[1];
        userInfo.password = columns[2];
        userInfo.statusCode = std::stoi(columns[3]);

        data[email] = userInfo;

    }

    file.close();

    return data;

}

//save userdata to userDB,eg sign up
void write_to_userDB(const string& email, const UserInfo& data){

    std::ofstream file(FILENAME, std::ios::app);

    if(!file.is_open()){

        write_line("Operation failed.");

        return;
    }

    //separate user data with comma
    file << std::endl;
    file << email;
    file << ",";
    file << data.userName;
    file << ",";
    file << data.password;
    file << ",";
    file << data.statusCode;

    file.close();


}