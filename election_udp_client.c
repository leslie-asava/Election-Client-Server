// Client side implementation of UDP client-server model
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
int register_candidate(int sockfd, struct sockaddr_in servaddr, int len);
void func(int sockfd, struct sockaddr_in servaddr, int len);
char *remove_new_line(char *strbuffer);
void registerVoter(int sockfd, struct sockaddr_in servaddr, int len);
int tally_votes(int sockfd, struct sockaddr_in servaddr, int len);
void vote_for_candidates(int sockfd, struct sockaddr_in servaddr, int len);
void vote_position(char position[30], int sockfd, struct sockaddr_in servaddr, int len);

// Struct to hold voter data
struct voter
{
    char fname[50];
    char lname[50];
    char reg_no[30];
    char password[30];
}info;

void send_data(int sockfd, struct sockaddr_in servaddr, int len, char *buff)
{
    sendto(sockfd, (const char *)buff, strlen(buff), 
    MSG_CONFIRM, (const struct sockaddr *) &servaddr,
        len);
}

char *receive_data(int sockfd, struct sockaddr_in servaddr, int len, char *buff)
{
    bzero(buff, MAX);

    int n;

    // read the message from client and copy it in buffer

    n = recvfrom(sockfd, (char *)buff, MAXLINE, 
            MSG_WAITALL, ( struct sockaddr *) &servaddr,
            &len);
    buff[n] = '\0';

	return buff;
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

int register_candidate(int sockfd, struct sockaddr_in servaddr, int len)
{
	char fname[50];
	char lname[50];
	char reg[20];
	char buff[MAX];

	printf("\n\n<<Welcome to SONU Elections Candidates Registration>>\n\n");

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

	int i = 0;
	for (i = 0; i < positions; i++)
	{
		printf("\t%d. %s \n", i + 1, position_titles[i]);
	}

	int choice, n;
	printf("\n\nPlease enter your choice on the position you wish to vie for :: ");
	scanf("%d", &choice);

	snprintf(buff, sizeof(buff), "%d", choice);

    // Send data
    sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));      
    // Receive data and save to buffer
    n = recvfrom(sockfd, (char *)buff, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    buff[n] = '\0';

	printf("Enter your First name \t:  ");
	scanf("%s", fname);

	strcpy(buff, fname);
    // Send data to server
	sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
	// Receive data and save to buffer
    bzero(buff, sizeof(buff));
    n = recvfrom(sockfd, (char *)buff, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    buff[n] = '\0';

	printf("Enter your Last name \t:  ");
	scanf("%s", lname);

	strcpy(buff, lname);
	// Send data to server
	sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));	
    // Receive data and save to buffer
    bzero(buff, sizeof(buff));
    n = recvfrom(sockfd, (char *)buff, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    buff[n] = '\0';

	printf("Enter your Reg no \t:  ");
	scanf("%s", reg);

	strcpy(buff, reg);
	// Send data to server
	sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    // Receive data and save to buffer
    bzero(buff, sizeof(buff));
    n = recvfrom(sockfd, (char *)buff, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    buff[n] = '\0';

	printf("\nSuccesfully Registered :: %s %s %s", fname, lname, reg);

	return 0;
}

void registerVoter(int sockfd, struct sockaddr_in servaddr, int len)
{
    int choice;
    char name[100];
    char pwd_comp[30];
    char file_path[100] = "";
    char dir[] = "voters/";
    int cmp;
	char buff[MAX];
    int n;
 
    printf("\tVOTER REGISTRATION\n");
    printf("\nFIRST NAME : ");
    scanf("%s", info.fname);

	strcpy(buff, info.fname);
	sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
	bzero(buff, sizeof(buff));
	n = recvfrom(sockfd, (char *)buff, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    buff[n] = '\0';

    printf("\nLAST NAME : ");
    scanf("%s", info.lname);

	strcpy(buff, info.lname);
	sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
	bzero(buff, sizeof(buff));
	n = recvfrom(sockfd, (char *)buff, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    buff[n] = '\0';

    printf("\nDO NOT USE '/' IN YOUR REG NO\nREGISTRATION NUMBER : ");
    scanf("%s", info.reg_no);

	strcpy(buff, info.reg_no);
	sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
	bzero(buff, sizeof(buff));
	n = recvfrom(sockfd, (char *)buff, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    buff[n] = '\0';
 
 
    int loc_file = atoi(buff);
    if( loc_file == 0)
    {
        printf("\nPASSWORD : ");
        scanf("%s", info.password);

		strcpy(buff, info.password);
		sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
        bzero(buff, sizeof(buff));
        n = recvfrom(sockfd, (char *)buff, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
        buff[n] = '\0';

        printf("\nCONFIRM PASSWORD : ");
        scanf("%s", pwd_comp);

		strcpy(buff, pwd_comp);
		sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
        bzero(buff, sizeof(buff));
        n = recvfrom(sockfd, (char *)buff, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
        buff[n] = '\0';

		strcpy(buff, "ACCEPT");
		sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
        bzero(buff, sizeof(buff));
        n = recvfrom(sockfd, (char *)buff, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
        buff[n] = '\0';
		printf("%s", buff);
       
    }
    else if(loc_file == -1)
    {
        //printf("account already exist !!!!\n");
		strcpy(buff, "ACCEPT");
		sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
        bzero(buff, sizeof(buff));
        n = recvfrom(sockfd, (char *)buff, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
        buff[n] = '\0';
		printf("%s", buff);
    }
 
}

// Display results
int tally_votes(int sockfd, struct sockaddr_in servaddr, int len)
{
	char buff[MAX];
    int n;

	printf("\n\t\t\tTALLYING\n\t\tThis will take a second");

	strcpy(buff, "READY\n");
	sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
	bzero(buff, sizeof(buff));
	n = recvfrom(sockfd, (char *)buff, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    buff[n] = '\0';
	printf("%s",buff);

	while (strncmp("exit", buff, sizeof(buff)) != 0) {

		int i = 0;
		strcpy(buff, "READY");
		sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
		bzero(buff, sizeof(buff));
		n = recvfrom(sockfd, (char *)buff, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
        buff[n] = '\0';

		printf("%s",buff);
	}

}

void vote_position(char position[30], int sockfd, struct sockaddr_in servaddr, int len)
{
	printf("\n\t\t<<< Position: %s >>>\n\n\t\t\tCandidates\n", position);
	char buff[MAX];
    int n;

	while (strncmp("exit", buff, sizeof(buff)) != 0) {

		int i = 0;
		strcpy(buff, "READY");
		sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
		bzero(buff, sizeof(buff));
		n = recvfrom(sockfd, (char *)buff, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
        buff[n] = '\0';

		if (strncmp("ACCEPT", buff, sizeof(buff)) != 0 && strncmp("exit", buff, sizeof(buff)) != 0)
		{
			printf("%s",buff);
		}
	}

	// Vote confirmation variables
	char confirm[1];
	char read_id[30] = { "0" };
	int candidate_selection;

	while (true)
	{
		printf("\n\n\t\t[#] Your selection: ");
		scanf("%s", read_id);

		strcpy(buff, read_id);
		sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
		//printf("%s",buff);
        bzero(buff, sizeof(buff));
		n = recvfrom(sockfd, (char *)buff, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
        buff[n] = '\0';

		/*if (atoi(read_id) > get_number_of_candidates(path))
		{
			continue;
		}*/

		printf("\n\t\tYour %s selection: ", position);
		//print_candidate_on_line(atoi(read_id), path);
		printf("%s \n", buff);

		printf("\n\n\t\t[--] Confirm[y/n]: ");
		scanf("%s", confirm);

		strcpy(buff, confirm);
		sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
		//printf("%s",buff);
		bzero(buff, sizeof(buff));
		n = recvfrom(sockfd, (char *)buff, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
        buff[n] = '\0';
		break;

	}
}


// Voting function
void vote_for_candidates(int sockfd, struct sockaddr_in servaddr, int len)
{
	char regInput[20];
	char buff[MAX];
	char input_name[20];
	char input_pass[20];
    int n;

	// Authenticate voter
	printf("\n\nEnter your registration number: ");
	scanf("%s", regInput);

	strcpy(buff, regInput);
	sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
	//printf("%s",buff);
	bzero(buff, sizeof(buff));
	n = recvfrom(sockfd, (char *)buff, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    buff[n] = '\0';
	//printf("%s",buff);

	bool authenticated = false;

	if (!authenticated)
	{

		printf("\n\t\tVOTER AUTHENTICATION\n\t\tThis should take a second :)\n");
		printf("\n\t[--] Your name: ");
		scanf("%s", input_name);

		strcpy(buff, input_name);
		sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
		bzero(buff, sizeof(buff));
		n = recvfrom(sockfd, (char *)buff, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
        buff[n] = '\0';

		printf("\t[--] Your passphrase: ");
		scanf("%s", input_pass);

		strcpy(buff, input_pass);
		sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
        bzero(buff, sizeof(buff));
		n = recvfrom(sockfd, (char *)buff, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
        buff[n] = '\0';

		if (strstr(buff, "Success") == NULL)
		{
			printf("\n%s", buff);
			return;
		}

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

	printf("\n\t\tVOTING\n\t\tMake your best selections\n");
	for (i = 0; i < positions; i++)
	{
		// Display candidates to choose from
		vote_position(position_titles[i], sockfd, servaddr, len);
	}
}

void func(int sockfd, struct sockaddr_in servaddr, int len)
{
	char buff[MAX];
	int n;
	for (;;) {
		bzero(buff, sizeof(buff));
		char entryOptions[5][100] = { "1. Register as a candidate",
			"2. Register as a voter",
			"3. Vote",
			"4. Announce results",
		};

		int entryChoice = 0;

		printf("Welcome to the UNSA Elections Voting System. \n\nWhat would you like to do? \n");

		while (entryChoice == 0)
		{
			int i = 0;
			for (i = 0; i < 4; i++)
			{
				printf("%s", entryOptions[i]);
				printf("\n");
			}

			printf(">> ");
			scanf("%d", &entryChoice);

			if (entryChoice < 1 || entryChoice > 4)
			{
				entryChoice = 0;
				printf("Please enter a valid choice\n");
			}
		}	

		n = 0;
		//while ((buff[n++] = getchar()) != '\n')
		//	;
		snprintf(buff, sizeof(buff), "%d", entryChoice);

        sendto(sockfd, (const char *)buff, strlen(buff),
        MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
            sizeof(servaddr));
        //printf("Hello message sent.\n");
            
        n = recvfrom(sockfd, (char *)buff, MAXLINE, 
                    MSG_WAITALL, (struct sockaddr *) &servaddr,
                    &len);
        buff[n] = '\0';
        printf("Server : %s\n", buff);

		if (atoi(buff) == 1)
        {
            register_candidate(sockfd, servaddr, len);
        }

        else if (atoi(buff) == 2)
        {
            registerVoter(sockfd, servaddr, len);
        }

        else if (atoi(buff) == 3)
        {
            vote_for_candidates(sockfd, servaddr, len);
        }

        else if (atoi(buff) == 4)
        {
            tally_votes(sockfd, servaddr, len);
            
			
        }

		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("Client Exit...\n");
			break;
		}
	}
}

   
// Driver code
int main() {
    int sockfd;
    char buffer[MAXLINE];
    char *hello = "Hello from client";
    struct sockaddr_in     servaddr;
   
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
   
    memset(&servaddr, 0, sizeof(servaddr));
       
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;
       
    int n, len;
       
    sendto(sockfd, (const char *)hello, strlen(hello),
        MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
            sizeof(servaddr));
    printf("Connected to server.\n");
           
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
                MSG_WAITALL, (struct sockaddr *) &servaddr,
                &len);
    buffer[n] = '\0';
    printf("Server : %s\n", buffer);

    // function for election
	func(sockfd, servaddr, len);
   
    close(sockfd);
    return 0;
}