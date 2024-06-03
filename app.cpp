#include "app.h"

UserInfo current_user; //store the key of the current user when the user login succesfully
int current_status_code;

//show user the main menu options
void print_main_menu(){

    create_line_break();
    write_line("Bug Tracking System Version 1.0.0");
    create_line_break();

    write_line("(1) Login");
    write_line("(2) Sign up");
    write_line("(3) Exit");
    write("PLease select your option: ");

    string input = read_input_range(1, 3);

    switch(std::stoi(input)){
        case 1:
            login();
            break;
        case 2:
            signup();
            break;
        case 3:
            write_line("Have a nice day");
            return;
    }

}

//after user login
void print_body(){

    create_line_break();

    write_line("Welcome " + current_user.userName + ", Status: " + print_status());
    write_line("Activites:");
    write_line("(1) Create ticket");
    write_line("(2) Work In Progress");
    write_line("(3) Completed");
    write_line("(4) Logout");

    int end = 4;

    //for admin only, for debugging purpose
    if(current_user.statusCode == 100){
        write_line("(5) Login as Employee");
        write_line("(6) Login as Admin");
        end = 6;
    }

    //for manager only, for debugging purpose
    if(current_user.statusCode == 200){
        write_line("(5) Login as Employee");
        write_line("(6) Login as Admin");
        write_line("(7) Login as Manager");
        end = 7;
    }

    write("Please select an option: ");

    string input = read_input_range(1, end);

    switch(std::stoi(input)){
        case 1:
            //create ticket
            create_ticket();
            break;
        case 2:
            //show wip ticket
            show_task(true);
            break;
        case 3:
            //show completed task/ticket
            show_task(false);
            break;
        case 4:
            //log off
            print_main_menu();
            break;
        case 5:
            if(end == 6 || end == 7){
                //login as employee, only for admin and manager
                change_status(0);
            }
            break;
        case 6:
            if(end == 6 || end == 7){
                //login as admin, only for admin and manager
                change_status(100);
            }
            break;
        case 7:
            if(end == 7){
                //login as manager, only for manager
                change_status(200);
            }
            break;
    }
    

}