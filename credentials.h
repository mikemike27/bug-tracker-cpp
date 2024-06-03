#ifndef CREDENTIALS
#define CREDENTIALS

struct UserInfo{

    string userName;
    string password;
    int statusCode; //0-normal user, 100-admin, 200-manager

};

const string FILENAME = "UserDB.csv";

#endif