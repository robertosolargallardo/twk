#ifndef _LP_HH_
#define _LP_HH_
#include <glob.hh>
#include <evlist.hh>
#include <communicator.hh>

class lp
{
protected:
    double _lvt;
    double _sstep;

    std::shared_ptr<evlist> _processed;
    std::shared_ptr<evlist>       _evl;
    std::shared_ptr<communicator> _comm;

public:
    lp(void);
    lp(const std::shared_ptr<evlist>&,const std::shared_ptr<communicator>&);
    lp(const lp&);
    lp& operator=(const lp&);
    ~lp(void);

    virtual void cause(const std::shared_ptr<event>&)=0;

    void processed(const std::shared_ptr<event>&);
    std::shared_ptr<evlist> processed(void) const;

    double lvt(void) const;
    void lvt(const double&);

    void sstep(const double&);
    void restore(const std::shared_ptr<event>&);
};
#endif
