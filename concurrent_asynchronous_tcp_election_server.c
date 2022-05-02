//Example code: A simple server side code, which echos back the received message.
//Handle multiple socket connections with select and fd_set on Linux 
#include <stdio.h> 
#include <string.h>   //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h>   //close 
#include <arpa/inet.h>    //close 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 
     
#include <netdb.h>
#include <unistd.h>
#include <stdbool.h>

#define TRUE   1 
#define FALSE  0 

#define MAX 300
#define PORT 8080
#define SA struct sockaddr

// Detect OS and define clearing command
#if defined(_WIN32)
	#define CLEAR_COMMAND "cls"		// Windows
#else
	#define CLEAR_COMMAND "clear"	// Other OS
# endif

// Declare Functions
char *remove_new_line(char *strbuffer);
void write_to_file(char fname[], char lname[], char regnum[], char pwd[], char file_path[]);
int verify(char voter_file[]);
bool authenticate_voter(char voter_id[20], int connfd);
void vote_position(char position[30], int connfd);
void vote_for_candidates(int connfd);
int get_number_of_candidates(char path[30]);
void print_candidate_on_line(int line_number, char path[30], int connfd);
void registerVoter(int connfd);
void display_results(char position[30], int *array_ptr, int connfd);
int tally_votes(int connfd);
void tally_position(char position[30], int *array_ptr);
int register_candidate(int connfd);
void func(int connfd);

char *remove_new_line(char *strbuffer)
{
	size_t len = strlen(strbuffer);
	if (strbuffer[len - 1] == '\n')
	{
		strbuffer[len - 1] = '\0';
	}

	return strbuffer;
}

struct voter
{
    char fname[50];
    char lname[50];
    char reg_no[30];
    char password[30];
}info;
 
void write_to_file(char fname[], char lname[], char regnum[], char pwd[], char file_path[])
{
    FILE *voter_file;
    voter_file = fopen(file_path, "w+");
    fprintf(voter_file, "%s\n%s\n%s\n%s", regnum, fname, pwd, lname);
    fclose(voter_file);
}
 
int verify(char voter_file[])
{
    FILE *file;
    if((file = fopen(voter_file, "r")) != NULL)
    {
        fclose(file);
        // printf("file exists");
        return -1;
    }
    else
    {
        return 0;
    }
}

bool authenticate_voter(char voter_id[20], int connfd)
{
    char buff[MAX];
	// Set user txt file path
	char path[] = "voters/";
	char extension[] = ".txt";
	strcat(voter_id, extension);
	strcat(path, voter_id);

	// Get user details from file
	FILE * userfileptr;
	userfileptr = fopen(path, "r");

	char line_str[30];
	char read_name[30];
	char read_pass[30];
	int line_number = 1;

	while (fgets(line_str, sizeof(line_str), userfileptr))
	{
		if (line_number == 1)
		{
			line_number++;
			continue;
		}
		else if (line_number == 2)
		{
			strcpy(read_name, remove_new_line(line_str));
			line_number++;
		}
		else if (line_number == 3)
		{
			strcpy(read_pass, remove_new_line(line_str));
			line_number++;
		}
		else
		{
			//strcpy(read_pass, remove_new_line(line_str));
			line_number++;
		}
	}

	fclose(userfileptr);

	bool authenticated = false;

	if (!authenticated)
	{
		// Prompt
		char input_name[30];
		char input_pass[30];
		
        bzero(buff, MAX);
        // read the message from client and copy it in buffer
        read(connfd, buff, sizeof(buff));
        strcpy(input_name, buff);
        strcpy(buff,"ACCEPT");
        write(connfd, buff, sizeof(buff));

		bzero(buff, MAX);
        // read the message from client and copy it in buffer
        read(connfd, buff, sizeof(buff));
        strcpy(input_pass, buff);

		// Determine if credentials match

		if ((strstr(read_name, input_name) != NULL) && (strstr(read_pass, input_pass) != NULL))
		{
			//system(CLEAR_COMMAND);
			printf("\n\t[++] Successful login!!!\n");
            //printf("%s %s %s %s", read_name, input_name,read_pass,input_pass);
            strcpy(buff,"\n\t[++] Success!!!\n");
			write(connfd, buff, sizeof(buff));
			return true;
		}
		else
		{
			//system(CLEAR_COMMAND);
            //printf("\n[%s] \n[%s] \n[%s] \n[%s]", read_name, input_name,read_pass,input_pass);
			printf("\n\t[*#@!] No match found :(\n");
			strcpy(buff,"\n\t[*#@!] No match found :(\n");
			write(connfd, buff, sizeof(buff));
			return false;
			/*char retry[1];
			printf("\n\t[--] Retry?[y/n]: ");
			scanf("%s", retry);
			if (!strcmp(retry, "n"))
			{
				return false;
			}
			else
			{
				system(CLEAR_COMMAND);
			}*/
		}

		char position[30];
		
        bzero(buff, MAX);
        // read the message from client and copy it in buffer
        read(connfd, buff, sizeof(buff));
        strcpy(position, buff);
        strcpy(buff,"ACCEPT");
        write(connfd, buff, sizeof(buff));

		vote_position(position, connfd);
	}
}

