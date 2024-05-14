#include <map>
#include <vector>
#include <chrono>
#include <ctime>
#include "splashkit.h"

enum TaskStatus { 
    WIP,
    Completed
 };

struct UserInfo{

    string userName;
    string password;
    int statusCode; //0-normal user, 100-admin, 200-manager

};

struct Comment{

    string comment_owner; //the user who posted the comment
    string comment_body; //the description
    string comment_date;

};

struct Task{
    int id;
    string task_title;
    string task_description;
    string create_by;
    string create_date;
    string completed_by;
    string completed_date;
    std::vector<Comment> comments; //an array of comments
    TaskStatus task_status;
};

std::map<string,UserInfo> userDB; //user database, pair user email and userInfo struct
std::vector<Task> taskDB;


UserInfo current_user; //store the key of the current user when the user login succesfully
int current_status_code;


void create_line_break();
string read_input_range(int start, int end);
void print_main_menu();
void login();
void signup();
string print_status();
void change_status(int targetCode);
void print_body();
string get_create_time();
void create_ticket();
void edit_ticket(Task& ticket);
void delete_ticket(Task& targetTask);
void post_comment(Task& task);
void mark_task_completed(Task& task, bool complete);
Task* get_task_by_id(int targetId);
void show_task(Task& targetTask);
void show_task(bool wip);

int main(){


    print_main_menu();


    return 0;
}

void create_line_break(){

    write_line("-----------------------------------------------------------------------");

}

string read_input_range(int start, int end){

    string input = read_line();
    
    if(!is_integer(input) || (std::stoi(input) < start && std::stoi(input) > end)){
        
        write_line("Invalid option.");
        write("PLease select your option: ");
        input = read_line();

    }

    return input;

}

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

        write_line("User created.");
        print_main_menu(); //go back to main menu

    }

}

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

void change_status(int targetCode){

    current_status_code = targetCode;

    print_body();

}

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

string get_create_time(){

    //get the current system time
    auto now = std::chrono::system_clock::now();

    //convert the system time to a time point in time_t
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    //convert the time_t value to a local time struct
    std::tm* localTime = std::localtime(&currentTime);

    //extract date and time components from the local time struct
    int year = localTime->tm_year + 1900; //since 1900
    int month = localTime->tm_mon + 1; //start from 0 so add 1
    int day = localTime->tm_mday; //day of the month
    int hour = localTime->tm_hour; //24hr format
    int minute = localTime->tm_min;
    int second = localTime->tm_sec;

    return std::to_string(hour)+":"+std::to_string(minute)+" "+std::to_string(day)+"/"+std::to_string(month)+"/"+std::to_string(year);

}

void create_ticket(){

    Task new_task;

    string input;

    do
    {
    
        create_line_break();
        write_line("Please enter your title:");
        new_task.task_title = read_line();

        write_line("Please enter your description:");
        new_task.task_description = read_line();

        write("Is that okay? [Y/N] ");
        input = read_line();

        while(is_integer(input) || (to_lowercase(input) != "y" && to_lowercase(input) != "n")){
            write("Invalid. Please enter Y or N");
            input = read_line();
        }

    } while (to_lowercase(input) == "n");

    new_task.id = taskDB.size() + 1;
    new_task.create_by = current_user.userName;
    new_task.create_date = get_create_time();
    new_task.task_status = (TaskStatus)0;

    taskDB.push_back(new_task);

    //after creating a task, go back to body
    write_line("Task created successfully");

    print_body();


}

void edit_ticket(Task& ticket){

    string input;

    do
    {
    
        create_line_break();
        write_line("Old title: "+ ticket.task_title);
        write_line("New title: ");
        ticket.task_title = read_line();

        write_line("---------------------------");

        write_line("Old description: "+ticket.task_description);
        write_line("New description: ");
        ticket.task_description = read_line();

        write("Is that okay? [Y/N] ");
        input = read_line();

        while(is_integer(input) || (to_lowercase(input) != "y" && to_lowercase(input) != "n")){
            write("Invalid. Please enter Y or N");
            input = read_line();
        }

    } while (to_lowercase(input) == "n");


    //after editing the task, go back to the wip task list
    write_line("Task edited successfully");

    show_task(true);

}

void delete_ticket(Task& targetTask){

    //performance is O(n) cuz need to loop thru nth element to find the specific element to delete
    int counter = 0;

    for(auto& task : taskDB){

        if(task.id == targetTask.id){

            taskDB.erase(taskDB.begin() + counter);

        }

        counter++;

    }

    write_line("Task deleted.");

    //go back to wip task list
    show_task(true);

}

void post_comment(Task& task){

    write_line("---------------------------");

    Comment new_comment;

    string commentMsg;
    string input;

    do
    {
    
        write_line("Write your comment: ");
        commentMsg = read_line();

        write("Post comment? [Y/N] ");
        input = read_line();

        while(is_integer(input) || (to_lowercase(input) != "y" && to_lowercase(input) != "n")){
            write("Invalid. Please enter Y or N");
            input = read_line();
        }

    } while (to_lowercase(input) == "n");
    
    new_comment.comment_body = commentMsg;
    new_comment.comment_owner = current_user.userName;
    new_comment.comment_date = get_create_time();

    task.comments.push_back(new_comment);

    write_line("Comment has posted.");

    //go back to the task
    show_task(task);

}

