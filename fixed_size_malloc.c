#include<stdio.h>
//this file holds the functions for allocation and deallocation of dynamic memory for fixed
//block size

//BLOCKSIZE tells the block size for which this code is designed
#define BLOCKSIZE 20 //update: it to your block size(if size is smaller than 20 it will work)
#define TOTALBLOCK 32//it is how many blocks we are allowed to ask for 

static char block_mem[BLOCKSIZE*TOTALBLOCK];//memory from which code alloate space

static unsigned int bit_field = 0;//this is bit field which keeps record of allocated block

void* b_alloc(int x){//function that allocate space
int i;
//for block size larger than define
if(x > BLOCKSIZE)
{
	perror("\nb_alloc error: block size is larger \n");
	return NULL;
}

for(i=0;i<TOTALBLOCK;i++){//search the bit-field for available space
	if((bit_field & (0x1 << i)) == 0)//search for first 0
	{
		bit_field |= (0x1 << i);//set
		printf("\nb_alloc index = %d\n",i);//debug
		return (void*)&block_mem[i*BLOCKSIZE];//return the address
	}

}
perror("\nb_alloc error: no sufficient  block space\n");
return NULL;
}

int b_dealloc(void* x){
int index;
//check wheather correct addresss has been sent 
if( ((int)((char*)x - block_mem)%BLOCKSIZE) == 0)//address diff would comp div by BLOCKSIZE
index = (int)(((char*)x - block_mem)/BLOCKSIZE);
else {
		perror("\nb_dealloc error: no such block index exist\n");
		return 0;
	}
printf("\nb_dealloc index = %d\n",index);

bit_field ^= (0x1 << index);//reset the bit field

return 1;
}
