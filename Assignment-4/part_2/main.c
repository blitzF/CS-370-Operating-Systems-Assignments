#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>


int Extract_Page_IN(int address, int k, int p)
{
	return (((1 << k) - 1) & (address >> (p-1)));
}
int Extract_Page_OUT(int address,int k, int p)
{
	return (((1 << k) - 1) & (address >> (p-1)));
}

int Extract_Offset(int address,int k,int p)
{
	return (((1 << k) - 1) & (address >> (p-1)));
}

int main(int argc, char *argv[]) 
{
	FILE * logi_address_file;
	int loop;
	int size = 4096; //2^8
	char *file = argv[1];
	char in;
	int pg_size_out = 64;
	int pg_size_in = 256;
	logi_address_file = fopen(file,"r");
	int data_store_file = open("DATA_STORE.bin",O_RDONLY);
	int index = 0;
	int logical_address, physical_address, page_in,page_out, offset;

	int pagetable1[pg_size_out][2];
	int pagetable2[pg_size_in][2];
	
	int frame_table[size];
	char frames[size];
	int page_fault_table = 0;
	int page_fault_memo = 0;
	unsigned int add, dirt;
	int available_frame;
	int frame_offset, st_index;
	int c_value;
	char f_in;
	char byte_stored;
	int loop2;
	bool found_out = false;
	bool found_in = false;
	int value_to_get_out, value_to_get_in,value_to_get;
	int indox = -1;
	int indoxx = -1;
	int indoxxx= -1;
	int replacer_out = 64;
	int replacer_in = 256;
	int frame_replacer = 4096;
	bool frame_found = false;
	int frame_fault = 0;
	double page_fault_percentage;
	for(loop=0;loop<pg_size_out;loop++)
	{
		for(loop2=0;loop2<2;loop2++)
		{
		pagetable1[loop][loop2] = -1;	
		}
	}
	for(loop=0;loop<pg_size_in;loop++)
	{
		for(loop2=0;loop2<2;loop2++)
		{
		pagetable2[loop][loop2] = -1;	
		}
	}

	for(loop=0;loop<size;loop++)
	{
		frame_table[loop] = -1;
	}

	while(fscanf(logi_address_file,"%06X %01X",&add,&dirt) != EOF)
	{
		printf("\n");
		
			fgetc(logi_address_file);
			logical_address = add;
			page_out = Extract_Page_OUT(logical_address,6,1);
			page_in = Extract_Page_IN(logical_address,8,7);
			offset = Extract_Offset(logical_address,10,15);
			//printf("Logical Address: %d , Page: %d, Offset: %d \n",logical_address,page,offset);
			printf("LOGICAL ADDRESS: %d\n",logical_address);
			printf("READ / WRITE BIT: %d\n",dirt);
			for(loop=0;loop<pg_size_out;loop++)
			{
				if(pagetable1[loop][0] == page_out)
				{
					value_to_get_out = pagetable1[loop][1];
					value_to_get = value_to_get_out + page_in;
					found_out = true;
					break;
				}
				else
				{
					found_out = false;
				}
			}
			if(found_out)
			{
				for(loop=0;loop<pg_size_in;loop++)
			{
				if(pagetable2[loop][0] == value_to_get)
				{
					value_to_get_in = pagetable2[loop][1];
					found_in = true;
					break;
				}
				else
				{
					found_in = false;
				}
			}
				if(found_in)
				{
				physical_address = (value_to_get_in * pg_size_in) + offset;
				//printf("PAGE FAULT: NO\n");
				printf("PHYSCIAL ADDRESS IS:%d\n", physical_address );
					
				}
				else
				{
					page_fault_table++;
					printf("PAGE FAULT: YES\n");
					for(loop2=0;loop2<pg_size_in;loop2++)
				{
					if(pagetable2[loop2][0] == -1)
					{
						indox = loop2;
						break;
					}
				}
				if(indox == -1)
				{
					indox = replacer_in % 256;
					replacer_in++;
				}
				pagetable2[indox][0] = value_to_get;
				for(loop=0;loop<size;loop++)
				{
					if(frame_table[loop] == -1)
					{
						available_frame = loop;
						frame_found = true;
						break;
					}
					else
					{
						frame_found = false;
					}

				}
				if(frame_found == false)
					{
						available_frame = frame_replacer % size;
						frame_fault++;
						frame_replacer++;
						printf("FRAME FAULT: YES\n");
					}
				if(data_store_file != -1)
				{
					frame_offset = 0;
					st_index = page_in * pg_size_in;
					c_value = lseek(data_store_file,st_index,SEEK_SET);
					while(((available_frame * frame_offset) < size) && (c_value))
					{
						c_value=read(data_store_file,&f_in,sizeof(char));
						frames[available_frame * frame_offset] = f_in;
						frame_offset++;
					}
				}
					else 
					{
						printf("YAKKKKHHHHH\n");
						return 0;
					}

					pagetable2[indox][1] = available_frame;
					frame_table[available_frame] = 0;
					physical_address = (available_frame * pg_size_in) + offset;
					printf("PHYSICAL ADDRESS: %d\n", physical_address);

				}
			}
			//else of OUT
			else
			{
				for(loop2=0;loop2<pg_size_out;loop2++)
				{
					if(pagetable1[loop2][0] == -1)
					{
						indoxx = loop2;
						break;
					}
				}
				if(indoxx == -1)
				{
					indoxx = replacer_out % 64;
					replacer_out++;
				}
				pagetable1[indoxx][0] = page_out;
				pagetable1[indoxx][1] = page_in;
				value_to_get = pagetable1[indoxx][1];
				for(loop=0;loop<pg_size_in;loop++)
				{
				if(pagetable2[loop][0] == value_to_get)
				{
					value_to_get_in = pagetable2[loop][1];
					found_in = true;
					break;
				}
				else
				{
					found_in = false;
				}
				}
				if(found_in)
				{
					physical_address = (value_to_get_in * pg_size_in) + offset;
					//printf("PAGE FAULT: NO\n");
					printf("PHYSCIAL ADDRESS IS:%d\n", physical_address );
					
				}
				else // NEW ELSE
				{
					page_fault_table++;
					printf("PAGE FAULT: YES\n");
					for(loop2=0;loop2<pg_size_in;loop2++)
					{	
						if(pagetable2[loop2][0] == -1)
						{
							indoxxx = loop2;
							break;
						}
					}
				if(indoxxx == -1)
				{
					indoxxx = replacer_in % 256;
					replacer_in++;
				}
				pagetable2[indoxxx][0] = value_to_get;
			for(loop=0;loop<size;loop++)
				{
					if(frame_table[loop] == -1)
					{
						available_frame = loop;
						frame_found = true;
						break;
					}
					else
					{
						frame_found = false;
					}
				}
				if(frame_found==false)
				{
					printf("FRAME FAULT: YES\n");
					available_frame = frame_replacer % size;
					frame_replacer++;
					frame_fault++;
				}
				if(data_store_file != -1)
				{
					frame_offset = 0;
					st_index = page_in * pg_size_in;
					c_value = lseek(data_store_file,st_index,SEEK_SET);
					while(((available_frame *frame_offset) < size) && (c_value))
					{
						c_value=read(data_store_file,&f_in,sizeof(char));
						frames[available_frame * frame_offset] = f_in;
						frame_offset++;
					}
				}
				else 
				{
						printf("YAKKKKHHHHH\n");
						return 0;
				}
					pagetable2[indoxxx][1] = available_frame;
					frame_table[available_frame] = 0;
					physical_address = (available_frame * pg_size_in) + offset;
					printf("PHYSICAL ADDRESS: %d\n", physical_address);

				} //NEW ELSE

			}
					//byte_stored = frames[physical_address];
					byte_stored = frames[available_frame];
					printf("VALUE STORED: %d\n",byte_stored );

	} // WHILE
		printf("\n");
		printf("Total Page Faults: %d\n",page_fault_table );
		printf("Total Frame Faults: %d\n",frame_fault );		
		//page_fault_percentage = page_fault;
		//printf("PAGE FAULT PERCENTAGE: %lf\n",page_fault_percentage/1000 );*/
		//fclose(logi_address_file);
		//close(data_store_file);
	return 0;
} // MAIN
	
