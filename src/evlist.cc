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
    if(this->_disp==this->_list.size())
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
    if(this->_list.empty() || this->_disp==this->_list.size())
        return(true);
    return(false);
}
void evlist::move(const int &_distance)
{
    this->_disp+=_distance;
}
int evlist::less_than(const double &_time)
{
    std::cout << this->_list.size() << std::endl;
    std::cout << this->_disp << std::endl;
    exit(0);
    int position=this->_disp;
    evlist_t::iterator it=this->_list.begin();
    std::advance(it,this->_disp);

    for(; it!=this->_list.end() && (*it)->time(EXECUTION)<_time; ++it,++position)
        {
            std::cout << (*it)->time(EXECUTION) << std::endl;
        }

    return(--position);
}
