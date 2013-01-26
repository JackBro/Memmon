#ifndef MMVARPROXY_H
#define MMVARPROXY_H

class Memmon;
class MmVarProxy
{
public:
    explicit MmVarProxy(Memmon *parent = 0);
    
private:
    Memmon* _parent;
    
};

#endif // MMVARPROXY_H
