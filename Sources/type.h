#ifndef FLOWGRAPH_TYPE_H
#define FLOWGRAPH_TYPE_H

#include <string>

#define FOR_EACH_TYPE(V) \
    V(String) \
    V(Number) \
    V(Boolean)

enum TID{
    kUnknownTID = 1,
#define DECLARE_TID(BaseName) k##BaseName##TID,
    FOR_EACH_TYPE(DECLARE_TID)
#undef DECLARE_TID
};

class Object{
public:
    Object(TID tid):
            tid_(tid){}
protected:
    TID tid_;
};

class String: public Object{
public:
    String(const std::string& value):
            value_(value),
            Object(kStringTID){}

    std::string Value(){
        return value_;
    }
private:
    std::string value_;
};

class Number: public Object{
public:
    Number(const double& value):
            value_(value),
            Object(kNumberTID){}

    double Value(){
        return value_;
    }
private:
    double value_;
};

class Boolean: public Object{
public:
    Boolean(const bool& value):
            value_(value),
            Object(kBooleanTID){}

    bool Value(){
        return value_;
    }
private:
    bool value_;
};

#endif