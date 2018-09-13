#include <iostream>
#include <glob.hh>
#include <simulator.hh>
#include <evlist.hh>
#define NUMBER_OF_LPS   16
#define MESSAGE_DENSITY 32

void spmd(void)
{
    bsp_begin(bsp_nprocs());
    std::shared_ptr<communicator> comm=std::make_shared<communicator>(omp_get_max_threads());

    #pragma omp parallel shared(comm)
    {
        simulator sim(comm);

        std::mt19937 rng(time(0)); 
        std::exponential_distribution<double> delay(1.0);
        std::uniform_int_distribution<int> proc(0,bsp_nprocs()-1);
        std::uniform_int_distribution<int> thread(0,omp_get_num_threads()-1);
        std::uniform_int_distribution<int> target(0,NUMBER_OF_LPS-1);
        
        for(int i=0;i<NUMBER_OF_LPS;++i){
            for(int j=0;j<MESSAGE_DENSITY;++j){
               json data;
    	   		data["name"]="LP"+std::to_string(target(rng));
               data["type"]="node";
               data["number-of-lps"]=16;
               data["message-density"]=32;
					comm->send(std::make_shared<event>(0.0,delay(rng),0.0,bsp_pid(),omp_get_thread_num(),proc(rng),thread(rng),data));

            }
        }
        int k=MESSAGE_DENSITY/2+1;

	     for(int i=0;i<10;++i){
        		#pragma omp barrier
        		#pragma omp master
        		{
	         	comm->send();
            	bsp_sync();
            	comm->recv();
        		}
        		#pragma omp barrier

        		sim.run(k);
		  }
    }

    bsp_end();
}
void test_evl(void);
void test_comm(void);

int main(int argc,char **argv)
{
    omp_set_num_threads(std::stoi(argv[1]));

    bsp_init(&spmd,argc,argv);
    spmd();

    /*bsp_init(&test_comm,argc,argv);
    test_comm();*/

    //test_evl();

    return(0);
}
void test_evl(void)
{
    json data;
    evlist evl;
    evl.push(std::make_shared<event>(0.0,1.2,0.0,0,0,1,1,data));
    evl.push(std::make_shared<event>(0.0,5.3,0.0,0,0,1,1,data));
    evl.push(std::make_shared<event>(0.0,3.4,0.0,0,0,1,1,data));
    evl.push(std::make_shared<event>(0.0,4.5,0.0,0,0,1,1,data));

    auto e=evl.top();

    std::cout << "top " << e->time(EXECUTION) << std::endl;

    evl.pop();

    e=evl.top();

    e->child(std::make_shared<event>(0.0,5.3,0.0,0,0,1,1,data));
    e->child(std::make_shared<event>(0.0,7.2,0.0,0,0,1,1,data));
    e->child(std::make_shared<event>(0.0,7.1,0.0,0,0,1,1,data));

    std::cout << "top " << e->time(EXECUTION) << std::endl;

    evl.pop();
    evl.pop();
    evl.pop();

    e=evl.top();

    std::cout << "top " << (e==nullptr) << std::endl;

    evl.show();

}
void test_comm(void)
{
    bsp_begin(bsp_nprocs());
    communicator comm(omp_get_max_threads());

	 #pragma omp parallel shared(comm)
    {
	 	 for(int i=0;i<bsp_nprocs();++i){
			for(int j=0;j<omp_get_num_threads();++j){
				uint64_t now=std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    	   	json data;
    	   	data["message"]=std::to_string(now)+" hello from proc "+std::to_string(bsp_pid())+" and thread "+std::to_string(omp_get_thread_num());
				comm.send(std::make_shared<event>(0.0,0.0,0.0,bsp_pid(),omp_get_thread_num(),i,j,data));
         }
		 }
	    #pragma omp barrier
		 #pragma omp master
       {
	    	comm.send();
         bsp_sync();
         comm.recv();
       }
		 #pragma omp barrier

	 }
	 
    bsp_end();
}


