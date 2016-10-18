/*
 * structure.h
 *
 *  Created on: Mar 24, 2016
 *      Author: kevinw
 *
 *
 *      This file gives the struct Person and Post and define the function prototypes
 */

#ifndef STRUCTURE_H_
#define STRUCTURE_H_


const int BUFFER_LENGTH=10000;
const int MAX_NUMBER_OF_POSTS=100000;

struct Person {
    char* name;
    Person** friends;
    int num_of_friends;
};

struct Post {
    char* title;
    char* content;
    Person* owner;
    int like;
    Post* followup;
};


Person* create_user(const char* name);
Person* search_user_by_name(Person** list, int size, const char* name);
void add_user_into_list(Person**& list, int size, Person* new_person);
void list_person(Person* p);
void print_post(const Post* p);
void delete_post(Post*& p);
void print_a_user_post(Post const* const* posts, int size, const Person* p);
Post* new_post(const char* title, const char* content, Person* person, Post* reply = NULL);
bool is_friend(const Person* a, const Person* b);
bool add_friend(Person* a, Person* b);
int count_followup(const Post* p);
int get_friends_of_friends(const Person* person, Person**& list);

#endif /* STRUCTURE_H_ */