void vote_position(char position[30], int connfd)
{
	char buff[MAX];
	char position_title[30];
	strcpy(position_title, position);
	// Prepare path
	char path[20] = "candidates/";
	char extension[] = ".txt";
	strcat(position, extension);
	strcat(path, position);

	// Set up file details
	FILE * candfileptr;
	candfileptr = fopen(path, "r");

	char read_name[30];
	int line_number = 1;

	bzero(buff, MAX);
	// read the message from client and copy it in buffer
	read(connfd, buff, sizeof(buff));
	strcpy(buff, "ACCEPT");
	write(connfd, buff, sizeof(buff));

	bzero(buff, MAX);
	// read the message from client and copy it in buffer
	read(connfd, buff, sizeof(buff));
	strcpy(buff, "ACCEPT");
	write(connfd, buff, sizeof(buff));

	while (fgets(read_name, sizeof(read_name), candfileptr))
	{
		bzero(buff, MAX);
        // read the message from client and copy it in buffer
        read(connfd, buff, sizeof(buff));
        snprintf(buff, sizeof(buff), "\n\t\t%d. %s", line_number, remove_new_line(read_name));
        write(connfd, buff, sizeof(buff));
		line_number++;
	}

	bzero(buff, MAX);
	// read the message from client and copy it in buffer
	read(connfd, buff, sizeof(buff));

	strcpy(buff, "exit");
    write(connfd, buff, sizeof(buff));

	fclose(candfileptr);

	// Vote confirmation variables
	char confirm[1];
	char read_id[30] = { "0" };
	int candidate_selection;

	while (true)
	{
		bzero(buff, MAX);
        // read the message from client and copy it in buffer
        read(connfd, buff, sizeof(buff));
        strcpy(read_id, buff);
		printf("%s\n",read_id);
		
        
		if (atoi(read_id) > get_number_of_candidates(path))
		{
			continue;
		}

		//printf("\n\t\tYour %s selection: ", position_title);
		print_candidate_on_line(atoi(read_id), path, connfd);

		
		bzero(buff, MAX);
        // read the message from client and copy it in buffer
        read(connfd, buff, sizeof(buff));
        strcpy(confirm, buff);
		printf("--------> %s", buff);
        strcpy(buff,"ACCEPT");
        write(connfd, buff, sizeof(buff));

		if (!strcmp(confirm, "n"))
		{
			continue;
		}
		else
		{
			// Change path to votes
			strcpy(path, "votes/");

			strcat(path, position);

			// Set up file details
			FILE * votesfileptr;
			votesfileptr = fopen(path, "a");

			strcat(read_id, "\n");

			// Append Candidate ID to file
			fprintf(votesfileptr,"%s", read_id);

			fclose(votesfileptr);
			system(CLEAR_COMMAND);
			break;
		}
	}
}

void vote_for_candidates(int connfd)
{
	char regInput[20];
    char buff[MAX];
	bzero(buff, MAX);
    // read the message from client and copy it in buffer
    read(connfd, buff, sizeof(buff));
    strcpy(regInput, buff);
    printf("%s",regInput);
    strcpy(buff,"ACCEPT");
    write(connfd, buff, sizeof(buff));

	// Validate User
	bool validate = authenticate_voter(regInput, connfd);

	if (!validate)
	{
		return;
	}

	// Iterate positions
	char position_titles[10][50] = { "Chairperson",
		"Vice Chair",
		"Secretary General",
		"Secretary Finance",
		"Organizing Secretary",
		"Secretary Academic Affairs",
		"Secretary Catering and Accommodation",
		"Secretary Health and Environment",
		"Secretary Sports and Entertainment",
		"Secretary Legal Affairs" };
	int positions = 10;

	int i;

	for (i = 0; i < positions; i++)
	{
		// Display candidates to choose from
		vote_position(position_titles[i], connfd);
	}
}

