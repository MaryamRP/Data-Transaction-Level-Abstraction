#include <systemc.h>

template <class T>
class put_if : virtual public sc_interface {
public:
    enum putStatus { putSuccess, putDuplicate, putBlocked };
    virtual putStatus put(int initiator, T data) = 0;  
    virtual bool checkPut() = 0;
};


template <class T, class P>
class get_if : virtual public sc_interface {
public:
    enum putStatus { putSuccess, putDuplicate, putBlocked };
    virtual sc_int<8>* get(int target) = 0;  
    virtual bool checkGet() = 0;
};
