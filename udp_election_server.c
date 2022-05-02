// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
   
#define PORT     8080
#define MAXLINE 1024
#define MAX 300

// Detect OS and define clearing command
#if defined(_WIN32)
	#define CLEAR_COMMAND "cls"		// Windows
#else
	#define CLEAR_COMMAND "clear"	// Other OS
# endif

// Function Declaration
char *remove_new_line(char *strbuffer);
void func(int connfd, struct sockaddr_in cliaddr, int len);
int register_candidate(int connfd, struct sockaddr_in cliaddr, int len);
int verify(char voter_file[]);
void registerVoter(int connfd, struct sockaddr_in cliaddr, int len);
void write_to_file(char fname[], char lname[], char regnum[], char pwd[], char file_path[]);
int tally_votes(int connfd, struct sockaddr_in cliaddr, int len);
void tally_position(char position[30], int *array_ptr);
void display_results(char position[30], int *array_ptr, int connfd, struct sockaddr_in cliaddr, int len);
void vote_for_candidates(int connfd, struct sockaddr_in cliaddr, int len);
int get_number_of_candidates(char path[30]);
void print_candidate_on_line(int line_number, char path[30], int connfd, struct sockaddr_in cliaddr, int len);
void vote_for_candidates(int connfd, struct sockaddr_in cliaddr, int len);
void vote_position(char position[30], int connfd, struct sockaddr_in cliaddr, int len);
bool authenticate_voter(char voter_id[20], int connfd, struct sockaddr_in cliaddr, int len);

// Struct to hold voter data
struct voter
{
    char fname[50];
    char lname[50];
    char reg_no[30];
    char password[30];
}info;

void send_data(int connfd, struct sockaddr_in cliaddr, int len, char *buff)
{
    sendto(connfd, (const char *)buff, strlen(buff), 
    MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
        len);
}

char *receive_data(int connfd, struct sockaddr_in cliaddr, int len, char *buff)
{
    bzero(buff, MAX);

    int n;

    // read the message from client and copy it in buffer

    n = recvfrom(connfd, (char *)buff, MAXLINE, 
            MSG_WAITALL, ( struct sockaddr *) &cliaddr,
            &len);
    buff[n] = '\0';

	return buff;
}

// Remove trailing new line from string
char *remove_new_line(char *strbuffer)
{
	size_t len = strlen(strbuffer);
	if (strbuffer[len - 1] == '\n')
	{
		strbuffer[len - 1] = '\0';
	}

	return strbuffer;
}