int get_number_of_candidates(char path[30])
{
	FILE * fileptr;
	fileptr = fopen(path, "r");

	char line_str[30];
	int read_number = 0;

	while (fgets(line_str, sizeof(line_str), fileptr))
	{
		read_number++;
	}

	fclose(fileptr);

	return read_number;
}

void print_candidate_on_line(int line_number, char path[30], int connfd)
{
	FILE * fileptr;
	fileptr = fopen(path, "r");
	char buff[MAX];

	char line_str[30];
	int read_number = 1;

	while (fgets(line_str, sizeof(line_str), fileptr))
	{
		if (read_number == line_number)
		{
			//printf("%s", remove_new_line(line_str));
			snprintf(buff, sizeof(buff), "%s", remove_new_line(line_str));
        	write(connfd, buff, sizeof(buff));
		}

		read_number++;
	}

	fclose(fileptr);
}


void registerVoter(int connfd)
{
    int choice;
    char name[100];
    char pwd_comp[30];
    char password[30];
    char file_path[100] = "";
    char dir[] = "voters/";
    int cmp;
	char buff[MAX];

    char fname[50];
	char lname[50];
	char reg_no[20];
 
    bzero(buff, MAX);
    // read the message from client and copy it in buffer
    read(connfd, buff, sizeof(buff));
    strcpy(fname, buff);
    strcpy(buff,"ACCEPT");
    write(connfd, buff, sizeof(buff));

    bzero(buff, MAX);
    // read the message from client and copy it in buffer
    read(connfd, buff, sizeof(buff));
    strcpy(lname, buff);
    strcpy(buff,"ACCEPT");
    write(connfd, buff, sizeof(buff));

    bzero(buff, MAX);
    // read the message from client and copy it in buffer
    read(connfd, buff, sizeof(buff));
    strcpy(reg_no, buff);

 
    strcat(file_path, dir); //      voters/
    strcat(file_path, reg_no);//     voters/P1546
    strcat(file_path, ".txt"); // voters/P1546.txt
 
    int loc_file = verify(file_path);
    snprintf(buff, sizeof(buff), "%d", loc_file);
    write(connfd, buff, sizeof(buff));
    if( loc_file == 0)
    {
       bzero(buff, MAX);
        // read the message from client and copy it in buffer
        read(connfd, buff, sizeof(buff));
        strcpy(password, buff);
        strcpy(buff,"ACCEPT");
        write(connfd, buff, sizeof(buff));

        bzero(buff, MAX);
        // read the message from client and copy it in buffer
        read(connfd, buff, sizeof(buff));
        strcpy(pwd_comp, buff);
        strcpy(buff,"ACCEPT");
        write(connfd, buff, sizeof(buff));
       
        cmp = strcmp(password, pwd_comp);
        if(cmp == 0)
        {
            //printf("%s", file_path);
            write_to_file(fname, lname, reg_no, password, file_path);
            //bzero(buff, MAX);
            // read the message from client and copy it in buffer
            //read(connfd, buff, sizeof(buff));
            strcpy(buff,"Registration successful");
            write(connfd, buff, sizeof(buff));
        }
        else if(cmp > 0)
        {
            //printf("the passwords do not match\n");
            bzero(buff, MAX);
            // read the message from client and copy it in buffer
            read(connfd, buff, sizeof(buff));
            strcpy(pwd_comp, buff);
            strcpy(buff,"the passwords do not match\n");
            write(connfd, buff, sizeof(buff));
        }
    }
    else if(loc_file == -1)
    {
        //printf("account already exist !!!!\n");
        bzero(buff, MAX);
        // read the message from client and copy it in buffer
        read(connfd, buff, sizeof(buff));
        strcpy(pwd_comp, buff);
        strcpy(buff,"account already exist !!!!\n");
        write(connfd, buff, sizeof(buff));
        
    }
 
}



