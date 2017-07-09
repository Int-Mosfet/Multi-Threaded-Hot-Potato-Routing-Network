#include "Thread.h"
Thread::Thread(Array<Mailbox*>& mail,Array<Neighbor*>& neigh,std::ofstream& of,pthread_mutex_t& mutex)
  :mailbox(mail)
  ,my_neighbor(neigh)
  ,KeepGoing(true)
  ,prod_mean_sleep_time(0)
  ,con_mean_sleep_time(0)
  ,nodeID(0)
  ,the_mutex(mutex)
  ,c_cons(PTHREAD_COND_INITIALIZER)
  ,p_prod(PTHREAD_COND_INITIALIZER)
  ,outfile(of)
{

}

void* Thread::producer(void* ptr)
{
  ((Thread*)ptr)->prod(((Thread*)ptr)->nodeID);
  return NULL;
}

void Thread::prod(pthread_t ID)
{
  Message* msg;
  //The nodes ID
  info.src = ID;
  info.time_to_live = DEFAULT_TTL;
  info.hop_count = 0;
  info.type = 0;
  info.rsrvd = 0;
  info.tx = ID;//The transmitters ID
  info.dst = 0xFFFFFFFF;
  info.rx = 0xFFFFFFFF;

	while(KeepGoing)
	{
    //insert into the buffer
    pthread_mutex_lock(&the_mutex);
    while(mailbox[info.src]->isFull())
    {
      //Tell the producer to wait
      pthread_cond_wait(&p_prod,&the_mutex);
    }

    //This points to the data struct
    //Put the message in the neighbor's mailbox
    extern int num_nodes;

		if((tme_.curr_time_sec() - begin) % 5 == 0)
		{
      //Prevents from wanting to ping itself
      do
      {
        random_neighbor = rand() % num_nodes;
      }while(random_neighbor == ID);

			std::cout <<ID<<" wants to send a Ping to " << random_neighbor << std::endl;

//----------- CREATE A NEW MESSAGE ---------------------------------------------
      msg = new Message;
      info.tm = tme_.curr_time_sec();
  		msg->bytes = sizeof(double);
      msg->data = new double(msg->bytes);
      //Put a value in the mailbox
      *((double*)msg->data) = random_uniform();
      /*Setting the destination value for
      where the message should be sent to*/
			info.dst = random_neighbor;
      /*Select a neighbor that will forward
      the message next*/
      info.rx = neighbor_id(ID);
      /*Provide the transmitters ID
      so the nodes will know where it came from*/
      info.tx = ID;
			info.type = 4;//ping
      info.src = ID;
      info.time_to_live = DEFAULT_TTL;
      info.hop_count = 0;
      std::cout<<info.tx<<" is telling "<<info.rx<<" to forward the message.\n";
      msg->data2 = &info;
//------------------------------------------------------------------------------
      //Send the message into the network
  		if(output_flag == false)
  		{
  		std::cout << mailbox[info.rx]->put(msg) << " "<<info.tx<< " post ";
  		std::cout << tme_.curr_time_sec() << "." << tme_.curr_time_nsec();
  		std::cout << std::endl;
  		}
  		else if(output_flag == true)
  		{
  			 outfile << mailbox[info.rx]->put(msg) << " "<<info.tx<< " post ";
     		 outfile << tme_.curr_time_sec() << "." << tme_.curr_time_nsec();
     		 outfile << std::endl;
  		}
		}//end ping statement
    //This sends a hello message to a node
    else if(( tme_.curr_time_sec() - begin) % 20 == 0)
    {
      random_neighbor = rand() % num_nodes;
      std::cout <<ID<<" is sending a Hello message to " <<random_neighbor<< std::endl;
      info.type = 1; //Hello type
    }//end hello statement
		pthread_mutex_unlock(&the_mutex);
    //Signals the consumer to access the mutex
    pthread_cond_signal(&c_cons);
		nsleep(random_exponential(prod_mean_sleep_time));
	}//end while
    pthread_exit(0);
}

void* Thread::consumer(void* ptr)
{
  ((Thread*)ptr)->cons(((Thread*)ptr)->nodeID);
  return nullptr;
}

