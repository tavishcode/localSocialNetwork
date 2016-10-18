/*
 * pa3.cpp
 *
 *  Complete on this file and submit it the CASS before due day.
 *  Good luck. If you need to seek clarification of help regarding this assignment,
 *  please approach Kevin Wang via (kevinw@cse.ust.hk) or write a post on the course facebook group.
 */

#include <iostream>
#include <cstring>
#include "structure.h"

using namespace std;


/**
 * TODO: This function counts how many follow-up post in a post thread 'p'.
 */
int count_followup(const Post* p)
{
	if(p->followup==NULL)
		return 0;
	return 1+count_followup(p->followup);
}

/**
 * TODO:  This function creates a Person structure in the memory and assigns the structure's
 * name with the given parameter. The initiate numbers of friends should be 0.
 */

Person* create_user(const char* name)
{
Person* person=new Person;
person->name=new char[BUFFER_LENGTH];
strcpy(person->name,name);
person->num_of_friends=0;
person->friends=new Person*[1];
person->friends[0] = NULL;
return person;
}

/**
 * TODO:  This function searches a person from a list with the name matched with a given
 *  string. It returns the address of the Person if it is found, or returns NULL if no such person is found.
 */
Person* search_user_by_name(Person** list, int list_size,  const char* name)
{
for(int i=0;i<list_size;i++)
{
	if(strcmp(list[i]->name,name)==0)
	{
		return list[i];
	}
}
return NULL;
}

/**
 * TODO: Given a list (array) of Person pointers 'list' with the size 'size', this function adds a
 * Person pointer 'new_person' into the list. However, if the 'person' is already in the
 * list, it will simply return without doing anything. In this function, you should take
 * care the size of the list and suitable enlarging 'list' using dynamic memory management
 * techniques.
 * In this function, you may always assume the size of list is correct. If the size is 0,
 * 'list' can be assumed empty without initialized.
 */
void add_user_into_list(Person**& list, int size, Person* new_person)
{
if(size==0)
{
	list=new Person*[1];
	list[0]=new_person;
}
else if(search_user_by_name(list,size,new_person->name)==NULL)
{
	Person**new_list=new Person*[size+1];
	for(int i=0;i<size;i++)
	{
		new_list[i] =list[i];
	}
	delete[] list;
	new_list[size]=new_person;
	list = new_list;
}
}


/**
 * TODO: This function will delete a thread of post from the memory. If there are any
 * follow up post, it should subsequently delete them as well
 */
void delete_post(Post*& p)
{
	if(p==NULL)
		return;
	delete_post(p->followup);
	delete[] p->title;
	delete[] p->content;
	delete p;
	p=NULL;
}

/**
 * TODO: Given the post title 'title', post content 'content', the owner of the post 'person',
 * this function should create a new post and return the pointer of this post.
 *
 * In case this post is a follow-up post of another post, the parameter 'reply_to' should not
 *  be NULL. It should be the pointer of the post being followed-up. In this case the post
 *  'reply_to' should link to this new_post as well.
 */
Post* new_post(const char* title, const char* content, Person* person, Post* reply_to)
{
	if(reply_to==NULL)
	{
		Post* post= new Post;
		post->title= new char[BUFFER_LENGTH];
		strcpy(post->title,title);
		post->content= new char[BUFFER_LENGTH];
		strcpy(post->content,content);
		post->owner=person;
		post->followup=NULL;
		post->like=0;
		return post;
	}
	else if((reply_to!=NULL) && (((reply_to->owner)==person)||(is_friend(person,reply_to->owner))))
	{
		Post* post= new Post;
		post->title= new char[BUFFER_LENGTH];
		strcpy(post->title,title);
		post->content= new char[BUFFER_LENGTH];
		strcpy(post->content,content);
		post->owner=person;
		post->followup=NULL;
		post->like=0;
		for(;reply_to!=NULL;reply_to=reply_to->followup)
		{
			if(reply_to->followup==NULL)
			{
				reply_to->followup=post;
				break;
			}
		}
		return post;
	}
	return NULL;
}

