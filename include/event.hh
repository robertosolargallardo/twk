#ifndef _EVENT_HH_
#define _EVENT_HH_
#include <glob.hh>
#include <iostream>
#define CREATION  0
#define EXECUTION 1
#define PID 0
#define TID 1

class event
{
public:
    class compare
    {
    public:
        bool operator()(const std::shared_ptr<event> &_a,const std::shared_ptr<event> &_b) const
        {
            return(_a->time(EXECUTION)<_b->time(EXECUTION));
        }
    };

private:
    std::array<double,2> _time;
    double               _sstep;
    std::array<int,2>    _src;
    std::array<int,2>    _dst;
    json                 _data;

    std::set<std::shared_ptr<event>,compare> _children;


public:
    event(void);
    event(const double&,const double&,const double&,const int&,const int&,const int&,const int&,const json&);
    event(const json&);
    event(const event&);
    event& operator=(const event&);
    ~event(void);

    double time(const int&) const;
    int src(const int&) const;
    int dst(const int&) const;
    double sstep(void) const;
    void sstep(const double&);

    void child(const std::shared_ptr<event>&);
    std::set<std::shared_ptr<event>,compare> children(void);
    json data(void) const;
    std::string serialize(void);

    /********************/
    void show(void)
    {
        std::cout << this->_time[0] << " " << this->_time[1] << " ";
        std::cout << this->_src[0] << " " << this->_src[1] << " ";
        std::cout << this->_dst[0] << " " << this->_dst[1] << std::endl;
    }
    /********************/
};
#endif
