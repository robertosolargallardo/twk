#ifndef _COMMUNICATOR_HH_
#define _COMMUNICATOR_HH_
#include <glob.hh>
#include <event.hh>
#include <evlist.hh>
class communicator
{
public:
    typedef std::vector<std::shared_ptr<event>> buffer_t;
private:
    std::unique_ptr<buffer_t[]> _input;
    std::unique_ptr<buffer_t[]> _output;
    int _number_of_threads;

public:
    communicator(void);
    communicator(const communicator&);
    communicator(const int&);
    communicator& operator=(const communicator&);
    ~communicator(void);

    int number_of_threads(void) const;

    void send(const std::shared_ptr<event>&);
    void send(void);
    void recv(void);
    std::vector<std::shared_ptr<event>> pull(const int&,const std::shared_ptr<evlist>&);
};
#endif
