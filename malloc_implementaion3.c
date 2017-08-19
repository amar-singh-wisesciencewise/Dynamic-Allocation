#include <stdio.h>
#include <stdlib.h>

#define TOTAL_MEM 2000 //heap size in Bytes

typedef struct link{//node structure: use by alloc to maintain the allocation table
	struct link* next;
	struct link* prev;
	int start_index;//it strores the index of first address allocated
	int index_to;//it stores the index of last address allowed
}a_node;

a_node* head = NULL;//it is head of list that maintains allocation table

char mem[TOTAL_MEM];//it is the memory from which alloc() allocate space

extern void* b_alloc(int);//function for fixed size dynamic memory 
extern int b_dealloc(void*);//free the above allocated
void* alloc(int);//function for variable size dynamic memory
int dealloc(void*);//free the variable dynamic memory allocated
a_node* available(int);//it is used by alloc()

int main(){
printf("size of struct = %d\n",sizeof(a_node));//it tells the size of structure so that 
//we could update BLOCKSIZE in fixed_size_malloc.c file

int* ptr = (int*)alloc(10*sizeof(int));
if(ptr!=NULL){
	ptr[0]  = 10;
	ptr[1] = 20;
	ptr[9] = 30;
	printf("\nptr[9] = %d",ptr[9]);//dispalying last allocated index
}

char* ptr2 = (char*)alloc(20);

if(ptr2!=NULL){
	ptr2[0] ='a';
	printf("\nptr[10] = %d",ptr[10]);//displaying ptr last-index+1
	printf("\nptr2[0] = %c %d",ptr2[0],ptr2[0]);
	ptr2[1] = 'm';
	ptr2[2] = 'a';
	ptr2[3] = 'r';
	ptr2[4] = '\0';
	printf("ptr2[3] = %c",ptr2[3]);
}

int* ptr3 = (int*)alloc(10*sizeof(int));

if(ptr3!=NULL){
	ptr3[0]  = 11;
	ptr3[1] = 21;

	printf("\nptr3[1] = %d",ptr3[1]);
}

dealloc(ptr2);

int* ptr4 = (int*)alloc(10);

if(ptr4!=NULL){
	ptr4[0]  = 12;
	ptr4[1] = 22;
	printf("\nptr4[1] = %d",ptr4[1]);
}

printf("\n%p ",ptr2);
printf("\n%p ",ptr4);


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
a_node*  temp2 = (a_node*)b_alloc(sizeof(a_node));//asking to fixed size malloc
if(temp2==NULL) return NULL;

temp2->next = temp2->prev = NULL;//initializing pointers to NULL
printf("\nalloc was called\n");//debug
if(head == NULL)//for first allocation request
{
	temp2->start_index = 1;//set  start_index to 1
	if(x<TOTAL_MEM)//check asked value in total memory range
	{	temp2->index_to = x;//if yes then set inndex_to to asked value
		head = temp2;//made head temp2
		return (void*)&mem[1];
	}else
		{	perror("\nalloc error: no sufficient space\n");
			b_dealloc(temp2);//free the temp2 if asked size is bigger than available
			return NULL;
		}
}else{ 
	if((temp = available(x)) != NULL){//available() returns the address of a_node after which required size is available
		temp2->start_index = temp->index_to+1;//if available
		temp2->index_to = temp->index_to + x;
		//update the pointer and insert the node in list
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
if(x==NULL){
	perror("dealloc error: nothing to free\n");
	return 0;
}

a_node* temp = head ;
int a = (int)((char*)x - mem);//calculating the index from address
printf("\ndealloc was called a = %d",a);//debug

while (temp->start_index <= a){
//traverse till node with start_index equal to "a"
	if(temp->start_index == a){//if a node with start_index = "a" found
			//remove from link and free it up
			if(temp->prev != NULL)//double linked list but not cyclic
			temp->prev->next = temp->next;
			else head  = temp->next;//if temp->prev is NULL then it's head so change head
			
			if(temp->next!=NULL)
			temp->next->prev = temp->prev;
			
			b_dealloc(temp);//free the node
			return 1;
		}else temp = temp->next;
	}

perror("\ndealloc error: no such index found");
return 0;
}


//this function checks wheather the asked size is available...
//it returns the node after which space is available
a_node* available(int x){
a_node* temp = head;

printf("\navailable was called\n");///debug
if(temp->next == NULL)//ie there is only head
{		
	if (temp->index_to + x < TOTAL_MEM)
		return temp;
	else perror("\navailable error: no sufficient space\n");	
}else{
		while(temp->next != NULL){//go till end of list 

			if((temp->next->start_index - temp->index_to) > x)//check for available space
				return temp;//if space available
			else temp= temp->next;//if not available

			if(temp->next == NULL)//for last node
			{
				if (temp->index_to + x < TOTAL_MEM)
					return temp;
				else perror("\navailable error: no sufficient space\n");
			}

		}
	}
return NULL;
}
