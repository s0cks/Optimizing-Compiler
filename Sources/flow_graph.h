#ifndef FLOWGRAPH_FLOW_GRAPH_H
#define FLOWGRAPH_FLOW_GRAPH_H

#include "il.h"

class FlowGraph{
public:
    FlowGraph(BlockEntryInstr* entry, AstNode* ast):
            ast_(ast),
            entry_(entry){}

    BlockEntryInstr* Entry(){
        return entry_;
    }

    AstNode* Tree(){
        return ast_;
    }
private:
    BlockEntryInstr* entry_;
    AstNode* ast_;
};

class FlowGraphBuilder{
public:
    FlowGraphBuilder(AstNode* node):
            node_(node){}

    FlowGraph* Build();

    AstNode* Tree(){
        return node_;
    }
private:
    AstNode* node_;
    GraphEntryInstr* graph_entry_;
};

class EffectGraphVisitor: public AstNodeVisitor{
public:
    EffectGraphVisitor(FlowGraphBuilder* owner):
            owner_(owner),
            entry_(nullptr),
            exit_(nullptr){}

#define DECLARE_VISIT(BaseName) \
    virtual void Visit##BaseName##Node(BaseName##Node* node);
FOR_EACH_NODE(DECLARE_VISIT);
#undef DECLARE_VISIT

    FlowGraphBuilder* Owner(){
        return owner_;
    }

    Instruction* Entry() const{
        return entry_;
    }

    Instruction* Exit() const{
        return exit_;
    }

    inline PushArgumentInstr* PushArgument(Object* value){
        PushArgumentInstr* res = new PushArgumentInstr(value);
        AddInstruction(res);
        return res;
    }

    void Append(const EffectGraphVisitor& other);
    void AddInstruction(Instruction* instr);
protected:
    inline Instruction* BuildStoreLocal(LocalVariable* local, Object* value){
        return new StoreLocalInstr(local, value);
    }

    inline Instruction* BuildLoadLocal(LocalVariable* local){
        return new LoadLocalInstr(local);
    }

    inline bool IsEmpty() const{
        return entry_ == nullptr;
    }
private:
    FlowGraphBuilder* owner_;
    Instruction* entry_;
    Instruction* exit_;
};

class ValueGraphVisitor: public EffectGraphVisitor{
public:
    ValueGraphVisitor(FlowGraphBuilder* owner):
            EffectGraphVisitor(owner),
            value_(nullptr){}

    virtual void VisitLoadLocalNode(LoadLocalNode* node);

    Object* Value(){
        return value_;
    }
protected:
    Object* value_;
};

#endif