#ifndef FLOWGRAPH_SCOPE_H
#define FLOWGRAPH_SCOPE_H

#include <string>

class LocalVariable{
public:
    LocalVariable(const std::string& name):
            name_(name){}

    std::string Name(){
        return name_;
    }

    int GetIndex(){
        return index_;
    }

    void SetIndex(int index){
        index_ = index;
    }

private:
    std::string name_;
    int index_;
};

#endif