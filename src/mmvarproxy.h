#ifndef MMVARPROXY_H
#define MMVARPROXY_H

#include <QStringList>

class Memmon;
class MmVarProxy
{
public:
    explicit MmVarProxy(Memmon *parent = 0);
    
public:
    Memmon* _parent;
    
    QStringList columns;

};

#endif // MMVARPROXY_H
