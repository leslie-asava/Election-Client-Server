#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>

#define MAX 300
#define PORT 8080
#define SA struct sockaddr

// Detect OS and define clearing command
#if defined(_WIN32)
	#define CLEAR_COMMAND "cls"		// Windows
#else
	#define CLEAR_COMMAND "clear"	// Other OS
# endif

struct voter
{
    char fname[50];
    char lname[50];
    char reg_no[30];
    char password[30];
}info;

// Function Declaration
char *remove_new_line(char *strbuffer);
void vote_position(char position[30], int sockfd);
void vote_for_candidates(int sockfd);
void registerVoter(int sockfd);
int tally_votes(int sockfd);
int register_candidate(int sockfd);
void func(int sockfd);

char *remove_new_line(char *strbuffer)
{
	size_t len = strlen(strbuffer);
	if (strbuffer[len - 1] == '\n')
	{
		strbuffer[len - 1] = '\0';
	}

	return strbuffer;
}

void vote_position(char position[30], int sockfd)
{
	printf("\n\t\t<<< Position: %s >>>\n\n\t\t\tCandidates\n", position);
	char buff[MAX];

	while (strncmp("exit", buff, sizeof(buff)) != 0) {

		int i = 0;
		strcpy(buff, "READY");
		write(sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));

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
		write(sockfd, buff, sizeof(buff));
		//printf("%s",buff);
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));

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
		write(sockfd, buff, sizeof(buff));
		//printf("%s",buff);
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
		break;

	}
}

void vote_for_candidates(int sockfd)
{
	char regInput[20];
	char buff[MAX];
	char input_name[20];
	char input_pass[20];
	// Authenticate voter
	printf("\n\nEnter your registration number: ");
	scanf("%s", regInput);

	strcpy(buff, regInput);
	write(sockfd, buff, sizeof(buff));
	//printf("%s",buff);
	bzero(buff, sizeof(buff));
	read(sockfd, buff, sizeof(buff));
	//printf("%s",buff);

	bool authenticated = false;

	if (!authenticated)
	{

		printf("\n\t\tVOTER AUTHENTICATION\n\t\tThis should take a second :)\n");
		printf("\n\t[--] Your name: ");
		scanf("%s", input_name);

		strcpy(buff, input_name);
		write(sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));

		printf("\t[--] Your passphrase: ");
		scanf("%s", input_pass);

		strcpy(buff, input_pass);
		write(sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));

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
		vote_position(position_titles[i], sockfd);
	}
}



void registerVoter(int sockfd)
{
    int choice;
    char name[100];
    char pwd_comp[30];
    char file_path[100] = "";
    char dir[] = "voters/";
    int cmp;
	char buff[MAX];
 
    printf("\tVOTER REGISTRATION\n");
    printf("\nFIRST NAME : ");
    scanf("%s", info.fname);

	strcpy(buff, info.fname);
	write(sockfd, buff, sizeof(buff));
	bzero(buff, sizeof(buff));
	read(sockfd, buff, sizeof(buff));

    printf("\nLAST NAME : ");
    scanf("%s", info.lname);

	strcpy(buff, info.lname);
	write(sockfd, buff, sizeof(buff));
	bzero(buff, sizeof(buff));
	read(sockfd, buff, sizeof(buff));

    printf("\nDO NOT USE '/' IN YOUR REG NO\nREGISTRATION NUMBER : ");
    scanf("%s", info.reg_no);

	strcpy(buff, info.reg_no);
	write(sockfd, buff, sizeof(buff));
	bzero(buff, sizeof(buff));
	read(sockfd, buff, sizeof(buff));
 
 
    int loc_file = atoi(buff);
    if( loc_file == 0)
    {
        printf("\nPASSWORD : ");
        scanf("%s", info.password);

		strcpy(buff, info.password);
		write(sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));

        printf("\nCONFIRM PASSWORD : ");
        scanf("%s", pwd_comp);

		strcpy(buff, pwd_comp);
		write(sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));

		strcpy(buff, "ACCEPT");
		write(sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
		printf("%s", buff);
       
    }
    else if(loc_file == -1)
    {
        //printf("account already exist !!!!\n");
		strcpy(buff, "ACCEPT");
		write(sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
		printf("%s", buff);
    }
 
}

int tally_votes(int sockfd)
{
	char buff[MAX];
	printf("\n\t\t\tTALLYING\n\t\tThis will take a second");

	strcpy(buff, "READY\n");
	write(sockfd, buff, sizeof(buff));
	bzero(buff, sizeof(buff));
	read(sockfd, buff, sizeof(buff));
	printf("%s",buff);

	while (strncmp("exit", buff, sizeof(buff)) != 0) {

		int i = 0;
		strcpy(buff, "READY");
		write(sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));

		printf("%s",buff);
	}

}

int register_candidate(int sockfd)
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

	int choice;
	printf("\n\nPlease enter your choice on the position you wish to vie for :: ");
	scanf("%d", &choice);

	snprintf(buff, sizeof(buff), "%d", choice);
	write(sockfd, buff, sizeof(buff));
	bzero(buff, sizeof(buff));
	read(sockfd, buff, sizeof(buff));

	printf("Enter your First name \t:  ");
	scanf("%s", fname);

	strcpy(buff, fname);
	write(sockfd, buff, sizeof(buff));
	bzero(buff, sizeof(buff));
	read(sockfd, buff, sizeof(buff));

	printf("Enter your Last name \t:  ");
	scanf("%s", lname);

	strcpy(buff, lname);
	write(sockfd, buff, sizeof(buff));
	bzero(buff, sizeof(buff));
	read(sockfd, buff, sizeof(buff));

	printf("Enter your Reg no \t:  ");
	scanf("%s", reg);

	strcpy(buff, reg);
	write(sockfd, buff, sizeof(buff));
	bzero(buff, sizeof(buff));
	read(sockfd, buff, sizeof(buff));

	printf("\nSuccesfully Registered :: %s %s %s", fname, lname, reg);

	return 0;
}

void func(int sockfd)
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
		write(sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
		printf("From Server : %s", buff);

		if (atoi(buff) == 1)
        {
            register_candidate(sockfd);
        }

        else if (atoi(buff) == 2)
        {
            registerVoter(sockfd);
        }

        else if (atoi(buff) == 3)
        {
            vote_for_candidates(sockfd);
        }

        else if (atoi(buff) == 4)
        {
            tally_votes(sockfd);
			
        }

		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("Client Exit...\n");
			break;
		}
	}
}

int main()
{
	int sockfd, connfd;
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
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	// function for election
	func(sockfd);

	// close the socket when done
	close(sockfd);
}

