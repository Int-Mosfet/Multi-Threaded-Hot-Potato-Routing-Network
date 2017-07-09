#include  "Message.h"
#include  "Data.h"
#include  "Thread.h"
#include	"Neighbor.h"
#include  "Arg_Converter.h"
#include	"Common.h"	//This header holds values that needs to be accessed by all files
#include  "Current_Time.h"
#include	<unistd.h>
#include	<stdlib.h>		//srand, rand
#include 	<stdio.h>
#include	<ctime>
#include	<stdint.h>
#include	<fstream>
#include  <signal.h>
#include  <iostream>
#include  <string>
#include  <cmath>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */



#define BUF_SIZE 10

//Global Variables
int cnt = 0;
Current_Time tme;
std::ofstream myfile("time_vals.txt");
int num_nodes;
//Total Blocking Time
float t_f;
//(T)     run time of program (in seconds)
int run_time;

//Prototypes
void wakeup(int);
void sigint_handler(int);

//decides whether to print to stdout or
//to a file
bool output_flag = false;
//output file
std::ofstream outfile;
Array<Neighbor*> neighbor_array;
int main (int argc, char * argv [])
{
	bool input_flag = false;
	bool time_flag = false;
	bool check = true;
	bool output_flag = false;

	double con_mean_sleep_time = 0.0;
	double prod_mean_sleep_time = 0.0;

	int cap_of_buf,num_of_nodes = 0;
	int value = 0;
	int node = 0;
	int prev_node = 0;
	int rev_n = 0;
	int index = 0;


	std::string line;
	std::string token;
	std::string answer;
	std::string input_;
	std::string input_file_name;
	std::string output_;
	std::string output_file_name;
	std::string time_;
	std::string time_number;

	std::vector<int> neighbor_vect;
	std::vector<int> reverse_vect;
	 srand (time(NULL));


	if(argc == 10)
	{
		input_ = convert<std::string>(argv[1]);
		input_file_name = convert<std::string>(argv[2]);
		output_ = convert<std::string>(argv[3]);
		output_file_name = convert<std::string>(argv[4]);
		time_ = convert<std::string>(argv[5]);
		time_number = convert<std::string>(argv[6]);
    con_mean_sleep_time = convert<double>(argv[7]);
    prod_mean_sleep_time = convert<double>(argv[8]);
    cap_of_buf = convert<int>(argv[9]);
	}
	else
	{
		std::cout << "Input format: ./exe_name -i file.gl -o file.txt -t 10" << std::endl;
		return -1;
	}

	if(input_ == "-i")
	{
		std::cout << "input file" << std::endl;
		input_flag = true;
	}
	if(output_ == "-o")
	{
		std::cout << "output_file" << std::endl;
		output_flag = true;
		//create output file
		outfile.open(output_file_name);
	}
	if(time_ == "-t")
	{
		std::cout << "time" << std::endl;
		time_flag = true;
	}
	/*Creating an interrupt to exit
  program after runtime is up*/
	signal(SIGALRM,wakeup);
	/*Install SIGINT handler*/
	signal(SIGINT,sigint_handler);
	alarm(stoi(time_number));

	/*This is the workspace that all the threads will
	work in*/
	pthread_mutex_t the_mutex = PTHREAD_MUTEX_INITIALIZER;

	//This is an array of unsigned integers
	Array<pthread_t> thread_space;
	Array<Mailbox*> mailbox_array;

	Neighbor* neigh;
	Neighbor* reverse_neigh;

	std::ifstream myfile(input_file_name);
	if(myfile.is_open())
	{
		while(getline(myfile, line))
		{
			std::istringstream input(line);
			input >> token;//Gives N or E or D

			if(token == "D")
				std::cout << "what does D do?" << std::endl;
			else if(token == "N")
			{
				/*Increment the size and add a neighbor,
				then provide a thread space for the nodes
				to run their process, then create a mailbox
				for each node to give and receive messages*/
				neighbor_array.resize(neighbor_array.size()+1);
				neighbor_array.set(index,new Neighbor);

				thread_space.resize(thread_space.size()+1);
				thread_space.set(index,index);//maybe ids can go here

				mailbox_array.resize(mailbox_array.size()+1);
				mailbox_array.set(index++,new Mailbox(cap_of_buf));

			}//end if token is N
			else if(token == "E")
			{
				//Get the node id
				input >> token;
				node = std::stoi(token);
				//Get the neighbor of that node
				input >> token;
				value = std::stoi(token);
				neighbor_array[node]->set_neighbor(value);
				neighbor_array[value]->set_neighbor(node);
			}//end token is E
		}//end while getline
		myfile.close();
	}
	else
	{
		std::cout << "ERROR! [File not found]" << std::endl;
	}//end getline function

	num_nodes = neighbor_array.size();
	std::cout << "Number Of Nodes " << num_nodes << std::endl;	

	for(int i = 0; i < neighbor_array.size(); i++)
	{
		Thread nodes(mailbox_array,neighbor_array,outfile,the_mutex);
		nodes.set_sleep(con_mean_sleep_time,prod_mean_sleep_time);
		nodes.create_node(thread_space,thread_space.get(i),output_flag);
	}
	//Wait for threads to finish
	while(1){}
	//close output file **IF OUTPUT FILE IS OPEN THEN CLOSE***
	outfile.close();
  return 0;
}

