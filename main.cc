#include <ast.h>
#include <flow_graph.h>
#include <iostream>

/*
 *     LiteralNode* ten = new LiteralNode(Number(10));
    BinaryOpNode* add = new BinaryOpNode(BinaryOp::kADD, ten, ten);
    StoreLocalNode* store = new StoreLocalNode(new LocalVariable("x"), add);
    LoadLocalNode* load = new LoadLocalNode(new LocalVariable("x"));
    ReturnNode* ret = new ReturnNode(load);

    SequenceNode* code = new SequenceNode();
    code->Add(store);
    code->Add(ret);

    FlowGraph* graph = (new FlowGraphBuilder(code))->Build();

    Instruction* curr = graph->Entry();
    while(curr != nullptr){
        std::cout << curr->Name() << std::endl;
        curr = curr->Next();
    }
 */

int main(int argc, char** argv){
    Assembler* assm = new Assembler();
}