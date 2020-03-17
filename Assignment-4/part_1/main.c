#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>

int hextodec(char number[])
{
	int len = strlen(number);
	int base = 1;
	int output = 0;
	int i;
	for(i=len-1;i>=0;i--)
	{
		if(number[i] >= '0' && number[i] <= '9')
		{
			output += (number[i] - 48) * base;
			base = base * 16;
		}
		else if (number[i] >='A' && number[i] <='F')
		{
			output += (number[i] -55 ) * base;
			base = base * 16;
		}
	}
	return output;
}

int Extract_Page(int address)
{
	return ((address & 0x0000ffff) >> 8);
}

int Extract_Offset(int address)
{
	return (address & 0x000000ff);
}

int main(int argc, char *argv[]) 
{
	FILE * logi_address_file;
	int num_of_addresses = 1000;
	int loop;
	int size = 256; //2^8
	char *file = argv[1];
	char in;
	int pg_size = 64;
	char addresses[num_of_addresses];
	logi_address_file = fopen(file,"r");
	int backing_store_file = open("BACKING_STORE.bin",O_RDONLY);
	int index = 0;
	int logical_address, physical_address, page, offset;
	int pagetable[pg_size][2];
	int frame_table[size];
	char frames[size*size];
	int page_fault = 0;
	unsigned int add, dirt;
	int available_frame;
	int frame_offset, st_index;
	int c_value;
	char f_in;
	char byte_stored;
	int loop2;
	bool found = false;
	int value_to_get;
	int indox = -1;
	int replacer = 64;
	double page_fault_percentage;
	for(loop=0;loop<pg_size;loop++)
	{
		for(loop2=0;loop2<2;loop2++)
		{
		pagetable[loop][loop2] = -1;	
		}
	}
	for(loop=0;loop<size;loop++)
	{
		frame_table[loop] = -1;
	}

	//printf("Logical Addresss          Physical Address          Value          Fault          DirtyBit\n");


	while(fscanf(logi_address_file,"%06X %01X",&add,&dirt) != EOF)
	{
		printf("\n");
		
		
		{
			fgetc(logi_address_file);
			logical_address = add;
			page = Extract_Page(logical_address);
			offset = Extract_Offset(logical_address);
			//printf("Logical Address: %d , Page: %d, Offset: %d \n",logical_address,page,offset);
			printf("LOGICAL ADDRESS: %d\n",logical_address);
			printf("READ / WRITE BIT: %d\n",dirt);
			for(loop=0;loop<pg_size;loop++)
			{
				if(pagetable[loop][0] == page)
				{
					value_to_get = pagetable[loop][1] ;
					found = true;
					break;
				}
				else
				{
					found = false;
				}
			}
			if(found)
			{
				
				physical_address = (value_to_get * size) + offset;
				printf("PAGE FAULT: NO\n");
				printf("PHYSCIAL ADDRESS IS:%d\n", physical_address );
			}

			else 
			{
				printf("PAGE FAULT: YES\n");
				for(loop2=0;loop2<pg_size;loop2++)
				{
					if(pagetable[loop2][0] == -1)
					{
						indox = loop2;
						break;
					}
				}
				if(indox == -1)
				{
					indox = replacer % 64;
					replacer++;
				}
				pagetable[indox][0] = page;
				page_fault++;
				for(loop=0;loop<size;loop++)
				{
					if(frame_table[loop] == -1)
					{
						available_frame = loop;
						break;
					}
				}
				if(backing_store_file != -1)
				{
					frame_offset = 0;
					st_index = page * size;
					c_value = lseek(backing_store_file,st_index,SEEK_SET);
					while(frame_offset < size && (c_value))
					{
						c_value=read(backing_store_file,&f_in,sizeof(char));
						frames[available_frame * frame_offset] = f_in;
						frame_offset++;
					}
				}
					else 
					{
						printf("YAKKKKHHHHH\n");
						return 0;
					}

					pagetable[indox][1] = available_frame;
					frame_table[available_frame] = 0;
					physical_address = (available_frame * size) + offset;
					printf("PHYSICAL ADDRESS: %d\n", physical_address);
				}

					byte_stored = frames[physical_address];
					printf("VALUE STORED: %d\n",byte_stored );
					//printf("                              YES");
					memset(addresses,0,sizeof(addresses));
					index = 0;
			}
		}
		printf("\n");
		printf("Total Page Faults: %d\n",page_fault );
		page_fault_percentage = page_fault;
		printf("PAGE FAULT PERCENTAGE: %lf\n",page_fault_percentage/1000 );
		fclose(logi_address_file);
		close(backing_store_file);
		return 0 ;
	}