void display_results(char position[30], int *array_ptr, int connfd)
{
    char buff[MAX];
    bzero(buff, MAX);

    // read the message from client and copy it in buffer
    read(connfd, buff, sizeof(buff));
	snprintf(buff, sizeof(buff),"\n\n\tPosition: %s", position);
    write(connfd, buff, sizeof(buff));


	// Variables for reading file
	char path[20] = "candidates/";

	// Prepare path
	strcat(path, position);

	// Set up file details
	FILE * candfileptr;
	candfileptr = fopen(path, "r");

	char read_name[50];
	char read_id[50];
	int line_number = 1;
	int max = -1;
	char winner[80];
	float total = 0;

	while (fgets(read_name, sizeof(read_name), candfileptr))
	{
        bzero(buff, sizeof(buff));
	    read(connfd, buff, sizeof(buff));
        
		snprintf(buff, sizeof(buff),"\n\t\t%d. %s \t-\t %d", line_number, remove_new_line(read_name), array_ptr[line_number]);
		write(connfd, buff, sizeof(buff));
        total = total + array_ptr[line_number];

		// Track who has the highest number of votes
		if (max < array_ptr[line_number])
		{
			max = array_ptr[line_number];
			strcpy(winner, read_name);
		}

		// If there is a tie print both
		else if (max == array_ptr[line_number])
		{
			max = array_ptr[line_number];
			strcat(winner, " &");
			strcat(winner, read_name);
		}

		line_number += 1;
	}

    bzero(buff, sizeof(buff));
	read(connfd, buff, sizeof(buff));

	snprintf(buff, sizeof(buff), "\n\n\tWINNER :: %s with a total of %d votes[%.2f%%]", winner, max, (max / total) *100);
	//printf("\n------------------------------------------------------");
    write(connfd, buff, sizeof(buff));

	fclose(candfileptr);
}

int tally_votes(int connfd)
{
	// Define titles
	char position_titles[10][50] = { "Chairperson",
		"Vice Chair",
		"Secretary General",
		"Secretary Finance",
		"Organizing Secretary",
		"Secretary Academic Affairs",
		"Secretary Catering and Accommodation",
		"Secretary Health and Environment",
		"Secretary Sports and Entertainment",
		"Secretary Legal Affairs" };
	int positions = 10;
    char buff[80];

	// Array to hold totals for each position
	int position_tallies[10][30] = { 0 };

	// Iterate positions
	int i;
	for (i = 0; i < positions; i++)
	{
		int *array_ptr = position_tallies[i];
		tally_position(position_titles[i], array_ptr);
		// Display results for this position
		display_results(position_titles[i], array_ptr, connfd);
	}
    bzero(buff, sizeof(buff));
	read(connfd, buff, sizeof(buff));

	strcpy(buff, "exit");
    write(connfd, buff, sizeof(buff));
}

void tally_position(char position[30], int *array_ptr)
{
	strcat(position, ".txt");

	// Variables for reading file
	char path[20] = "votes/";

	// Prepare path
	strcat(path, position);

	// Set up file details
	FILE * candfileptr;
	candfileptr = fopen(path, "r");

	char read_name[30];
	char read_id[30];
	int line_number = 1;
	//int candidate_votes[30] = {0};

	while (fgets(read_id, sizeof(read_id), candfileptr))
	{
		//printf("\n\t\t%d. %s", line_number, read_name);
		line_number += 1;
		array_ptr[atoi(read_id)]++;
	}

	fclose(candfileptr);

}


