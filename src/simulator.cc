#include <simulator.hh>
simulator::simulator(void)
{
    this->_pid=bsp_pid();
    this->_tid=omp_get_thread_num();
    this->_evl=std::make_shared<evlist>();
    this->_comm=nullptr;
}
simulator::simulator(const std::shared_ptr<communicator> &_comm)
{
    this->_pid=bsp_pid();
    this->_tid=omp_get_thread_num();
    this->_evl=std::make_shared<evlist>();
    this->_comm=_comm;
}
simulator::simulator(const simulator &_sim)
{
    this->_pid=_sim._pid;
    this->_tid=_sim._tid;
    this->_comm=_sim._comm;
    this->_evl=std::make_shared<evlist>(*_sim._evl);
}
simulator& simulator::operator=(const simulator &_sim)
{
    this->_pid=_sim._pid;
    this->_tid=_sim._tid;
    this->_comm=_sim._comm;
    this->_evl=std::make_shared<evlist>(*_sim._evl);
    return(*this);
}
simulator::~simulator(void)
{
    this->_lps.clear();
}
void simulator::run(const int &_k)
{
    int k=0;
    this->_comm->pull(this->tid(),this->_evl);

    while(k<_k && !this->_evl->empty())
        {
            auto e=this->_evl->top();
            std::string name=e->data()["name"].get<std::string>();

            if(this->_lps.count(name)==0)
                this->_lps[name]=factory::build(e->data(),this->_evl,this->_comm);

            if(this->_lps[name]->lvt()>e->time(EXECUTION))
                {
                    this->rollback(name,e->time(EXECUTION));
                    //exit(0);
                }
            else
                {
                    this->_lps[name]->cause(e);
                    this->_evl->pop();
                    ++k;
                }
        }
}
int simulator::pid(void) const
{
    return(this->_pid);
}
int simulator::tid(void) const
{
    return(this->_tid);
}
void simulator::rollback(const std::string &_name,const double &_time)
{
    std::cout << "rollback " << _time <<std::endl;

    int position=this->_lps[_name]->processed().less_than(_time);
    std::cout << position << std::endl;

    exit(0);

    /*evlist::evlist_t::iterator it=this->_lps[_name]->processed().begin();

    for(;it!=this->_lps[_name]->processed().end();++it)
    	if((*it)->time()[1]<_time) break;*/
}
void simulator::rollback(const std::string &_name,const evlist::evlist_t::iterator &_it)
{
    /*if((*_it)==nullptr){
    	this->_lps[_name]->lvt(0.0);
    	this->_lps[_name]->sstep(0.0);
    }
    else{
    	this->_lps[_name]->lvt((*_it)->time()[1]);
    	this->_lps[_name]->sstep((*_it)->sstep());
    }*/
}
