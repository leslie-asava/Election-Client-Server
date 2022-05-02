#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>

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

char* chop(char *string)
{
	int i, len;
	len = strlen(string);
	char *newstring;

	newstring = (char *)malloc(len-1);


	for(i = 0; i < strlen(string)-1; i++)
	{
		newstring[i] = string[i]; 
	}

	return newstring;
}

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
        
		snprintf(buff, sizeof(buff),"\n\t%d. %s\t   Votes:: %d\n", line_number, read_name, array_ptr[line_number]);
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

	snprintf(buff, sizeof(buff), "\n\n\tWINNER :: %s \twith a total of %d votes[%.2f%%]", winner, max, (max / total) *100);
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

// Driver function
int main()
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("server accept failed...\n");
		exit(0);
	}
	else
		printf("server accept the client...\n");

	// Function for communication between client and server
	func(connfd);

	// Close the socket when done
	close(sockfd);
}

