#ifndef _SIMULATOR_HH_
#define _SIMULATOR_HH_
#include <glob.hh>
#include <lp.hh>
#include <evlist.hh>
#include <factory.hh>
#include <communicator.hh>
#include <iostream>

class simulator
{
private:
    int _pid,_tid;
    std::shared_ptr<evlist> _evl;
    std::map<std::string,std::shared_ptr<lp>> _lps;

    std::shared_ptr<communicator> _comm;

public:
    simulator(void);
    simulator(const std::shared_ptr<communicator>&);
    simulator(const simulator&);
    simulator& operator=(const simulator&);
    ~simulator(void);

    void run(const int&);
    int pid(void) const;
    int tid(void) const;

private:
    void rollback(const std::string&,const double&);
    void rollback(const std::string&,const evlist::evlist_t::iterator&);
};
#endif
