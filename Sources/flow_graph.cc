#include <iostream>
#include "flow_graph.h"

static inline Instruction* AppendFragment(BlockEntryInstr* entry, const EffectGraphVisitor& frag){
    entry->Link(frag.Entry());
    return frag.Exit();
}

FlowGraph* FlowGraphBuilder::Build() {
    graph_entry_ = new GraphEntryInstr();
    EffectGraphVisitor for_effect(this);
    node_->Visit(&for_effect);
    AppendFragment(graph_entry_, for_effect);
    return new FlowGraph(graph_entry_, node_);
}

void EffectGraphVisitor::Append(const EffectGraphVisitor& other) {
    if(other.IsEmpty()) return;
    if(IsEmpty()){
        entry_ = other.entry_;
    } else{
        exit_->Link(other.entry_);
    }
    exit_ = other.exit_;
}

void EffectGraphVisitor::AddInstruction(Instruction* instr) {
    if(IsEmpty()){
        entry_ = exit_ = instr;
    } else{
        exit_->Link(instr);
        exit_ = instr;
    }
}

void EffectGraphVisitor::VisitBinaryOpNode(BinaryOpNode* node) {
    ValueGraphVisitor for_left_value(Owner());
    node->Left()->Visit(&for_left_value);
    Append(for_left_value);
    PushArgumentInstr* push_left = PushArgument(for_left_value.Value());

    ValueGraphVisitor for_right_value(Owner());
    node->Right()->Visit(&for_right_value);
    Append(for_right_value);
    PushArgumentInstr* push_right = PushArgument(for_right_value.Value());

    std::vector<PushArgumentInstr*> args = std::vector<PushArgumentInstr*>();
    args.push_back(push_left);
    args.push_back(push_right);

    InstanceCallInstr* call = new InstanceCallInstr(node->Kind(), args);
    AddInstruction(call);
}

void ValueGraphVisitor::VisitLoadLocalNode(LoadLocalNode* node) {
    Instruction* load = BuildLoadLocal(node->Local());
    AddInstruction(load);
}

void EffectGraphVisitor::VisitStoreLocalNode(StoreLocalNode* node) {
    ValueGraphVisitor for_value(Owner());
    node->Value()->Visit(&for_value);
    Append(for_value);

    Instruction* store = BuildStoreLocal(node->Local(), for_value.Value());
    AddInstruction(store);
}

void EffectGraphVisitor::VisitReturnNode(ReturnNode* node) {
    ValueGraphVisitor for_value(Owner());
    node->Value()->Visit(&for_value);
    Append(for_value);
    Object* return_value = for_value.Value();

    ReturnInstr* return_instr = new ReturnInstr(return_value);
    AddInstruction(return_instr);
}

void EffectGraphVisitor::VisitLiteralNode(LiteralNode* node) {
    AddInstruction(new ConstantInstr(node->Literal()));
}

void EffectGraphVisitor::VisitSequenceNode(SequenceNode* node) {
    std::vector<AstNode*>::iterator iter;
    for(iter = node->Begin(); iter != node->End(); iter++){
        EffectGraphVisitor for_effect(Owner());
        (*iter)->Visit(&for_effect);
        Append(for_effect);
    }
}

void EffectGraphVisitor::VisitLoadLocalNode(LoadLocalNode* node) {
    // Fallthrough
}