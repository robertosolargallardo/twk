#ifndef _EVLIST_HH_
#define _EVLIST_HH_
#include <glob.hh>
#include <event.hh>
#include <iostream>

class evlist
{
public:
    typedef std::set<std::shared_ptr<event>,event::compare> evlist_t;
private:
    evlist_t _list;
    int      _disp;

public:
    evlist(void);
    evlist(const evlist&);
    evlist& operator=(const evlist&);
    ~evlist(void);

    void push(const std::shared_ptr<event>&);
    std::shared_ptr<event> top(void);
    void pop(void);
    bool empty(void);
    void move(const int&);
    int less_than(const double&);

    int disp(void) const
    {
        return(this->_disp);
    }
    /**************/
    void show(void) const
    {
        for(auto& e : this->_list)
            {
                std::cout << e->time(EXECUTION) << std::endl;
                for(auto& c : e->children())
                    std::cout <<"\t" << c->time(EXECUTION) << std::endl;

            }
    }
    /**************/
};
#endif
