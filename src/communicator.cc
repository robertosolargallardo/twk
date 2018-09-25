#include <communicator.hh>
#include <iostream>

communicator::communicator(void)
{
    this->_number_of_threads=1;
    this->_input=std::make_unique<buffer_t[]>(_number_of_threads);
    this->_output=std::make_unique<buffer_t[]>(_number_of_threads);
}
communicator::communicator(const int &_number_of_threads)
{
    this->_number_of_threads=_number_of_threads;
    this->_input=std::make_unique<buffer_t[]>(_number_of_threads);
    this->_output=std::make_unique<buffer_t[]>(_number_of_threads);
}
communicator::communicator(const communicator &_comm)
{
    this->_number_of_threads=_comm._number_of_threads;
    this->_input=std::make_unique<buffer_t[]>(_comm._number_of_threads);
    this->_output=std::make_unique<buffer_t[]>(_comm._number_of_threads);
    for(int i=0; i<this->_number_of_threads; ++i)
        {
            this->_input[i]=_comm._input[i];
            this->_output[i]=_comm._output[i];
        }
}
communicator& communicator::operator=(const communicator &_comm)
{
    this->_number_of_threads=_comm._number_of_threads;
    this->_input=std::make_unique<buffer_t[]>(_comm._number_of_threads);
    this->_output=std::make_unique<buffer_t[]>(_comm._number_of_threads);
    for(int i=0; i<this->_number_of_threads; ++i)
        {
            this->_input[i]=_comm._input[i];
            this->_output[i]=_comm._output[i];
        }
    return(*this);
}
communicator::~communicator(void)
{
    ;
}
int communicator::number_of_threads(void) const
{
    return(this->_number_of_threads);
}
void communicator::send(void)
{
    for(int i=0; i<this->_number_of_threads; ++i)
        {
            while(!this->_output[i].empty())
                {
                    auto e=this->_output[i].back();
                    std::string serialized=e->serialize();
                    bsp_send(e->dst(PID),nullptr,serialized.c_str(),serialized.size());
                    this->_output[i].pop_back();
                }
        }
}
void communicator::send(const std::shared_ptr<event> &_event)
{
    this->_output[_event->src(TID)].push_back(_event);
}
void communicator::recv(void)
{
    int number_of_packages;
    int size,tag;
    char *buffer=nullptr;

    bsp_qsize(&number_of_packages,&size);

    for(int i=0; i<number_of_packages; ++i)
        {
            bsp_get_tag(&size,&tag);
            buffer=(char*)malloc(size);
            bsp_move(buffer,size);

            std::shared_ptr<event> e=std::make_shared<event>(json::parse(std::string(buffer,size)));

            this->_input[e->dst(TID)].push_back(e);

            free(buffer);
        }
}
void communicator::pull(const int &_tid,const std::shared_ptr<evlist> &_evl)
{
    while(!this->_input[_tid].empty())
        {
            if(this->_input[_tid].back()->time(EXECUTION)<0.0)
                {
                    std::cout<< "antimensaje" <<std::endl;
                }
            else
                {
                    _evl->push(this->_input[_tid].back());
                }
            this->_input[_tid].pop_back();
        }
}
