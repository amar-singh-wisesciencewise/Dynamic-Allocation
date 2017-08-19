#include <stdio.h>
#include <stdlib.h>

#define TOTAL_MEM 2000

//TODO: make this program free of malloc() and free()

typedef struct link{
	struct link* next;
	struct link* prev;
	int start_index;
	int index_to;
}a_node;

a_node* head = NULL;

char mem[TOTAL_MEM];
int free_index;/// do you need this

void* alloc(int);
int dealloc(void*);
a_node* available(int);

int main(){
//intializing head of allocation table linked list
//head = (a_node*)malloc(sizeof(a_node));
//head->next = NULL;
//head->prev = NULL;
//head->start_index = 0;
//head->index_to = 0;
int* ptr = (int*)alloc(10*sizeof(int));
ptr[0]  = 10;
ptr[1] = 20;
ptr[10] = 30;
printf("\nptr[1] = %d",ptr[1]);

char* ptr2 = (char*)alloc(20);
ptr2[0] ='a';
printf("\nptr[10] = %d",ptr[10]);
ptr2[1] = 'm';
ptr2[2] = 'a';
ptr2[3] = 'r';
ptr2[4] = '\0';
printf("ptr2[3] = %c",ptr2[3]);


int* ptr3 = (int*)alloc(10*sizeof(int));
ptr3[0]  = 11;
ptr3[1] = 21;

printf("\nptr3[1] = %d",ptr3[1]);

dealloc(ptr2);

int* ptr4 = (int*)alloc(10);
ptr4[0]  = 12;
ptr4[1] = 22;

printf("\n%p ",ptr2);
printf("\n%p ",ptr4);
printf("\nptr4[1] = %d",ptr4[1]);


printf("\nhead = %p",head);

dealloc(ptr);

printf("\nhead = %p",head);

dealloc(ptr3);
dealloc(ptr4);
//dealloc();
printf("\nhead = %p",head);

printf("\n");
return 0;
}

//alloc function
void* alloc(int x){
a_node* temp;
a_node*  temp2 = (a_node*)malloc(sizeof(a_node));

temp2->next = temp2->prev = NULL;
printf("\nalloc was called\n");
if(head == NULL)
{
	temp2->start_index = 1;
	if(x<TOTAL_MEM)
	{	temp2->index_to = x;
		head = temp2;
		return (void*)&mem[1];
	}else
		{	perror("\nerror: no sufficient space\n");
			free(temp2);
			return NULL;
		}
}else{ 
	if((temp = available(x)) != NULL){//available() returns the address of a_node after which required size is available
		temp2->start_index = temp->index_to+1;
		temp2->index_to = temp->index_to + x;

		temp2->next = temp->next;
		temp2->prev = temp;
		if(temp->next!=NULL)
		temp->next->prev = temp2;
		temp->next = temp2;
	
		return (void*)&mem[temp2->start_index];
	}else return NULL;
	}
return NULL;
	
}

int dealloc(void* x){
a_node* temp = head ;

int a = (int)((char*)x - mem);

printf("\ndealloc was called a = %d",a);

while (temp->start_index <= a){
//traverse till node with start_index equal to "a"
	if(temp->start_index == a){
			
			if(temp->prev != NULL)
			temp->prev->next = temp->next;
			else head  = temp->next;
			
			if(temp->next!=NULL)
			temp->next->prev = temp->prev;
			
			free(temp);
			return 1;
		}else temp = temp->next;
}
perror("\nerror has occured: no such index found");
return 0;
}

a_node* available(int x){
a_node* temp = head;

printf("\navailable was called\n");
if(temp->next == NULL)//ie there is only head
	{
		if (temp->index_to + x < TOTAL_MEM)
			return temp;
		else perror("\nerror: no sufficient space\n");
	}else{
		while(temp->next != NULL){

			if((temp->next->start_index - temp->index_to) > x)
				return temp;
			else temp= temp->next;

			if(temp->next == NULL)//for last node
			{
				if (temp->index_to + x < TOTAL_MEM)
					return temp;
				else perror("\nerror: no sufficient space\n");
			}

		}
	}
return NULL;
}
