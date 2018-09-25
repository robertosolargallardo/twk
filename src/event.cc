#include <event.hh>
#include <iostream>

event::event(void)
{
    this->_time= {0.0,0.0};
    this->_sstep=0.0;
    this->_src= {0,0};
    this->_dst= {0,0};
    this->_processed=false;
}
event::event(const double &_current_time,const double &_time,const double &_sstep,const int &_src_pid,const int &_src_tid,const int &_dst_pid,const int &_dst_tid,const json &_data)
{
    this->_time= {_current_time,_time};
    this->_sstep=_sstep;
    this->_src= {_src_pid,_src_tid};
    this->_dst= {_dst_pid,_dst_tid};
    this->_data=_data;
    this->_processed=false;
}
event::event(const event &_event)
{
    this->_time=_event._time;
    this->_sstep=_event._sstep;
    this->_src=_event._src;
    this->_dst=_event._dst;
    this->_data=_event._data;
    this->_children=_event._children;
    this->_processed=_event._processed;
}
event::event(const json &_jevent)
{
    this->_time= {_jevent["time"][0].get<double>(),_jevent["time"][1].get<double>()};
    this->_sstep=_jevent["sstep"].get<double>();
    this->_src= {_jevent["src"][0].get<int>(),_jevent["src"][1].get<int>()};
    this->_dst= {_jevent["dst"][0].get<int>(),_jevent["dst"][1].get<int>()};
    this->_data=_jevent["data"];
    this->_processed=false;
}
event& event::operator=(const event &_event)
{
    this->_time=_event._time;
    this->_sstep=_event._sstep;
    this->_src=_event._src;
    this->_dst=_event._dst;
    this->_data=_event._data;
    this->_children=_event._children;
    this->_processed=_event._processed;
    return(*this);
}
event::~event(void)
{
    this->_children.clear();
}
void event::child(const std::shared_ptr<event> &_event)
{
    this->_children.insert(_event);
}
std::set<std::shared_ptr<event>,event::compare>& event::children(void)
{
    return(this->_children);
}
json event::data(void) const
{
    return(this->_data);
}
std::string event::serialize(void)
{
    json jevent;
    jevent["time"]= {this->_time[0],this->_time[1]};
    jevent["sstep"]=this->_sstep;
    jevent["src"]= {this->_src[0],this->_src[1]};
    jevent["dst"]= {this->_dst[0],this->_dst[1]};
    jevent["data"]=this->_data;
    return(jevent.dump());
}
double event::sstep(void) const
{
    return(this->_sstep);
}
void event::sstep(const double &_sstep)
{
    this->_sstep=_sstep;
}
void event::time(const int &_type,const double &_value){
	this->_time[_type]=_value;
}
double event::time(const int &_type) const
{
    return(this->_time[_type]);
}
int event::src(const int &_type) const
{
    return(this->_src[_type]);
}
int event::dst(const int &_type) const
{
    return(this->_dst[_type]);
}
bool event::processed(void) const{
	return(this->_processed);
}
void event::processed(const bool &_processed){
   this->_processed=_processed;
}
