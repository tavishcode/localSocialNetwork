#include <iostream>
#include "structure.h"

using namespace std;

// menu option used during interaction mode
const char* MENU_ITEM[] = { "Create new Users", "Login", "View your own posts",
                            "View a friend's posts", "New Post", "See the friends list",
                            "See your friends of friends network",
                            "Add friends", "Exit Program" };

// enum used during interaction mode. note that QUIT is the 9th item and has the value 8. 
// LAST_ITEM would also be 8 in this case. It is easier for developer to add more command in the middle
// without counting the number of items.
enum {
    CREATE_USERS,
    LOGIN,
    OWN_POST,
    FRIEND_POST,
    NEW_POST,
    LIST_FRIEND,
    FRIENDS_OF_FRIENDS,
    ADD_FRIEND,
    QUIT,
    LAST_ITEM = QUIT
};

// to display the main menu in the interaction mode
int display_mainmenu() {
    int option;
    do {
        for (int i = 0; i <= LAST_ITEM; i++) {
            cout << i << ")" << MENU_ITEM[i] << endl;
        }
        cout << "Please select one of the options: ";
        cin >> option;
        cin.ignore(); 
    } while (option < 0 || option > LAST_ITEM);
    return option;
}


//This function preload a social networks with four users, for testing purpose.
void preload(Person**& users, int& number_of_users) {
    Person* a = create_user("Randy");
    Person* b = create_user("Roger");
    Person* c = create_user("Mariano");
    Person* d = create_user("Chien-Ming");
    add_user_into_list(users, number_of_users++, a);
    add_user_into_list(users, number_of_users++, b);
    add_user_into_list(users, number_of_users++, c);
    add_user_into_list(users, number_of_users++, d);
    add_friend(a, b);
    add_friend(b, c);
    add_friend(b, d);
    add_friend(a, c);
}


