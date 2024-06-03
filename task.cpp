#include "task.h"

std::vector<Task> taskDB;

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