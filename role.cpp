#include "role.h"

//show role status based on employee code, only after login
string print_status(){

    //we store it in a variable instead of using it directly from the struct,
    //cuz later admin/manager can switch to different role, eg debugging purpose

    switch(current_status_code){
        case 0:
            return "Employee";
        case 1:
            return "Admin";
        case 2:
            return "Manager";
        default:
            return "";
    }

}

//for debugging purpose, eg manager to admin/ admin to normal user
void change_status(int targetCode){

    current_status_code = targetCode;

    print_body();

}