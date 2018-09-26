#include <evlist.hh>

evlist::evlist(void)
{
    this->_disp=0;
}
evlist::evlist(const evlist &_evlist)
{
    this->_list=_evlist._list;
    this->_disp=this->_disp;
}
evlist& evlist::operator=(const evlist &_evlist)
{
    this->_list=_evlist._list;
    this->_disp=this->_disp;
    return(*this);
}
evlist::~evlist(void)
{
    this->_list.clear();
}

void evlist::push(const std::shared_ptr<event> &_event)
{
    this->_list.insert(_event);
}
std::shared_ptr<event> evlist::top(void)
{
    if(this->empty())
        return(nullptr);

    evlist_t::iterator it=this->_list.begin();
    std::advance(it,this->_disp);
    return(*it);
}
void evlist::pop(void)
{
    this->_disp++;
}
bool evlist::empty(void)
{
    if(this->_list.empty() || this->_disp>=this->_list.size())
        return(true);
    return(false);
}
void evlist::move(const int &_disp)
{
    this->_disp=_disp;
}
void evlist::remove(const std::shared_ptr<event> &_e)
{
    auto it=std::find_if(this->_list.begin(),this->_list.end(),[&_e](const std::shared_ptr<event> &_i)->bool{return((*_i)==(*_e));});
    this->remove(std::distance(this->_list.begin(),it));
}
void evlist::remove(const int &_position)
{
    if(_position==this->_list.size())
        return;

    evlist_t::iterator it=this->_list.begin();
    std::advance(it,_position);

    this->_list.erase(it);

    if(_position<this->_disp)
        --this->_disp;
}
int evlist::less_than(const double &_time)
{
    //int position=this->_disp;
    int position=0;
    evlist_t::iterator it=this->_list.begin();
    //std::advance(it,this->_disp);

    for(; it!=this->_list.end() && (*it)->time(EXECUTION)<_time; ++it,++position);

    return(--position);
}