/**
 * TODO: This function checks whether Person 'a' and 'b' are friends (return true if they
 * are). Please note that friendship is commutative but not associative. That means, if A
 * is B's friend, B should be A's friend. But if A and B are C's friends, A and B may or
 * may not be friend.
 */
bool is_friend(const Person* a, const Person* b)
{
	if(search_user_by_name(a->friends,a->num_of_friends,b->name)==NULL)
		return false;
	return true;
}

/**
 * TODO: This function make Person 'a' and 'b' friends. Under one of the following
 * conditions, this function will abort and return false:
 * 		1) 'a' and 'b' is actually the same person. (One cannot be its own friend); or
 * 		2) 'a' and 'b' are friend already.
 */
bool add_friend(Person* a, Person* b)
{
	if(a==b || is_friend(a,b))
		return false;
	Person **new_friend_list_a= new Person*[(a->num_of_friends)+1];
	for(int i=0;i<(a->num_of_friends);i++)
	{
		new_friend_list_a[i]=(a->friends[i]);
	}
	new_friend_list_a[(a->num_of_friends)]=b;
	delete[] a->friends;
	a->friends=new_friend_list_a;
	a->num_of_friends++;

	Person **new_friend_list_b= new Person*[(b->num_of_friends)+1];
	for(int i=0;i<(b->num_of_friends);i++)
	{
		new_friend_list_b[i]=(b->friends[i]);
	}
	new_friend_list_b[(b->num_of_friends)]=a;
	delete[] b->friends;
	b->friends=new_friend_list_b;
	b->num_of_friends++;
	return true;
}
/**
 * TODO: This function will print all posts of a user. This function must use our
 * given function void print_post(const Post*). The order of post
 * printing must follow the original order in the variable 'posts'. In other words,
 * if post A and B are to printed and A is placed before B on the list (array) 'posts', A
 * should print before B. In this function, it is very important to note that you must
 * not do any cout or printf.
 * In this function, you may always assume the size of 'posts' is correct. If the size
 * is 0, 'post' can be assumed empty without initialized.
 */
void print_a_user_post(Post const* const* posts, int size, const Person* p)
{
	for(int i=0;i<size;i++)
	{
		if(posts[i]->owner==p)
			print_post(posts[i]);
	}
}

/**
 * TODO: This function create a list of friends-of-friends of the Person 'person' and
 * save it in the list 'list' (dynamic array of pointers). It also returns the size of this list. The definition of
 * friends-of-friends of a person A is:
 * 		1) any person that is a friend of person A; or
 * 		2) any person that is a friend of a friend of person A, excluding person A himself.
 *
 */
int get_friends_of_friends(const Person* person, Person**& list)
{
	int count=0;
	list=new Person*[1];
	list[0]=NULL;
	for(int i=0;i<person->num_of_friends;i++)
	{
		if(list[0]==NULL)
		{
			list[0]=person->friends[i];
			count++;
		}
		else if(search_user_by_name(list,count,person->friends[i]->name)==NULL)
		{
			Person**temp_list1=new Person*[count+1];
			for(int j=0;j<count;j++)
				temp_list1[j]=list[j];
			temp_list1[count]=person->friends[i];
			delete[] list;
			list= temp_list1;
			count++;
		}
		else
			continue;
		for(int j=0;j<person->friends[i]->num_of_friends;j++)
		{
			if(search_user_by_name(list,count,person->friends[i]->friends[j]->name)==NULL
				&& (person->friends[i]->friends[j]!=person))
			{
				Person** temp_list2=new Person*[count+1];
				for(int k=0;k<count;k++)
					temp_list2[k]=list[k];
				temp_list2[count]=person->friends[i]->friends[j];
				delete[] list;
				list= temp_list2;
				count++;
			}
		}
	}
	return count;
}

