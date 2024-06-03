#ifndef APP
#define APP

#include "utility.h"
#include "auth.h"
#include "role.h"
#include "credentials.h"
#include "database.h"

void print_main_menu();
void print_body();

extern UserInfo current_user; //store the key of the current user when the user login succesfully
extern int current_status_code;

#endif