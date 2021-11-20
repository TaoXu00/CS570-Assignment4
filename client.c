/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <rpc/rpc.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include "ssnfs.h"
#include <fcntl.h>
#include <errno.h>

CLIENT *clnt;

void
ssnfsprog_1(char *host)
{
clnt = clnt_create (host, SSNFSPROG, SSNFSVER, "tcp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}else
		printf("client is created successfully!\n");
}
int Open(char *filename_to_open){
  open_output  *result_1;
  open_input  open_file_1_arg;
  strcpy(open_file_1_arg.file_name,filename_to_open);
  strcpy(open_file_1_arg.user_name, getpwuid(getuid())->pw_name);
  //printf("username:%s\n", open_file_1_arg.user_name);
  //printf("file to open:%s", open_file_1_arg.file_name);
  
  result_1 = open_file_1(&open_file_1_arg, clnt);
	if (result_1 == (open_output *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	printf ("In client: Directory name is:%s \nIn client: Name of the file opened is:%s \nIn client: file descriptor returned is:%d\n", open_file_1_arg.user_name, open_file_1_arg.file_name,  result_1->fd);
	return  result_1->fd;
}
void Write(int fd, char * buffer, int num_bytes_to_write){
write_output  *result_3;
write_input  write_file_1_arg;
       strcpy(write_file_1_arg.user_name, getpwuid(getuid())->pw_name); // Copy user name.
       write_file_1_arg.fd=fd;
       write_file_1_arg.numbytes=num_bytes_to_write;
       write_file_1_arg.buffer.buffer_val = strdup(buffer);
       write_file_1_arg.buffer.buffer_len = strlen(buffer);
       result_3 = write_file_1(&write_file_1_arg, clnt);
       if (result_3 == (write_output *) NULL) {
		clnt_perror (clnt, "call failed");
	}else{
	if(result_3->success==-1)
        printf("In client: write failed\n");
	else if(result_3->success==1)
	printf("In client: write success\n");
	printf ("%s\n",result_3->out_msg.out_msg_val);
	}
}
//why read has one more parameter of buffer.
//void Read(int fd, char * buffer, int num_bytes_to_read){
void Read(int fd, int num_bytes_to_read){
	/*struct read_output {
	int success;
	struct {
		u_int buffer_len;
		char *buffer_val;
	} buffer;
	struct {
		u_int out_msg_len;
		char *out_msg_val;
	} out_msg;
};*/
        read_output  *result_2;
	read_input  read_file_1_arg;
	char* username =getpwuid(getuid())->pw_name;
	strcpy(read_file_1_arg.user_name, username); // Copy user name.
        read_file_1_arg.fd=fd; //assign file name
	read_file_1_arg.numbytes=num_bytes_to_read;
	result_2 = read_file_1(&read_file_1_arg, clnt);
	printf("send read request!");
	if (result_2 == (read_output *) NULL) {
		clnt_perror (clnt, "call failed");
	}else{
	if(result_2->success==-1)
        printf("In client: read failed\n");
	else if(result_2->success==1)
	printf("In client: read success\n");
	printf ("%s\n",result_2->buffer.buffer_val);
	}
	printf("%s\n", result_2->out_msg.out_msg_val);
}
void Close(int fd){
	close_output  *result_6;
	close_input  close_file_1_arg;
        result_6 = close_file_1(&close_file_1_arg, clnt);
	if (result_6 == (close_output *) NULL) {
		clnt_perror (clnt, "call failed");
	}
}
void List(){
	list_output  *result_4;
	list_input  list_files_1_arg;
	char* username =getpwuid(getuid())->pw_name;
	strcpy(list_files_1_arg.user_name, username); // Copy user name.
	result_4 = list_files_1(&list_files_1_arg, clnt);
	if (result_4 == (list_output *) NULL) {
		clnt_perror (clnt, "call failed");
	}else
	printf("%s\n", result_4->out_msg.out_msg_val);
}
void Delete(char * file_name){
	delete_output  *result_5;
	delete_input  delete_file_1_arg;
	result_5 = delete_file_1(&delete_file_1_arg, clnt);
	if (result_5 == (delete_output *) NULL) {
		clnt_perror (clnt, "call failed");
	}
}	
int get_fd(char* file_name){
	int fd;
	return fd;
}

void prompt(){
		printf("%s\n","*********************************************************************************");
		printf("%s\n","*                          Welcome to SSNFS!                                    *");
		printf("%s\n","*    Please select your operation, press Enter:                                 *");
		printf("%s\n","*    Open file,   input 'o  <filename>'                                         *");
		printf("%s\n","*    Write file,  input 'w  <file_discriptor> <num_bytes_to_write> <data>'      *");
		printf("%s\n","*    Read  file,  input 'r  <file_discriptor> <num_bytes_to_read>'                     *");
		printf("%s\n","*    List files,  input 'l' 			                                *");
		printf("%s\n","*    Delete file, input 'd  <filename>'                                         *");
		printf("%s\n","*    close file,  input 'c  <filename>'                                         *");
		printf("%s\n","*    Enter 'q' to exist the program                                             *");
		printf("%s\n","*********************************************************************************");


}
int
main (int argc, char *argv[])
{
	char *host;
	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	ssnfsprog_1 (host);  //create a RCP client
	// Main Interactive Loop
	prompt();
	while(TRUE){
		char *fd_file, *filename, *command, *tofilename;  // Strings for the line buffer, command, filename...
		char *linebuffer =NULL;		
		int valid_command, fd, num_bytes_to_read, num_bytes_to_write ; // Flag to determine if the user entered a valid command, file discriptor, and number of bytes to read and write....
    	void *outp; // All the output structs have the same structure, just use a single pointer.
    	char *offset, *numbytes, *buffer; // Parameters to read and write.
		
                printf(">");
		size_t len=0;
		ssize_t read=0;
		read=getline(&linebuffer, &len, stdin);
		if(read==-1)
			printf("input error");
		/*else    // for debug
			printf("%s", linebuffer);*/ 
		linebuffer[strlen(linebuffer)-1]='\0';  
		command = strtok(linebuffer, " "); // Split off the first word, the command
		//printf("comamnd:%s\n",command);     //debug
		switch(command[0]){
			case('o'):
				filename=strtok(NULL, " ");
				//printf("filename %s\n", filename);
				if(filename){
					//filename[strlen(filename)-1]='\0';
					Open(filename);
				}
				else // If the user does not enter a filename
					printf("Filename required. \n");
				break;
			case('w'):
				fd_file = strtok(NULL, " "); // Split off the discriptor of the file to which we will write.
                if (fd_file) { // If the user does enter a filename...
                        numbytes = strtok(NULL, " "); // Split off the length of the write.
                        if (numbytes) { // If the user does enter an offset...
                            buffer = strtok(NULL, " "); // Split off content of the write.
                            if (buffer) {
				fd=atoi(fd_file);
                                num_bytes_to_write=atoi(numbytes); // The number of bytes to write.
                                //buffer[strlen(buffer)-1]='\0';
				Write(fd, buffer, num_bytes_to_write);
                                valid_command = 1; // User entered a valid command.
                            } else {
                                printf("Data to write required.\n"); // Report an error message.
                            }
                        } else { // User didn't enter numbytes...
                            printf("Number of bytes required.\n"); // Report an error message.
                        }
                } else // If the user doesn't enter a filename...
                    printf("Filename required.\n"); // Report an error message.
				break;
			case('r'):
		    fd_file = strtok(NULL, " "); // Split off the discriptor of the file from which we will read.
		   if (fd_file) { // If the user does enter a filename...
                    numbytes = strtok(NULL, " "); // Split off the offset.
                        if (numbytes) { // If the user does enter a filename...
                           fd=atoi(fd_file);
			  // numbytes[strlen(numbyte)]='\0'
			   num_bytes_to_read = atoi(numbytes); // The number of bytes to read.
                           Read(fd, num_bytes_to_read);
                           // valid_command = 1; // User entered a valid command.
                        } else {
                            printf("Number of bytes required.\n"); // Report an error message.
                        }
                    
                } else { // If the user doesn't enter a filename...
                    printf("Filename required.\n"); // Report an error message.
                }	
				break;
			case('l'):  //if the user enters the list command
				List();
				break;
			case('d'):  //if the user enters the delete command
				filename=strtok(NULL, " ");
				if(filename)
					Delete(filename);
				else // If the user does not enter a filename
					printf("Filename required. \n");
				break;
			case('c'): //if the user enters close command
				filename=strtok(NULL, " ");
				if(filename){
					fd=get_fd(filename);
					Close(fd);
				}	
				else // If the user does not enter a filename
					printf("Filename required. \n");
				break;
			case('p'):
				prompt();
				break;
			case('q'):
				exit(0);
			default:
			 	printf("%s\n", "Invalid operation, press 'p' to see the manual:");
				
				
		}
		free(linebuffer);
	}
	/*int fd=Open("myfile");
	printf("File descriptor returnd inside main() is:%d\n",  fd);*/
exit (0);
}
