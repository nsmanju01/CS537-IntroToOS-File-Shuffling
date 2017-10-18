#include<stdio.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>


int main(int argc, char *argv[]) {
		FILE *file_ptr_open = NULL;
		FILE *file_ptr_write = NULL;
		FILE *file_empty = NULL;
		int next_character = 0;
		char *input_file_name = NULL;
		char *output_file_name = NULL;
		
		struct stat file_details;
		char *ptr_buffer = NULL;
		
		size_t bufferlen_read = 0;
		
		int ptr_arr_index = 0;
		int i = 0;
		int j = 0;
		int k = 0;
		char *input_check = "-i";
		char *output_check = "-o";
		if (argc != 5)
		{
		fprintf(stderr,"Usage: shuffle -i inputfile -o outputfile\n");
		exit(1);
		}

		if (!((strcmp(argv[1],input_check) == 0 && strcmp(argv[3],output_check) == 0) || (strcmp(argv[1],output_check) == 0 && strcmp(argv[3],input_check) == 0)))
		{
		fprintf(stderr,"Usage: shuffle -i inputfile -o outputfile\n");
		exit(1);
		}

		

		while((next_character = getopt(argc, argv, "i:o:"))!= -1)
		{
			switch(next_character)
			{	
		
			case 'i':
				input_file_name = optarg;
				break;
			case 'o':
				output_file_name = optarg;
				break;
			default :
				fprintf(stderr,"Usage: shuffle -i inputfile -o outputfile\n");
				exit(1);
			}		
		}

		//Opening the input file
		
		file_ptr_open = fopen(input_file_name,"r");
		if (file_ptr_open == NULL)
			{
				fprintf(stderr,"Error: Cannot open file %s\n",input_file_name);
				exit(1);
			}
		//calculating the size of the file using stat command
		int return_stat ;
		return_stat = stat(input_file_name, &file_details);

		if(return_stat == -1)
		{
			fprintf(stderr, "Error : Cannot identify file size\n");
			exit(1);
		}
		int f_size = file_details.st_size;
		if (f_size == 0)
		{
			file_empty=fopen(output_file_name,"w");
			fclose(file_empty);
			exit(0);
		}
		ptr_buffer = (char *)malloc(f_size);
		if (ptr_buffer == NULL)
		{
			fprintf(stderr,"Error : Memory not available\n");
			exit(1);
		}
		
		//Reading the entire input data to buffer location
		bufferlen_read = fread(ptr_buffer,1 ,f_size ,file_ptr_open);
		
		if (bufferlen_read!= f_size)
		{
			fprintf(stderr,"Error : File read failed\n");
			exit (1);
		}
		
		

		//Close the opened file
		
		fclose(file_ptr_open);
		
		
		int n_lines=0;
		
		for(k=0; k<f_size; k++)
		{
			if(ptr_buffer[k]=='\n')
				n_lines++;
		}
		
		char **char_pointer_to_array = (char **) malloc(sizeof(char *) * n_lines); 
		
		if (char_pointer_to_array == NULL)
		{
			fprintf(stderr,"Error : Memory allocation failed");			
			exit(1);
		}
		
		
		j=0;
		//Create the array of char pointer
		char_pointer_to_array[0] = (char *)ptr_buffer;
		
		ptr_arr_index = 1;
			
		//Pointers to the Address of first character each line present in the input buffer pointer	
		while(j<f_size-1)
		{
				
			if(ptr_buffer[j] == '\n')
			{
				char_pointer_to_array[ptr_arr_index] = (char *) (ptr_buffer+j+1);
				ptr_arr_index++;
				
			}
			j++;
			
		};

		//Open the output file for writing
		
		file_ptr_write = fopen(output_file_name,"w");
		if (file_ptr_write == NULL)
		{
			fprintf(stderr,"Could not open the file");
			exit(1);
				
		}
		//Writing the contents of Input Buffer to the files using the array of character pointers
		i = 0;
		j = ptr_arr_index-1;
	
		while (i <= j)
		 {
			
			bufferlen_read = fwrite(char_pointer_to_array[i], 1, (char_pointer_to_array[i + 1] - char_pointer_to_array[i]), file_ptr_write);

				if ( bufferlen_read != (char_pointer_to_array[i + 1] - char_pointer_to_array[i]))
					{
						
						fprintf(stderr,"Write Unsuccessful - first loop\n");
						exit(1);
					}
				if (i<j)
				{
					if( j== ptr_arr_index-1)
					{
						bufferlen_read = fwrite(char_pointer_to_array[j], 1, (f_size-(size_t)(char_pointer_to_array[j]-char_pointer_to_array[0])), file_ptr_write);
						if ( bufferlen_read != (f_size-(size_t)(char_pointer_to_array[j]-char_pointer_to_array[0])))
						{
							fprintf(stderr,"Write Unsuccessful\n");
							exit(1);
						}
					}	
					else
					{
						bufferlen_read = fwrite(char_pointer_to_array[j], 1,(char_pointer_to_array[j+1] - char_pointer_to_array[j]), file_ptr_write);
						if (bufferlen_read != (char_pointer_to_array[j+1] - char_pointer_to_array[j]))
						{
							fprintf(stderr,"Write Unsuccessful\n");
							exit(1);
						}
						
					}
				}
				i++;
				j--;
		}

		
		
		fclose(file_ptr_write);
		
		return 0;
		
}