void mark_task_completed(Task& task, bool complete){

    task.task_status = complete? (TaskStatus)1: (TaskStatus)0;
    task.completed_by = complete? current_user.userName: "";
    task.completed_date = complete? get_create_time(): "";

    string msg = complete?"Completed":"WIP";
    write_line("This task has been marked as "+msg+".");

    if(complete){
        //mark as completed, mean user at wip menu
        show_task(true);
    }
    else{
        //mark as WIP, mean user at completed menu
        show_task(false);
    }

}

Task* get_task_by_id(int targetId){

    //peformance is O(n) cuz has to search thru the vector array nth times to retrieve the task
    for(auto& task : taskDB){

        if(task.id == targetId){

            return &task;
            
        }    
    }

    return nullptr;

}

//show the specific task searched by id
void show_task(Task& targetTask){

    create_line_break();

    write_line("Task - #"+ std::to_string(targetTask.id));

    string taskStatus = (targetTask.task_status==(TaskStatus)0? "WIP":"Completed");
    write_line("Status : " + taskStatus);

    write_line("---------------------------");
    
    write_line("Title: "+ targetTask.task_title);
    write_line("Description: "+ targetTask.task_description);
    
    write_line("---------------------------");
    
    write_line("Created by "+ targetTask.create_by);
    write_line("Created at "+targetTask.create_date);

    write_line("---------------------------");
    
    write_line("Completed by "+ targetTask.completed_by);
    write_line("Completed at "+targetTask.completed_date);
    
    write_line("---------------------------");
    write_line("Comments: ");
    
    if(targetTask.comments.size() <= 0){
        write_line("No comments yet");
    }
    else{

        int counter = 1;

        for(const auto& comment : targetTask.comments){
            
            write_line(comment.comment_owner + " ("+comment.comment_date+") : "+ comment.comment_body);

        }

    }
    
    //after finished printing the specific task,
    write_line("---------------------------");

    if(targetTask.task_status == (TaskStatus)0){

        write_line("(1) Mask as completed");

    }
    else{

        write_line("(1) Mask as WIP");

    }
    
    write_line("(2) Comment");
    write_line("(3) Go back");

    int end = 3;

    //if user is the thread owner or admin/manager role, then the user can edit or delete
    if(targetTask.create_by == current_user.userName || current_user.statusCode != 0){

        //thread owner
        write_line("(4) Edit");
        write_line("(5) Delete");

        end = 5;

    }

    write("Please select an option: ");
    string input = read_input_range(1,end);

    switch(std::stoi(input)){
        case 1:
            //mark this task as completed
            if(targetTask.task_status == (TaskStatus)0){

                mark_task_completed(targetTask, true);
            }
            else{

                mark_task_completed(targetTask, false);
            }
            break;
        case 2:
            //post a comment
            post_comment(targetTask);
            break;
        case 3:
            if(targetTask.task_status == (TaskStatus)0){

                //go back to the wip task list menu
                show_task(true);
            }
            else{

                //go back to the wip task list menu
                show_task(false);
            }
            
            break;
        case 4:
            //edit the task
            if(end == 5){
                edit_ticket(targetTask);
            }
            break;
        case 5:
            //delete the task
            if(end == 5){
                delete_ticket(targetTask);
            }
            break;
    }

}

void show_task(bool wip){
    
    create_line_break();

    write_line("|  Task ID  |  Task Title  |  Created By  |  Created Date  |  Status  |  Comments  |\n");

    TaskStatus targetTaskStatus = wip? (TaskStatus)0 : (TaskStatus)1;

    //list all the wip task in taskDB
    for(const auto& task : taskDB){

        //filter out wip task
        if(task.task_status == targetTaskStatus){

            write_line(("| "+ std::to_string(task.id) +" | " + task.task_title + " | " + task.create_by + " | " + task.create_date +" | " + (targetTaskStatus==(TaskStatus)0? "WIP":"Completed")+ " | " + std::to_string(task.comments.size()) +" Comments |\n"));

        }

    }

    write_line("Type -1 to go back.");

    bool taskFound = false;
    do{
    
        write("Please select a task with the ID: ");

        //here can't use read input range cuz the id might be not continuous eg after removing a task
        string input = read_line();

        while(!is_integer(input)){
            write_line("Invalid input.");
            write("Please select a task with the ID: ");
            input = read_line();
        }

        //first check if user need to go back or not
        if(std::stoi(input) == -1){
            //go back
            print_body();
            return;
        }

        Task* task_result = get_task_by_id(std::stoi(input));
        if(task_result != nullptr){

            if((wip && task_result->task_status != (TaskStatus)0) || (!wip && task_result->task_status != (TaskStatus)1)){

                write_line("No task found.");
            }
            else{

                taskFound = true;
                show_task(*task_result);

            }
            
        }
        else{
            
            //no task found
            write_line("No task found.");

        }

    }while(!taskFound);

}
