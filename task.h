#ifndef TASK
#define TASK

#include "database.h"
#include "app.h"
#include "utility.h"

enum TaskStatus { 
    WIP,
    Completed
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

void edit_ticket(Task& ticket);
void delete_ticket(Task& targetTask);
void mark_task_completed(Task& task, bool complete);
Task* get_task_by_id(int targetId);
void show_task(Task& targetTask);
void show_task(bool wip);
void post_comment(Task& task);
void create_ticket();

extern std::vector<Task> taskDB;

#endif