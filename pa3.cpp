#include <iostream>
#include <cstring>
#include "structure.h"

using namespace std;

int count_followup(const Post* p)
{
	if(p->followup==NULL)
		return 0;
	return 1+count_followup(p->followup);
}

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

bool is_friend(const Person* a, const Person* b)
{
	if(search_user_by_name(a->friends,a->num_of_friends,b->name)==NULL)
		return false;
	return true;
}

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

void print_a_user_post(Post const* const* posts, int size, const Person* p)
{
	for(int i=0;i<size;i++)
	{
		if(posts[i]->owner==p)
			print_post(posts[i]);
	}
}

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

