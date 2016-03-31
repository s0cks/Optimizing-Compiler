#ifndef FLOWGRAPH_IL_H
#define FLOWGRAPH_IL_H

#include <vector>
#include "type.h"
#include "scope.h"
#include "ast.h"

#define FOR_EACH_INSTRUCTION(V) \
    V(GraphEntry) \
    V(PushArgument) \
    V(Return) \
    V(LoadLocal) \
    V(StoreLocal) \
    V(BinaryOp) \
    V(InstanceCall) \
    V(Constant)

#define FORWARD_DECLARE(BaseName) class BaseName##Instr;
FOR_EACH_INSTRUCTION(FORWARD_DECLARE)
#undef FORWARD_DECLARE

#define DECLARE_INSTRUCTION \
    virtual const char* Name();

class Instruction{
public:
#define DECLARE_INSTRUCTION_TYPE_CHECK(BaseName) \
    bool Is##BaseName() { return As##BaseName() != nullptr; } \
    virtual BaseName##Instr* As##BaseName(){ return nullptr; }
FOR_EACH_INSTRUCTION(DECLARE_INSTRUCTION_TYPE_CHECK)
#undef DECLARE_INSTRUCTION_TYPE_CHECK

    Instruction* Previous(){
        return previous_;
    }

    void SetPrevious(Instruction* instr){
        previous_ = instr;
    }

    Instruction* Next(){
        return next_;
    }

    void SetNext(Instruction* instr){
        next_ = instr;
    }

    void Link(Instruction* next){
        SetNext(next);
        next->SetPrevious(this);
    }

    virtual const char* Name(){ return ""; }
private:
    Instruction* next_;
    Instruction* previous_;
};

class BlockEntryInstr: public Instruction{
public:
    virtual BlockEntryInstr* AsBlockEntry(){
        return this;
    }

    void AddDominatedBlock(BlockEntryInstr* block){
        block->SetDominator(this);
        dominated_blocks_.push_back(block);
    }

    std::vector<BlockEntryInstr*> DominatedBlocks(){
        return dominated_blocks_;
    }

    virtual const char* Name(){
        return "BlockEntry";
    }
protected:
    BlockEntryInstr():
            dominated_blocks_(std::vector<BlockEntryInstr*>()),
            dominator_(nullptr){}
private:
    BlockEntryInstr* dominator_;
    std::vector<BlockEntryInstr*> dominated_blocks_;

    void SetDominator(BlockEntryInstr* instr){
        dominator_ = instr;
    }
};

class GraphEntryInstr: public BlockEntryInstr{
public:
    GraphEntryInstr():
            BlockEntryInstr(){}

    DECLARE_INSTRUCTION;
};

class ReturnInstr: public Instruction{
public:
    ReturnInstr(Object* value):
            value_(value){}

    DECLARE_INSTRUCTION;
private:
    Object* value_;
};

class LoadLocalInstr: public Instruction{
public:
    LoadLocalInstr(LocalVariable* local):
            local_(local){}

    LocalVariable* Local(){
        return local_;
    }

    DECLARE_INSTRUCTION;
private:
    LocalVariable* local_;
};

class StoreLocalInstr: public Instruction{
public:
    StoreLocalInstr(LocalVariable* local, Object* value):
            local_(local),
            value_(value){}

    LocalVariable* Local(){
        return local_;
    }

    Object* Value(){
        return value_;
    }

    DECLARE_INSTRUCTION;
private:
    LocalVariable* local_;
    Object* value_;
};

class BinaryOpInstr: public Instruction{
public:
    BinaryOpInstr(BinaryOp op, Object* left, Object* right):
            op_(op),
            left_(left),
            right_(right){}

    Object* Left(){
        return left_;
    }

    Object* Right(){
        return right_;
    }

    BinaryOp Op(){
        return op_;
    }

    DECLARE_INSTRUCTION;
private:
    BinaryOp op_;
    Object* left_;
    Object* right_;
};

class PushArgumentInstr: public Instruction{
public:
    PushArgumentInstr(Object* value):
            value_(value){}

    Object* Value(){
        return value_;
    }

    DECLARE_INSTRUCTION;
private:
    Object* value_;
};

class InstanceCallInstr: public Instruction{
public:
    InstanceCallInstr(BinaryOp op, std::vector<PushArgumentInstr*> args):
            args_(args),
            op_(op){}

    BinaryOp Kind(){
        return op_;
    }

    DECLARE_INSTRUCTION
private:
    BinaryOp op_;
    std::vector<PushArgumentInstr*> args_;
};

class ConstantInstr: public Instruction{
public:
    ConstantInstr(Object* value):
            value_(value){}

    Object* Value(){
        return value_;
    }

    DECLARE_INSTRUCTION;
private:
    Object* value_;
};

#endif