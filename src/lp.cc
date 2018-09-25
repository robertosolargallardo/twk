#include <lp.hh>
lp::lp(void)
{
    this->_lvt=0.0;
    this->_sstep=0.0;
    this->_evl=nullptr;
    this->_comm=nullptr;
    this->_processed=std::make_shared<evlist>();
}
lp::lp(const lp &_lp)
{
    this->_lvt=_lp._lvt;
    this->_sstep=_lp._sstep;
    this->_evl=_lp._evl;
    this->_comm=_lp._comm;
    this->_processed=std::make_shared<evlist>(*(_lp._processed));
}
lp& lp::operator=(const lp &_lp)
{
    this->_lvt=_lp._lvt;
    this->_sstep=_lp._sstep;
    this->_evl=_lp._evl;
    this->_comm=_lp._comm;
    this->_processed=std::make_shared<evlist>(*(_lp._processed));
    return(*this);
}
lp::lp(const std::shared_ptr<evlist> &_evl,const std::shared_ptr<communicator> &_comm)
{
    this->_lvt=0.0;
    this->_sstep=0.0;
    this->_evl=_evl;
    this->_comm=_comm;
    this->_processed=std::make_shared<evlist>();
}
lp::~lp(void)
{

}
void lp::processed(const std::shared_ptr<event> &_event)
{
    this->_processed->push(_event);
}
double lp::lvt(void) const
{
    return(this->_lvt);
}
std::shared_ptr<evlist> lp::processed(void) const
{
    return(this->_processed);
}
void lp::lvt(const double &_lvt)
{
    this->_lvt=_lvt;
}
void lp::sstep(const double &_sstep)
{
    this->_sstep=_sstep;
}
void lp::restore(const std::shared_ptr<event> &_e)
{
    if(_e==nullptr)
        {
            this->_lvt=0.0;
            this->_sstep=0.0;
        }
    else
        {
            this->_lvt=_e->time(EXECUTION);
            this->_sstep=_e->sstep();
        }
}