// Interaction mode allows a human user to use the social network after finishing all required functions.
// You will be able to try your code using interaction mode without writing extra test cases
void interaction_mode() {

    Person** users = NULL;   //A dynamic array of Person pointers
    int number_of_users = 0; //Total number of users in the dynamic array
    Post* posts[MAX_NUMBER_OF_POSTS]; //A static array of list of Post
    int number_of_posts = 0; //Total post in the array posts.

    Person* me = NULL;  //A Person pointer to indicate the identity of one logged in.
    bool quit = false;  //A boolean to control the program ends or not
    int option;         //To identify which option the user has picked.
    char buffer[BUFFER_LENGTH];  //An input buffer to receive user's input

    //other variables used in temporary
    Person* tmp_person = NULL;
    Post* tmp_post = NULL;
    char* title = NULL;
    char* content = NULL;
    int count;
    int option2;

    char to_preload;
    do {
        cout << "Do you want to preload some users? (Y/N)" << endl;
        cin >> to_preload;
    } while (to_preload != 'Y' && to_preload !='N');
    if (to_preload == 'Y')
        preload(users, number_of_users);

    do {
        if (me == NULL)
            cout << "You have not login yet" << endl;
        else
            cout << "Welcome, " << me->name << "!" << endl;
        option = display_mainmenu();
        switch (option) {
        case CREATE_USERS:
            cout << "Registering a new user. " << endl
                    << "Please enter the new user's name:" << endl;
            cin.getline(buffer, BUFFER_LENGTH);
            tmp_person = create_user(buffer);
            add_user_into_list(users, number_of_users, tmp_person);
            number_of_users++; //after adding the user, the number_of_users increment by one

            cout << "The following users has been created" << endl;
            list_person(tmp_person);
            break;
        case LOGIN:
            cout << "Login." << endl << "Type your full name:";
            cin.getline(buffer, BUFFER_LENGTH);
            me = search_user_by_name(users, number_of_users, buffer);
            if (me == NULL) {
                cout << "Login fails" << endl;
                break;
            }
            break;
        case OWN_POST:
            if (me == NULL)
                break;
            cout << "Post of " << me->name << ":" << endl;
            cout << "============================" << endl;
            print_a_user_post(posts, number_of_posts, me);
            cout << "Enter the post ID to reply, or enter -1 to return main menu:" << endl;
            cin >> option2;
            if (option2 < 0 || option2 >= number_of_posts)
                break;
            if (posts[option2]->owner != me) {
                cout << "Not a post by " << tmp_person->name << endl;
                break;
            }
            cout << "Write Something to Reply." << endl;
	    cin.ignore();
            cin.getline(buffer, BUFFER_LENGTH);
            tmp_post = new_post(posts[option2]->title, buffer, me, posts[option2]);
            break;
        case FRIEND_POST:
            if (me == NULL)
                break;

            cout << "Check a friends' posts." << endl << "Enter the full name of your friend: ";
            cin.getline(buffer, BUFFER_LENGTH);
            tmp_person = search_user_by_name(users, number_of_users, buffer);
            if (tmp_person == NULL) {
                cout << "User not found" << endl;
                break;
            }

            if (!is_friend(tmp_person, me)) {
                cout << "You are not friend of " << buffer << "!" << endl;
                break;
            }
            print_a_user_post(posts, number_of_posts, tmp_person);



            cout << "Enter the post ID to like or to reply, or enter -1 to return main menu:" << endl;
            cin >> option2;
            if (option2 < 0 || option2 >= number_of_posts)
                break;
            if (posts[option2]->owner != tmp_person) {
                cout << "Not a post by " << tmp_person->name << endl;
                break;
            }

            do {
                cout << "Enter R for Reply, L for Like. You can like many many times you want" << endl;
                cin >> buffer;
            } while (strcmp(buffer,"R") && strcmp(buffer, "L"));
            if (!strcmp(buffer, "L")) {
                posts[option2]->like++;
                cout << "One more like count to the post. " << endl;
            } else {
                cout << "Write Something to Reply." << endl;
		cin.ignore();
                cin.getline(buffer, BUFFER_LENGTH);
                tmp_post = new_post(posts[option2]->title, buffer, me, posts[option2]);
            }


            break;
        case NEW_POST:
            if (me == NULL)
                break;
            cout << "Write a new Post. " << endl << "Title: ";
            cin.getline(buffer, BUFFER_LENGTH);
            title = new char[strlen(buffer) + 1]; //add one more character for the terminator null symbol
            strcpy(title, buffer);
            cout << "Content: ";
            cin.getline(buffer, BUFFER_LENGTH);
            content = new char[strlen(buffer) + 1]; //add one more character for the terminator null symbol
            strcpy(content, buffer);

            //we call your function here
            tmp_post = new_post(title, content, me);

            posts[number_of_posts] = tmp_post;
            number_of_posts++;

            if (title != NULL) {
              delete [] title;
              title = NULL;
            }
            if (content != NULL) {
              delete [] content;
              content = NULL;
            }

            cout << "You have just posted the following online!" << endl;
            print_post(tmp_post);

            break;
        case LIST_FRIEND:
            if (me == NULL)
                break;

            for (int i = 0; i < number_of_users; i++) {
                if (is_friend(users[i], me))
                    list_person(users[i]);
            }
            break;
        case FRIENDS_OF_FRIENDS:
            if (me == NULL)
                break;

            Person **tmp_list;
            count = get_friends_of_friends(me, tmp_list);
            for (int i = 0; i < count; i++)
                list_person(tmp_list[i]);
            break;
        case ADD_FRIEND:
            if (me == NULL)
                break;
            cout << "Add friend." << endl << "Type the full name of your friends:";
            cin.getline(buffer, BUFFER_LENGTH);
            tmp_person = search_user_by_name(users, number_of_users, buffer);
            if (add_friend(me, tmp_person))
                cout << "You and " << tmp_person->name <<  " are now friends to each other." << endl;
            else
                cout << "You are unable to make friends with " << tmp_person->name << endl;
            break;
        case QUIT:
            cout << "Bye" << endl;
            quit = true;
        }
        cout << "--------------------------------------" << endl;
    } while (!quit);

    for (int i = 0; i < number_of_users; i++) {
        if (users[i] != NULL) {
            delete [] users[i]->name;
            if (users[i]->friends != NULL)
                delete [] users[i]->friends;
            delete users[i];
            users[i] = NULL; 
        }
    }
    if (users != NULL) {
        delete[] users;
        users = NULL;
    }

    for (int i = 0; i < number_of_posts; i++)
        delete_post(posts[i]);

    //we don't put delete[] post here as 'posts' is a static array
    //however, each of the element of posts is not static.

}


