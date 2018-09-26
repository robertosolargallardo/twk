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
    auto antimessages=this->_comm->pull(this->tid(),this->_evl);

    if(!antimessages.empty())
        {
            for(auto a : antimessages)
                {
                    a->time(EXECUTION,-a->time(EXECUTION));
                    std::string name=a->data()["name"].get<std::string>();
                    if(this->_lps.count(name)==0 || this->_lps[name]->lvt()>a->time(EXECUTION))
                        this->_evl->remove(a);
                    else
                        this->rollback(name,a->time(EXECUTION));
                }
            antimessages.clear();
        }

    while(k<_k && !this->_evl->empty())
        {
            auto e=this->_evl->top();
            std::string name=e->data()["name"].get<std::string>();

            if(this->_lps.count(name)==0)
                this->_lps[name]=factory::build(e->data(),this->_evl,this->_comm);

            if(this->_lps[name]->lvt()>e->time(EXECUTION))
                {
                    this->rollback(name,e->time(EXECUTION));
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
    this->_toroll.push_back(_name);

    int N=this->_toroll.size();

    for(int i=0; i<N; ++i)
        {
            int position=this->_lps[this->_toroll[i]]->processed()->less_than(_time);
            this->rollback(std::string(this->_toroll[i]),position);
            N=this->_toroll.size();
        }
    this->_toroll.clear();
}
bool simulator::local(const std::shared_ptr<event> &_e)
{
    return(_e->dst(PID)==this->_pid && _e->dst(TID)==this->_tid);
}
void simulator::rollback(const std::string &_name,int &_position)
{
    this->_lps[_name]->processed()->move(_position);
    auto e=this->_lps[_name]->processed()->top();

    this->_lps[_name]->restore(e);

    this->_lps[_name]->processed()->move(++_position);
    e=this->_lps[_name]->processed()->top();

    while(e!=nullptr)
        {
            for(auto c : e->children())
                {
                    if(!c->processed())
                        {
                            if(this->local(c))
                                {
                                    this->_evl->remove(c);
                                }
                            else
                                {
                                    c->time(EXECUTION,-c->time(EXECUTION));
                                    this->_comm->send(c);
                                }
                        }
                    else
                        {
                            std::string name=c->data()["name"].get<std::string>();
                            if(name!=_name)
                                {
                                    if(std::count(this->_toroll.begin(),this->_toroll.end(),name)==0)
                                        {
                                            this->_toroll.push_back(name);
                                        }
                                }
                        }
                }
            e->children().clear();
            e->processed(false);
            this->_evl->push(e);

            this->_lps[_name]->processed()->remove(_position);
            e=this->_lps[_name]->processed()->top();
        }
}