int register_candidate(int connfd)
{
	char fname[50];
	char lname[50];
	char reg[20];
    char buff[MAX];
	FILE * fptr;

    printf("\n%s\n","called");

	// Define titles
	char position_titles[10][50] = { "Chairperson",
		"Vice Chair",
		"Secretary General",
		"Secretary Finance",
		"Organizing Secretary",
		"Secretary Academic Affairs",
		"Secretary Catering and Accommodation",
		"Secretary Health and Environment",
		"Secretary Sports and Entertainment",
		"Secretary Legal Affairs" };
	int positions = 10;
	char file_path[50] = "candidates/";

    bzero(buff, MAX);

    // read the message from client and copy it in buffer
    read(connfd, buff, sizeof(buff));

	int choice = atoi(buff);
	
    switch (choice)
	{
		case 1:
			strcat(file_path, position_titles[choice - 1]);
			strcat(file_path, ".txt");
			fptr = fopen(file_path, "a");

			break;

		case 2:
			strcat(file_path, position_titles[choice - 1]);
			strcat(file_path, ".txt");
			fptr = fopen(file_path, "a");

			break;

		case 3:
			strcat(file_path, position_titles[choice - 1]);
			strcat(file_path, ".txt");
			fptr = fopen(file_path, "a");

			break;

		case 4:
			strcat(file_path, position_titles[choice - 1]);
			strcat(file_path, ".txt");
			fptr = fopen(file_path, "a");

			break;

		case 5:
			strcat(file_path, position_titles[choice - 1]);
			strcat(file_path, ".txt");
			fptr = fopen(file_path, "a");

			break;

		case 6:
			strcat(file_path, position_titles[choice - 1]);
			strcat(file_path, ".txt");
			fptr = fopen(file_path, "a");

			break;

		case 7:
			strcat(file_path, position_titles[choice - 1]);
			strcat(file_path, ".txt");
			fptr = fopen(file_path, "a");

			break;

		case 8:
			strcat(file_path, position_titles[choice - 1]);
			strcat(file_path, ".txt");
			fptr = fopen(file_path, "a");

			break;

		case 9:
			strcat(file_path, position_titles[choice - 1]);
			strcat(file_path, ".txt");
			fptr = fopen(file_path, "a");

			break;

		case 10:
			strcat(file_path, position_titles[choice - 1]);
			strcat(file_path, ".txt");
			fptr = fopen(file_path, "a");

			break;

		default:
			printf("\nError: Invalid Choice");
			return 1;
	}

    strcpy(buff, "ACCEPT");
    write(connfd, buff, sizeof(buff));

	// Error Reading File
	if (fptr == NULL)
	{
		printf("Error!");
		return 0;
	}
	else
	{
        bzero(buff, MAX);
        // read the message from client and copy it in buffer
        read(connfd, buff, sizeof(buff));
	    strcpy(fname, buff);
        // send
		strcpy(buff, "ACCEPT");
        write(connfd, buff, sizeof(buff));

		bzero(buff, MAX);
        // read the message from client and copy it in buffer
        read(connfd, buff, sizeof(buff));
	    strcpy(lname, buff);
        // send
		strcpy(buff, "ACCEPT");
        write(connfd, buff, sizeof(buff));

		bzero(buff, MAX);
        // read the message from client and copy it in buffer
        read(connfd, buff, sizeof(buff));
	    strcpy(reg, buff);
        // send
		strcpy(buff, "ACCEPT");
        write(connfd, buff, sizeof(buff));

		fprintf(fptr, "%s %s %s \n", fname, lname, reg);
		fclose(fptr);

	}

	return 0;
}