void wakeup(int signum)
{
  std::cout<<"Times Up!!"<<std::endl<<"Total Execution Time: "<<run_time
	<<"sec(s)"<<std::endl;
  std::cout<<"Total Blocking Time: "<<t_f<<"sec(s)"<<std::endl;
  tme.mean = tme.time_diff/1;//cnt;
  std::cout<<"The Mean in seconds: "<<tme.mean<<std::endl;
	myfile.close();
	std::ifstream myfile;
	myfile.open("time_vals.txt");

  //Reset Variance for calculation
  tme.variance = 0;
	while (!myfile.eof())
	{
		myfile >> tme.xi;
		tme.variance += ((tme.xi - tme.mean) * (tme.xi - tme.mean));
	}
	myfile.close();
	tme.dev = sqrt(tme.variance);
	std::cout << "The Standard Deviation:" << tme.dev <<std::endl;
  exit(signum);
}

void sigint_handler(int signum)
{
	char buffer[BUF_SIZE];
	char c_PID[BUF_SIZE];
	/*Clear screen and disable stdout*/
	system("clear");
	//get underlying buffer
	std::streambuf* orig_buf = std::cout.rdbuf();
	/*c_PID gets string version of pid*/
	sprintf(c_PID, "%d", getpid());

	std::cout << std::endl;
	std::cout << "Type one of the commands below" << std::endl;
	std::cout << "'info' : Print Program Statistics" << std::endl;
	std::cout << "'resume' : Continue Displaying Logs" << std::endl;
	std::cout << "'neighbor' : Display neighbors of node" << std::endl;
	std::cout << "'quit' : Finish Program Execution (exit)" <<std::endl;

	/*Disable output*/
	std::cout.rdbuf(NULL);

	std::cin  >> buffer;

	/*Check user input*/
	if(strcmp(buffer,"info") == 0)
	{
		/*Clear screen and enable output*/
		system("clear");
		std::cout.rdbuf(orig_buf);

		std::cout << "Program Statistics (press enter to exit):\n\n";

		tme.mean = tme.time_diff/1;//cnt;

		std::ifstream myfile;
		myfile.open("time_vals.txt");

		/*Reset Variance for calculation*/
		tme.variance = 0;

		while (!myfile.eof())
		{
			myfile >> tme.xi;
			tme.variance += ((tme.xi - tme.mean) * (tme.xi - tme.mean));
		}
		myfile.close();
		tme.dev = sqrt(tme.variance);

		std::cout << "	Mean Time Difference: " << tme.mean << " second(s)\n";
		std::cout << "	Deviation of Time Difference: " << tme.dev << " nsecond(s)\n";

		/*Disable output*/
		std::cout.rdbuf(NULL);

		/*wait for [enter]*/
		getchar();
		getchar();
	}//end if strcmp
	else if(strcmp(buffer,"resume") == 0)
	{
		/*continue displaying logs*/
	}
	/*Enter the neighbor menu*/
	else if(strcmp(buffer,"neighbor") == 0)
	{
		/*Enable output*/
		std::cout.rdbuf(orig_buf);

		std::cout << "howdy neighbor (enter to exit)" << std::endl;

		/*Print all neighbors of all nodes*/
		for(int i = 0; i < neighbor_array.size(); i++)
		{
			std::cout << "Node " << i << " has neighbor(s): ";

			for(int j = 0; j < neighbor_array[i]->size(); j++)
			{
				std::cout << neighbor_array[i]->get_neighbor(j)<<", ";
			}
			std::cout << std::endl;
		}
		/*Disable output*/
		std::cout.rdbuf(NULL);

		/*wait for [enter]*/
		getchar();
		getchar();
	}
	else if(strcmp(buffer,"quit") == 0)
	{
		/*Enable output*/
		std::cout.rdbuf(orig_buf);

		/*finish program execution (exit)*/
		std::cout << "Exiting.." << std::endl;

		/*kill this process*/
		memset(buffer,0,sizeof(buffer));
		strcpy(buffer,"kill ");
		strcat(buffer,c_PID);
		system(buffer);
	}
	else
	{
		/*Enable output*/
		std::cout.rdbuf(orig_buf);

		std::cout << "Invalid Input.. Resuming" << std::endl;

		/*Disable output*/
		std::cout.rdbuf(NULL);

		/*wait 1 sec to display message*/
		sleep(1);
	}

	/*Clear screen and enable output*/
	system("clear");
	std::cout.rdbuf(orig_buf);
}
