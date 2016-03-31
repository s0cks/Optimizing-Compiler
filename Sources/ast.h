#ifndef FLOWGRAPH_AST_H
#define FLOWGRAPH_AST_H

#include <vector>
#include "scope.h"
#include "type.h"

#define FOR_EACH_NODE(V) \
    V(Return) \
    V(BinaryOp) \
    V(Literal) \
    V(LoadLocal) \
    V(StoreLocal) \
    V(Sequence)

#define FORWARD_DECLARE(BaseName) class BaseName##Node;
FOR_EACH_NODE(FORWARD_DECLARE)
#undef FORWARD_DECLARE

class AstNodeVisitor{
public:
    AstNodeVisitor(){}
    virtual ~AstNodeVisitor(){}

#define DEFINE_VISIT_FUNCTION(BaseName) \
    virtual void Visit##BaseName##Node(BaseName##Node* node) {}
FOR_EACH_NODE(DEFINE_VISIT_FUNCTION)
#undef DEFINE_VISIT_FUNCTION
};

#define DEFINE_COMMON_NODE_FUNCTIONS(BaseName) \
    virtual void Visit(AstNodeVisitor* visitor); \
    virtual BaseName* As##BaseName(){ return this; }

class AstNode{
public:
    AstNode(){}
    virtual ~AstNode(){}

#define AST_TYPE_CHECK(BaseName) \
    bool Is##BaseName##Node() { return As##BaseName##Node() != nullptr; } \
    virtual BaseName##Node* As##BaseName##Node(){ return nullptr; }
FOR_EACH_NODE(AST_TYPE_CHECK)
#undef AST_TYPE_CHECK

    virtual void Visit(AstNodeVisitor* visitor) = 0;
    virtual void VisitChildren(AstNodeVisitor* visitor) = 0;
};

class SequenceNode: public AstNode{
public:
    SequenceNode():
            nodes_(std::vector<AstNode*>()){}

    void VisitChildren(AstNodeVisitor* visitor){
        std::vector<AstNode*>::iterator iter;
        for(iter = nodes_.begin(); iter != nodes_.end(); iter++){
            (*iter)->Visit(visitor);
        }
    }

    void Add(AstNode* node){
        nodes_.push_back(node);
    }

    std::vector<AstNode*>::iterator Begin(){
        return nodes_.begin();
    }

    std::vector<AstNode*>::iterator End(){
        return nodes_.end();
    }

    DEFINE_COMMON_NODE_FUNCTIONS(SequenceNode);
private:
    std::vector<AstNode*> nodes_;
};

class LoadLocalNode: public AstNode{
public:
    LoadLocalNode(LocalVariable* local):
            local_(local){}

    LocalVariable* Local(){
        return local_;
    }

    virtual void VisitChildren(AstNodeVisitor* visitor){ }

    DEFINE_COMMON_NODE_FUNCTIONS(LoadLocalNode);
private:
    LocalVariable* local_;
};

class StoreLocalNode: public AstNode{
public:
    StoreLocalNode(LocalVariable* local, AstNode* value):
            local_(local),
            value_(value){}

    LocalVariable* Local(){
        return local_;
    }

    AstNode* Value(){
        return value_;
    }

    virtual void VisitChildren(AstNodeVisitor* visitor){
        Value()->Visit(visitor);
    }

    DEFINE_COMMON_NODE_FUNCTIONS(StoreLocalNode);
private:
    LocalVariable* local_;
    AstNode* value_;
};

class ReturnNode: public AstNode{
public:
    ReturnNode(AstNode* value):
            value_(value){}

    AstNode* Value(){
        return value_;
    }

    virtual void VisitChildren(AstNodeVisitor* visitor){
        if(Value() != nullptr){
            Value()->Visit(visitor);
        }
    }

    DEFINE_COMMON_NODE_FUNCTIONS(ReturnNode);
private:
    AstNode* value_;
};

enum BinaryOp{
    kADD,
    kSUB,
    kMUL,
    kDIV
};

class BinaryOpNode: public AstNode{
public:
    BinaryOpNode(BinaryOp op, AstNode* left, AstNode* right):
            op_(op),
            left_(left),
            right_(right){}

    BinaryOp Kind(){
        return op_;
    }

    AstNode* Left(){
        return left_;
    }

    AstNode* Right(){
        return right_;
    }

    virtual void VisitChildren(AstNodeVisitor* visitor){
        Left()->Visit(visitor);
        Right()->Visit(visitor);
    }

    DEFINE_COMMON_NODE_FUNCTIONS(BinaryOpNode);
private:
    BinaryOp op_;
    AstNode* left_;
    AstNode* right_;
};

class LiteralNode: public AstNode{
public:
    LiteralNode(const Object& literal):
            literal_(literal){}

    Object* Literal(){
        return &literal_;
    }

    virtual void VisitChildren(AstNodeVisitor* visitor){}

    DEFINE_COMMON_NODE_FUNCTIONS(LiteralNode);
private:
    Object literal_;
};

#endif