// Function designed for chat between client and server.
void func(int connfd)
{
	char buff[MAX];
	int n;
	// infinite loop for chat
	for (;;) {
		bzero(buff, MAX);

		// read the message from client and copy it in buffer
		read(connfd, buff, sizeof(buff));
		// print buffer which contains the client contents
		printf("From client: %s\t To client : ", buff);

        if (atoi(buff) == 1)
        {
            printf("\nClient chose 1 \n");
            bzero(buff, MAX);
            n = 0;
            // and send that buffer to client
            strcpy(buff, "1");
            write(connfd, buff, sizeof(buff));
            register_candidate(connfd);
        }

        else if (atoi(buff) == 2)
        {
            bzero(buff, MAX);
            n = 0;
            // and send that buffer to client
            strcpy(buff, "2");
            write(connfd, buff, sizeof(buff));
            registerVoter(connfd);
        }

        else if (atoi(buff) == 3)
        {
            bzero(buff, MAX);
            n = 0;
            // and send that buffer to client
            strcpy(buff, "3");
            write(connfd, buff, sizeof(buff));
            vote_for_candidates(connfd);
        }

        else if (atoi(buff) == 4)
        {
            bzero(buff, MAX);
            n = 0;
            // and send that buffer to client
            strcpy(buff, "4");
            write(connfd, buff, sizeof(buff));
            tally_votes(connfd);
        }

		// if msg contains "Exit" then server exit and chat ended.
		if (strncmp("exit", buff, 4) == 0) {
			printf("Server Exit...\n");
			break;
		}
	}
}

     
int main(int argc , char *argv[])  
{  
    int opt = TRUE;  
    int master_socket , addrlen , new_socket , client_socket[30] , 
          max_clients = 30 , activity, i , valread , sd;  
    int max_sd;  
    struct sockaddr_in address;  
         
    char buffer[1025];  //data buffer of 1K 
         
    //set of socket descriptors 
    fd_set readfds;  
         
    //a message 
    char *message = "ECHO Daemon v1.0 \r\n";  
     
    //initialise all client_socket[] to 0 so not checked 
    for (i = 0; i < max_clients; i++)  
    {  
        client_socket[i] = 0;  
    }  
         
    //create a master socket 
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)  
    {  
        perror("socket failed");  
        exit(EXIT_FAILURE);  
    }  
     
    //set master socket to allow multiple connections , 
    //this is just a good habit, it will work without this 
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
          sizeof(opt)) < 0 )  
    {  
        perror("setsockopt");  
        exit(EXIT_FAILURE);  
    }  
     
    //type of socket created 
    address.sin_family = AF_INET;  
    address.sin_addr.s_addr = INADDR_ANY;  
    address.sin_port = htons( PORT );  
         
    //bind the socket to localhost port 8888 
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)  
    {  
        perror("bind failed");  
        exit(EXIT_FAILURE);  
    }  
    printf("Listener on port %d \n", PORT);  
         
    //try to specify maximum of 3 pending connections for the master socket 
    if (listen(master_socket, 3) < 0)  
    {  
        perror("listen");  
        exit(EXIT_FAILURE);  
    }  
         
    //accept the incoming connection 
    addrlen = sizeof(address);  
    puts("Waiting for connections ...");  
         
    while(TRUE)  
    {  
        //clear the socket set 
        FD_ZERO(&readfds);  
     
        //add master socket to set 
        FD_SET(master_socket, &readfds);  
        max_sd = master_socket;  
             
        //add child sockets to set 
        for ( i = 0 ; i < max_clients ; i++)  
        {  
            //socket descriptor 
            sd = client_socket[i];  
                 
            //if valid socket descriptor then add to read list 
            if(sd > 0)  
                FD_SET( sd , &readfds);  
                 
            //highest file descriptor number, need it for the select function 
            if(sd > max_sd)  
                max_sd = sd;  
        }  
     
        //wait for an activity on one of the sockets , timeout is NULL , 
        //so wait indefinitely 
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);  
       
        if ((activity < 0) && (errno!=EINTR))  
        {  
            printf("select error");  
        }  
             
        //If something happened on the master socket , 
        //then its an incoming connection 
        if (FD_ISSET(master_socket, &readfds))  
        {  
            if ((new_socket = accept(master_socket, 
                    (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)  
            {  // Function for communication between client and server
	            func(new_socket);
                perror("accept");  
                exit(EXIT_FAILURE);  
            }  
             
            //inform user of socket number - used in send and receive commands 
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs
                  (address.sin_port));  
           
            //send new connection greeting message 
            if( send(new_socket, message, strlen(message), 0) != strlen(message) )  
            {  
                perror("send");  
            }  
                 
            puts("Welcome message sent successfully");  
                 
            //add new socket to array of sockets 
            for (i = 0; i < max_clients; i++)  
            {  
                //if position is empty 
                if( client_socket[i] == 0 )  
                {  
                    client_socket[i] = new_socket;  
                    printf("Adding to list of sockets as %d\n" , i);  
                         
                    break;  
                }  
            }  
        }  
             
        //else its some IO operation on some other socket
        for (i = 0; i < max_clients; i++)  
        {  
            sd = client_socket[i];  
                 
            if (FD_ISSET( sd , &readfds))  
            {  
                //Check if it was for closing , and also read the 
                //incoming message 
                if ((valread = read( sd , buffer, 1024)) == 0)  
                {  
                    //Somebody disconnected , get his details and print 
                    getpeername(sd , (struct sockaddr*)&address , \
                        (socklen_t*)&addrlen);  
                    printf("Host disconnected , ip %s , port %d \n" , 
                          inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  
                         
                    //Close the socket and mark as 0 in list for reuse 
                    close( sd );  
                    client_socket[i] = 0;  
                }  
                     
                //Echo back the message that came in 
                else 
                {  
                    //set the string terminating NULL byte on the end 
                    //of the data read 
                    buffer[valread] = '\0';  
                    send(sd , buffer , strlen(buffer) , 0 );  
                }  
            }  
        }  
    }  
         
    return 0;  
}  