//test case 1, this test the function create_user, is_friend, add_friend, get_friends_of_friends
void test_case1() {
    cout << "Test Case 1" << endl;
    Person* a = create_user("User a");
    Person* b = create_user("User b");
    Person* c = create_user("User c");
    Person* d = create_user("User d");

    cout << "Initially they are not friend to each other" << endl;
    cout << is_friend(a, b) << is_friend(b, c) << is_friend(c, a) << endl;
    cout << "Even if you look at the other way" <<endl;
    cout << is_friend(b, a) << is_friend(c, b) << is_friend(a, c) << endl << endl;

    add_friend(a, b);
    cout << "Now a and b are friends" << endl;
    cout << is_friend(a, b) << is_friend(b, c) << is_friend(c, a) << endl;
    cout << "Even if you look at the other way" <<endl;
    cout << is_friend(b, a) << is_friend(c, b) << is_friend(a, c) << endl << endl;

    add_friend(b, c);
    cout << "Now b and c are friends, but not a and c" << endl;
    cout << is_friend(a, b) << is_friend(b, c) << is_friend(c, a) << endl;
    cout << "Even if you look at the other way" <<endl;
    cout << is_friend(b, a) << is_friend(c, b) << is_friend(a, c) << endl << endl;

    cout << "Now we make c and d friends" << endl << endl;
    add_friend(c, d);

    cout << "Try taking the friends of friends of a: Should be (b and c)" << endl;
    Person** list;
    int count = get_friends_of_friends(a, list);
    for (int i = 0; i < count; i++) {
        list_person(list[i]);
    }

    cout << "How about the friends of friends of b? Should be (a, c, d)" << endl;
    Person** list2;
    int count2 = get_friends_of_friends(b, list2);
    for (int i = 0; i < count2; i++) {
        list_person(list2[i]);
    }

    cout << "Let's clean up the memory" << endl;
    delete [] list;
    delete [] list2;
    delete [] a->friends;
    delete [] b->friends;
    delete [] c->friends;
    delete [] d->friends;
    delete [] a->name;
    delete [] b->name;
    delete [] c->name;
    delete [] d->name;
    delete a;
    delete b;
    delete c;
    delete d;
}