void Thread::cons(pthread_t ID)
{
	Message* msg;
  Data* my_data;//is for self referencing
  bool skip_flg = false;
  time_t time_diff = 0;

	while (KeepGoing)
	{
    pthread_mutex_lock(&the_mutex);
		while(mailbox[ID]->isEmpty())
    {
      pthread_cond_wait(&c_cons,&the_mutex);
    }
    std::cout<<ID<<"My mailbox has something in it\n";
    //Receiving the message
    msg = mailbox[ID]->get();
    my_data = ((Data*)(msg->data2));
    /*The amount of time the msg can live
    before getting destoryed*/
    if(my_data->time_to_live > 0)
    {
      my_data->time_to_live--;
      my_data->hop_count++;
    }
    /*Calcualte the RTT if the msg reached
    the destination*/
    if(my_data->type == 5 && my_data->dst == ID)
    {
      time_diff = tme_.curr_time_sec() - my_data->tm;
      std::cout<<"The RTT: "<<time_diff<<std::endl;
    }
    /*Checking to make sure that the nodes
    is not receiving a hello message or given
    the responsibility to forward the message*/
    else if((my_data->rx != 0xFFFFFFFF && my_data->rx != ID) ||
    my_data->time_to_live == 0)
    {
      std::cout<<ID<<" is Ignoring message: "<<*((double*)msg->data)<<std::endl;
      std::cout<<"Sent by "<<my_data->tx<<std::endl;
      msg_drops++;
      delete msg;
      msg = nullptr;
    }
    //Handling The hello messages
    else if(my_data->rx == 0xFFFFFFFF && my_data->dst == 0xFFFFFFFF)
    {
      std::cout <<my_data->src<<" Says Hello, World!" << std::endl;
    }
    //Accept the message if the ID match the correct destination.
    else if(my_data->dst == ID && my_data->type == 4)
    {
      //Compute the time difference
      time_diff = (tme_.curr_time_sec() - my_data->tm);
      std::cout<<"Time difference: "<<time_diff<<"(s)"<<std::endl;
      std::cout<<"The hop count: "<<my_data->hop_count<<std::endl;
    	if(output_flag == false)
    	{
        std::cout << *((double*)msg->data)<<" "<<my_data->dst<<" recv ";
        std::cout << tme_.curr_time_sec() << "." << tme_.curr_time_nsec();
        std::cout << std::endl;
    	}
    	else if (output_flag == true)
    	{
    		 outfile << *((double*)msg->data)<<" "<<my_data->dst<<" recv ";
    		 outfile << tme_.curr_time_sec() << "." << tme_.curr_time_nsec();
    		 outfile << std::endl;
    	}
      //Send a ping back to the source.
      reply(my_data);
    }//end dst == ID
    //Keep the message going through the network.
    else if(my_data->dst != ID)
      relay_to_neighbor(ID,msg);

    pthread_mutex_unlock(&the_mutex);
    pthread_cond_signal(&p_prod);
    nsleep(random_exponential(con_mean_sleep_time));
	}
	pthread_exit(0);
}

int Thread::create_node(Array<pthread_t>& thread_space,pthread_t id,bool flag)
{
  set_id(thread_space[id]);
  set_outputflag(flag);
  // Initialize the mutex and condition variables for producer and consumer
  pthread_mutex_init(&the_mutex, NULL);
//  pthread_cond_init(&c_cons, NULL);
//	pthread_cond_init(&p_prod, NULL);
  // Create threads
  pthread_create(&thread_space[id], NULL, Thread::producer,this);
  pthread_create(&thread_space[id], NULL, Thread::consumer,this);
  return 0;
}

void Thread::set_sleep(double cs, double ps)
{
  prod_mean_sleep_time = ps;
  con_mean_sleep_time = cs;
}

void Thread::set_id(pthread_t id)
{
  nodeID = id;
}

int Thread::neighbor_id(int my_id)
{
  /*This function will return the neighbor ID based off
  the nodes ID number*/
  Neighbor* neigh = my_neighbor[my_id];
  return neigh->get_neighbor(random_value(neigh->size()));
}

void Thread::set_outputflag(bool flag)
{
  output_flag = flag;
}

void Thread::reply(Data* my_data)
{
  Message* msg;
  pthread_t id = my_data->dst;

  my_data->type = 5;
  my_data->dst = my_data->src;
  my_data->time_to_live = DEFAULT_TTL;
  my_data->src = id;

  msg = new Message;
  msg->bytes = sizeof(double);
  msg->data = new double(msg->bytes);

  /*Select a neighbor that will forward
  the message next*/
  my_data->rx = neighbor_id(id);
  std::cout<<id<<" is telling "<<my_data->rx<<" to send back the message.\n";
  /*Provide the transmitters ID
  so the nodes will know where it came from*/
  my_data->tx = ID;

  //This points to the data struct
  //Put the message in the neighbor's mailbox
  msg->data2 = &info;
  /*Reply to the node that sent
  the message to this node*/
  mailbox[my_data->dst]->put(msg);

  if(output_flag == false)
  {
  std::cout <<"receiver "<<ID<<" replied "<<my_data->type<<" to node "<<my_data->dst;
  std::cout<<" Time: "<<tme<<std::endl;
  }
  else if(output_flag == true)
  {
    outfile <<"receiver "<<ID<<" replied "<<my_data->type<<"to node "<<my_data->dst;
    outfile<<" Time: "<<tme<<std::endl;
  }
}
void Thread::relay_to_neighbor(pthread_t my_id, Message* msg)
{
  int id;
  Data* my_data;
  double my_msg;

  my_data = ((Data*)(msg->data2));
  my_msg = *((double*)(msg->data));
  /*This makes sure that we do not
  send the message back to the person who
  sent it*/
  do
  {
    //Forwarding message to one of this nodes neighbors.
    id = ((Data*)(msg->data2))->rx = neighbor_id(my_id);
  }while(id == ((Data*)(msg->data2))->tx);

  //Updating the transmitters ID for the next node.
  my_data->tx = my_id;
  std::cout<<my_id<<" is forwarding the message: "<<my_msg<<" to node: "<<id<<std::endl;
  mailbox[id]->put(msg);
}
