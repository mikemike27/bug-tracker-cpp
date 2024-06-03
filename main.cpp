#include "app.h"
#include "splashkit.h"


int main(){

    //load userDB from the csv file
    userDB = load_userDB();

    print_main_menu();

    return 0;
    
}