//this testing function test create_user, new_post, add_friend, delete_post
void test_case2() {
    cout << "Test Case 2" << endl;
    Person* a = create_user("User A");
    Person* b = create_user("User B");
    Person* c = create_user("User C");

    add_friend(a, b);
    add_friend(b, c);

    Post* p[4];
    p[0] = new_post("The first post", "The content of first post", a);
    p[1] = new_post("The second post", "The content of second post", b);
    p[2] = new_post("The third post", "The content of third post", c);
    p[3] = new_post("The fourth post", "The content of fourth post", c);

    cout << "Print Out all post here" << endl;
    for (int i = 0; i < 4; i++)
        print_post(p[i]);

    cout << "Now follow up some post" << endl;
    new_post("Does not matter", "Followup 1 on first post", a, p[0]);
    new_post("Does not matter", "Followup 2 on first post", b, p[0]);
    new_post("Does not matter", "Followup 3 on first post", c, p[0]);

    new_post("Does not matter", "Followup 1 on second post", a, p[1]);
    new_post("Does not matter", "Followup 2 on second post", b, p[1]);
    new_post("Does not matter", "Followup 3 on second post", c, p[1]);

    new_post("Does not matter", "Followup 1 on third post", a, p[2]);
    new_post("Does not matter", "Followup 2 on third post", b, p[2]);
    new_post("Does not matter", "Followup 3 on third post", c, p[2]);

    cout << "Print Out all post here after some reply, User C should not comment on the first post and User A should not comment on the third post." << endl;
    for (int i = 0; i < 4; i++)
        print_post(p[i]);

    cout << "There should be 2 follow up on first post: " << count_followup(p[0]) << endl;
    cout << "There should be 3 follow up on second post: " << count_followup(p[1]) << endl;
    cout << "There should be 2 follow up on third post: " << count_followup(p[2]) << endl;
    cout << "There should be 0 follow up on fourth post: " << count_followup(p[3]) << endl;

    cout << "Let's clean up the memory" << endl;
    for (int i = 0; i < 4; i++)
        delete_post(p[i]);
    delete [] a->friends;
    delete [] b->friends;
    delete [] c->friends;
    delete [] a->name;
    delete [] b->name;
    delete [] c->name;
    delete a;
    delete b;
    delete c;

}

//this test the function create_user, search_user_by_name, add_user_into_list
void test_case3() {
    cout << "Test Case 3" << endl;
    Person* p[4];
    p[0] = create_user("User A");
    p[1] = create_user("User B");
    p[2] = create_user("User C");
    p[3] = create_user("User D");

    cout << "Try to search a name call Uber A: ";
    if (search_user_by_name(p, 4, "Uber A") == NULL)
        cout << "Should be no such user" << endl;
    else
        cout << "Something wrong with your code" << endl;

    cout << "Try to search a name call user b (case sensitive): ";
    if (search_user_by_name(p, 4, "user b") == NULL)
        cout << "Should be no such user" << endl;
    else
        cout << "Something wrong with your code" << endl;

    cout << "Try to search a name call User C: ";
    if (search_user_by_name(p, 4, "User C") == p[2])
        cout << "Should find User C" << endl;
    else
        cout << "Something wrong with your code" << endl;

    cout << "Now try testing the function add_user_into_list" << endl;
    Person** list;
    for (int i = 0; i < 4; i++)
        add_user_into_list(list, i, p[i]);

    cout << "Print those people from list - there should be 4." << endl;
    for (int i = 0; i < 4; i++)
        list_person(p[i]);


    cout << "Let's clean up the memory" << endl;
    for (int i = 0; i < 4; i++) {
        delete [] list[i]->name;
        if (list[i]->friends != NULL)
            delete [] list[i]->friends;
        delete list[i];
    }
    delete [] list;
    //no need to delete p[i] as they are same as list[i], suppose you got the right code.

}

int main() {
    int option;
    cout << "Select 0) for interaction, 1-3 for different test case" << endl;
    cin >> option;
    switch (option) {
    case 0:
        interaction_mode(); break;
    case 1:
        test_case1(); break;
    case 2:
        test_case2(); break;
    case 3:
        test_case3(); break;
    default:
        cout << "invalid choice" << endl; break;
    }
    return 0;
}

//print the details of a list_person.
void list_person(Person* p) {
    if (p == NULL)
        return;
    cout << "Name: \t" << p->name << "\t";
    cout << "He has " << p->num_of_friends << " friends." << endl;
}


//print a post and its follow-up post
void print_post(const Post* p) {
    if (p == NULL)
        return;
    cout << p->title << endl;
    cout << p->content << endl;
    cout << p->like << " people like it" << endl;
    cout << "There are " << count_followup(p) << " follow-up posts." << endl;
    cout << "----------------------------" << endl;
    Post* f = p->followup;
    while (f != NULL) {
        cout << f->owner->name << " says: " << f->content << endl;
        f = f->followup;
    }
    cout << "============================" << endl;

}

