#ifndef DATABASE
#define DATABASE

#include <map>
#include "task.h"
#include "utility.h"
#include "role.h"
#include "credentials.h"

using namespace std;
using std::string;

extern std::map<string,UserInfo> userDB;

std::map<string,UserInfo> load_userDB();
void write_to_userDB(const string& email, const UserInfo& data);

#endif