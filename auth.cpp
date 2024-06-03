#include "auth.h"

void login(){

    create_line_break();
    write_line("User Log in");
    create_line_break();

    //user login
    write("Please enter your email: ");
    string email = read_line();

    //search the database with the info
    auto it = userDB.find(email);
    if(it != userDB.end()){

        write("Please enter your password: ");
        string password = read_line();

        //found email, check password correct or not
        string pwd = it->second.password;

        if(password != pwd){

            write_line("Password incorrect.");
            print_main_menu();

        }
        else{

            //login succesfully
            current_user = it->second;
            
            current_status_code = current_user.statusCode;

            print_body();

        }
        

    }
    else{

        write_line("No user found.");
        print_main_menu(); //go back to main menu

    }

}

void signup(){

    create_line_break();
    write_line("User Sign up");
    create_line_break();

    UserInfo new_user;

    write("Please enter your email: ");
    string email = read_line();

    //search the database with the info
    auto it = userDB.find(email);
    if(it != userDB.end()){

        //found email, mean already created with the email, cannot sign up with duplicate email
        write_line("Email has already been used.");
        
        print_main_menu();
        
    }
    else{

        write("Please enter your password: ");
        string password = read_line();

        //create username, has to be unique cuz need to show who create the ticket, thus cannot be duplicated
        string userName;
        do{

            write("Please enter a username: ");
            userName = read_line();

            for(const auto& pair : userDB){
                if(userName == pair.second.userName){
                    //found duplicated username
                    write_line("Username has already been used.");
                    userName = "again";
                    break;
                }
            }

        }while(userName == "again");

        new_user.password = password;
        new_user.userName = userName;
        new_user.statusCode = 0;
        
        userDB[email] = new_user;

        write_to_userDB(email, new_user);

        write_line("User created.");
        print_main_menu(); //go back to main menu

    }

}