int register_candidate(int connfd, struct sockaddr_in cliaddr, int len)
{
	char fname[50];
	char lname[50];
	char reg[20];
    char buff[MAX];
	FILE * fptr;

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
    int n;

    bzero(buff, MAX);

    // read the message from client and copy it in buffer
    bzero(buff, MAX);
    n = recvfrom(connfd, (char *)buff, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
    buff[n] = '\0';

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
    // Send response to client
    sendto(connfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);

	// Error Reading File
	if (fptr == NULL)
	{
		printf("Error!");
		return 0;
	}
	else
	{
        // read the message from client and copy it in buffer
        bzero(buff, MAX);
        n = recvfrom(connfd, (char *)buff, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        buff[n] = '\0';
	    strcpy(fname, buff);
        // send
		strcpy(buff, "ACCEPT");
        sendto(connfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);

		// read the message from client and copy it in buffer
        bzero(buff, MAX);
        n = recvfrom(connfd, (char *)buff, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        buff[n] = '\0';
	    strcpy(lname, buff);
        // send
		strcpy(buff, "ACCEPT");
        sendto(connfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);

		// read the message from client and copy it in buffer
        bzero(buff, MAX);
        n = recvfrom(connfd, (char *)buff, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        buff[n] = '\0';
	    strcpy(reg, buff);
        // send
		strcpy(buff, "ACCEPT");
        sendto(connfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);

		fprintf(fptr, "%s %s %s \n", fname, lname, reg);
		fclose(fptr);

	}

	return 0;
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

void write_to_file(char fname[], char lname[], char regnum[], char pwd[], char file_path[])
{
    FILE *voter_file;
    voter_file = fopen(file_path, "w+");
    fprintf(voter_file, "%s\n%s\n%s\n%s", regnum, fname, pwd, lname);
    fclose(voter_file);
}

void registerVoter(int connfd, struct sockaddr_in cliaddr, int len)
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
 
    int n;
    bzero(buff, MAX);
    // read the message from client and copy it in buffer
    n = recvfrom(connfd, (char *)buff, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
    buff[n] = '\0';
    strcpy(fname, buff);
    strcpy(buff,"ACCEPT");
    sendto(connfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);

    bzero(buff, MAX);
    // read the message from client and copy it in buffer
    n = recvfrom(connfd, (char *)buff, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
    buff[n] = '\0';
    strcpy(lname, buff);
    strcpy(buff,"ACCEPT");
    sendto(connfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);

    bzero(buff, MAX);
    // read the message from client and copy it in buffer
    n = recvfrom(connfd, (char *)buff, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
    buff[n] = '\0';
    strcpy(reg_no, buff);

 
    strcat(file_path, dir); //      voters/
    strcat(file_path, reg_no);//     voters/P1546
    strcat(file_path, ".txt"); // voters/P1546.txt
 
    int loc_file = verify(file_path);
    snprintf(buff, sizeof(buff), "%d", loc_file);
    sendto(connfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
    if( loc_file == 0)
    {
        bzero(buff, MAX);
        // read the message from client and copy it in buffer
        n = recvfrom(connfd, (char *)buff, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        buff[n] = '\0';
        strcpy(password, buff);
        strcpy(buff,"ACCEPT");
        sendto(connfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);

        bzero(buff, MAX);
        // read the message from client and copy it in buffer
        n = recvfrom(connfd, (char *)buff, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        buff[n] = '\0';
        strcpy(pwd_comp, buff);
        strcpy(buff,"ACCEPT");
        sendto(connfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
       
        cmp = strcmp(password, pwd_comp);
        if(cmp == 0)
        {
            //printf("%s", file_path);
            write_to_file(fname, lname, reg_no, password, file_path);
            //bzero(buff, MAX);
            // read the message from client and copy it in buffer
            //read(connfd, buff, sizeof(buff));
            strcpy(buff,"Registration successful");
            sendto(connfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
        }
        else if(cmp > 0)
        {
            //printf("the passwords do not match\n");
            bzero(buff, MAX);
            // read the message from client and copy it in buffer
            n = recvfrom(connfd, (char *)buff, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
            buff[n] = '\0';
            strcpy(pwd_comp, buff);
            strcpy(buff,"the passwords do not match\n");
            sendto(connfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
        }
    }
    else if(loc_file == -1)
    {
        //printf("account already exist !!!!\n");
        bzero(buff, MAX);
        // read the message from client and copy it in buffer
        n = recvfrom(connfd, (char *)buff, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        buff[n] = '\0';
        strcpy(pwd_comp, buff);
        strcpy(buff,"account already exist !!!!\n");
        sendto(connfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
        
    }
 
}

void display_results(char position[30], int *array_ptr, int connfd, struct sockaddr_in cliaddr, int len)
{
    char buff[MAX];
    int n;
    bzero(buff, MAX);

    // read the message from client and copy it in buffer
    n = recvfrom(connfd, (char *)buff, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
    buff[n] = '\0';
	snprintf(buff, sizeof(buff),"\n\n\tPosition: %s", position);
    sendto(connfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);


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
	    n = recvfrom(connfd, (char *)buff, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        buff[n] = '\0';
        
		snprintf(buff, sizeof(buff),"\n\t%d. %s\t   Votes:: %d\n", line_number, read_name, array_ptr[line_number]);
		sendto(connfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
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
	n = recvfrom(connfd, (char *)buff, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
    buff[n] = '\0';

	snprintf(buff, sizeof(buff), "\n\n\tWINNER :: %s \twith a total of %d votes[%.2f%%]", winner, max, (max / total) *100);
	//printf("\n------------------------------------------------------");
    sendto(connfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);

	fclose(candfileptr);
}

int tally_votes(int connfd, struct sockaddr_in cliaddr, int len)
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
    int n;

	// Array to hold totals for each position
	int position_tallies[10][30] = { 0 };

	// Iterate positions
	int i;
	for (i = 0; i < positions; i++)
	{
		int *array_ptr = position_tallies[i];
		tally_position(position_titles[i], array_ptr);
		// Display results for this position
		display_results(position_titles[i], array_ptr, connfd, cliaddr, len);
	}
    bzero(buff, sizeof(buff));
	n = recvfrom(connfd, (char *)buff, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
    buff[n] = '\0';

	strcpy(buff, "exit");
    sendto(connfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
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

bool authenticate_voter(char voter_id[20], int connfd, struct sockaddr_in cliaddr, int len)
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
        int n;
		
        bzero(buff, MAX);
        // read the message from client and copy it in buffer
        n = recvfrom(connfd, (char *)buff, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        buff[n] = '\0';
        strcpy(input_name, buff);
        strcpy(buff,"ACCEPT");
        sendto(connfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);

		bzero(buff, MAX);
        // read the message from client and copy it in buffer
        n = recvfrom(connfd, (char *)buff, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        buff[n] = '\0';
        strcpy(input_pass, buff);

		// Determine if credentials match

		if ((strstr(read_name, input_name) != NULL) && (strstr(read_pass, input_pass) != NULL))
		{
			//system(CLEAR_COMMAND);
			printf("\n\t[++] Successful login!!!\n");
            strcpy(buff,"\n\t[++] Success!!!\n");
			sendto(connfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
			return true;
		}
		else
		{
			//system(CLEAR_COMMAND);
			printf("\n\t[*#@!] No match found :(\n");
			strcpy(buff,"\n\t[*#@!] No match found :(\n");
			sendto(connfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
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
        n = recvfrom(connfd, (char *)buff, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        buff[n] = '\0';
        strcpy(position, buff);
        strcpy(buff,"ACCEPT");
        sendto(connfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);

		vote_position(position, connfd, cliaddr, len);
	}
}

void vote_position(char position[30], int connfd, struct sockaddr_in cliaddr, int len)
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
    int n;

	bzero(buff, MAX);
	// read the message from client and copy it in buffer
	n = recvfrom(connfd, (char *)buff, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
    buff[n] = '\0';
	strcpy(buff, "ACCEPT");
	sendto(connfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);

	bzero(buff, MAX);
	// read the message from client and copy it in buffer
	n = recvfrom(connfd, (char *)buff, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
    buff[n] = '\0';
	strcpy(buff, "ACCEPT");
	sendto(connfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);

	while (fgets(read_name, sizeof(read_name), candfileptr))
	{
		bzero(buff, MAX);
        // read the message from client and copy it in buffer
        n = recvfrom(connfd, (char *)buff, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        buff[n] = '\0';
        snprintf(buff, sizeof(buff), "\n\t\t%d. %s", line_number, remove_new_line(read_name));
        sendto(connfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
		line_number++;
	}

	bzero(buff, MAX);
	// read the message from client and copy it in buffer
	n = recvfrom(connfd, (char *)buff, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
    buff[n] = '\0';

	strcpy(buff, "exit");
    sendto(connfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);

	fclose(candfileptr);

	// Vote confirmation variables
	char confirm[1];
	char read_id[30] = { "0" };
	int candidate_selection;

	while (true)
	{
		bzero(buff, MAX);
        // read the message from client and copy it in buffer
        n = recvfrom(connfd, (char *)buff, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        buff[n] = '\0';
        strcpy(read_id, buff);
		printf("%s\n",read_id);
		
        
		if (atoi(read_id) > get_number_of_candidates(path))
		{
			continue;
		}

		//printf("\n\t\tYour %s selection: ", position_title);
		print_candidate_on_line(atoi(read_id), path, connfd, cliaddr, len);

		
		bzero(buff, MAX);
        // read the message from client and copy it in buffer
        n = recvfrom(connfd, (char *)buff, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        buff[n] = '\0';
        strcpy(confirm, buff);
		printf("%s", buff);
        strcpy(buff,"ACCEPT");
        sendto(connfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);

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

void vote_for_candidates(int connfd, struct sockaddr_in cliaddr, int len)
{
	char regInput[20];
    char buff[MAX];
    int n;

	bzero(buff, MAX);
    // read the message from client and copy it in buffer
    n = recvfrom(connfd, (char *)buff, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
    buff[n] = '\0';
    strcpy(regInput, buff);
    printf("%s",regInput);
    strcpy(buff,"ACCEPT");
    sendto(connfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);

	// Validate User
	bool validate = authenticate_voter(regInput, connfd, cliaddr, len);

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
		vote_position(position_titles[i], connfd, cliaddr, len);
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

void print_candidate_on_line(int line_number, char path[30], int connfd, struct sockaddr_in cliaddr, int len)
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
        	sendto(connfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
		}

		read_number++;
	}

	fclose(fileptr);
}

// Function designed for chat between client and server.
void func(int connfd, struct sockaddr_in cliaddr, int len)
{
	char buff[MAX];
	int n;
	// infinite loop for chat
	for (;;) {
		bzero(buff, MAX);

		// read the message from client and copy it in buffer

        n = recvfrom(connfd, (char *)buff, MAXLINE, 
                MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                &len);
        buff[n] = '\0';
        printf("%s\n",buff);
		printf("From client: %s\t To client : ", buff);

        if (atoi(buff) == 1)
        {
            printf("%s","Candidate\n");
            printf("\nClient chose 1 \n");
            bzero(buff, MAX);
            n = 0;
            strcpy(buff, "1");
            sendto(connfd, (const char *)buff, strlen(buff), 
            MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
                len);
            register_candidate(connfd, cliaddr,len);
        }

        else if (atoi(buff) == 2)
        {
            bzero(buff, MAX);
            n = 0;
            // and send that buffer to client
            strcpy(buff, "2");
            //write(connfd, buff, sizeof(buff));
            sendto(connfd, (const char *)buff, strlen(buff), 
            MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
                len);
            registerVoter(connfd, cliaddr,len);
        }

        else if (atoi(buff) == 3)
        {
            bzero(buff, MAX);
            n = 0;
            // and send that buffer to client
            strcpy(buff, "3");
            //write(connfd, buff, sizeof(buff));
            sendto(connfd, (const char *)buff, strlen(buff), 
            MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
                len);
            vote_for_candidates(connfd, cliaddr,len);
        }

        else if (atoi(buff) == 4)
        {
            bzero(buff, MAX);
            n = 0;
            // and send that buffer to client
            strcpy(buff, "4");
            //write(connfd, buff, sizeof(buff));
            sendto(connfd, (const char *)buff, strlen(buff), 
            MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
                len);
            tally_votes(connfd, cliaddr,len);
        }

		// if msg contains "Exit" then server exit and chat ended.
		if (strncmp("exit", buff, 4) == 0) {
			printf("Server Exit...\n");
			break;
		}
	}
}

   
// Driver code
int main() {
    int sockfd;
    char buffer[MAXLINE];
    char *hello = "Hello from server";
    struct sockaddr_in servaddr, cliaddr;
       
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
       
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
       
    // Filling server information
    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
       
    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, 
            sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
       
    int len, n;
   
    len = sizeof(cliaddr);  //len is value/result
   
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
                MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                &len);
    buffer[n] = '\0';
    printf("Client : %s\n", buffer);
    sendto(sockfd, (const char *)hello, strlen(hello), 
        MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
            len);
    printf("Connected to client.\n"); 

    func(sockfd, servaddr, len);
       
    return 0;
}