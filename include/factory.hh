#ifndef _FACTORY_HH_
#define _FACTORY_HH_
#include <lp.hh>

class node: public lp
{
public:
    node(const std::shared_ptr<evlist> &_evl,const std::shared_ptr<communicator> &_comm):lp(_evl,_comm)
    {
        ;
    }
    void cause(const std::shared_ptr<event> &_event)
    {
//std::cout << _event->data()["name"].dump() << std::endl;
        static thread_local std::mt19937 rng(time(0));

        this->_lvt=_event->time(EXECUTION);

        this->processed(_event);
        std::cout << this->_processed.disp() << std::endl;

        if(this->_sstep<_event->sstep())
            this->_sstep=_event->sstep();
        _event->sstep(this->_sstep);

        std::uniform_int_distribution<int> proc(0,bsp_nprocs()-1);
        std::uniform_int_distribution<int> thread(0,omp_get_num_threads()-1);
        std::exponential_distribution<double> delay(1.0);
        std::uniform_int_distribution<int> target(0,_event->data()["number-of-lps"].get<int>()-1);

        json data=_event->data();
        data["name"]="LP"+std::to_string(target(rng));
        std::shared_ptr<event> child=std::make_shared<event>(_event->time(EXECUTION),_event->time(EXECUTION)+delay(rng),this->_sstep,bsp_pid(),omp_get_thread_num(),proc(rng),thread(rng),data);
        _event->child(child);

        if(child->dst(PID)==bsp_pid() && child->dst(TID)==omp_get_thread_num())
            this->_evl->push(child);
        else
            {
                child->sstep(child->sstep()+1.0);
                this->sent(child);
                this->_comm->send(child);
            }
    }
};

class factory
{
public:
    static std::shared_ptr<lp> build(const json &_data,const std::shared_ptr<evlist> &_evl,const std::shared_ptr<communicator> &_comm)
    {
        std::string type=_data["type"].get<std::string>();
        if(type=="node")
            return(std::make_shared<node>(_evl,_comm));
        else
            {
                std::cerr << "error :: unknown class \"" << type << "\"" << std::endl;
                exit(EXIT_FAILURE);
            }
        return(nullptr);
    }
};
#endif
