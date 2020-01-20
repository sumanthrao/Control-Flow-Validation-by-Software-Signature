//===- Hello.cpp - Example code from "Writing an LLVM Pass" ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements two versions of the LLVM "Hello World" pass described
// in docs/WritingAnLLVMPass.html
//
//===----------------------------------------------------------------------===//
#include<map>


#include "llvm/IR/Module.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/CFG.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstIterator.h"


using namespace llvm;

#define DEBUG_TYPE "control"

STATISTIC(HelloCounter, "Counts number of functions greeted");

namespace {
  LLVMContext Ctx;
  class Control : public FunctionPass {
  public:
    static char ID;
    GlobalVariable* GlobalSig;

    Control() : FunctionPass(ID){}
    bool runOnFunction(Function &F) override{
        Module *M = F.getParent();
       //declare a global variable for global signature (gSig)
        if(GlobalSig == NULL){
            GlobalSig = new GlobalVariable(
                *M, 
                IntegerType::get(M->getContext(), 32),
                false,
                GlobalValue::ExternalLinkage,
                0,
                "gSig");
            ConstantInt* const_int32_8 = ConstantInt::get(M->getContext(), APInt(32, StringRef("0"), 10));
            GlobalSig->setInitializer(const_int32_8);
        }
        gSig = GlobalSig;
        
        BBSig.clear();
        errs() << "hello\n" ;
        /* Get the global variable required */
        std::map<BasicBlock*, int> BBSig;
        ++HelloCounter;
        errs() << "Entering function: ";
        errs().write_escaped(F.getName()) << '\n';
        /* block for BB traversal and counting */
        traverse_basic_blocks(F);
        /* get the predecessor information */
        get_basic_block_information(F);
        /* assign and print he static sugnature for basic blocks */
        assign_signature_to_basic_blocks(F);
        print_basic_block_signature_values(F);
        /* Add the signature difference */
        for (std::map<BasicBlock*, int>::iterator itr = Control::BBSig.begin(); itr != Control::BBSig.end(); ++itr) { 
            BasicBlock* current_block = itr->first;
            int current_block_sig = itr->second;
            /* add the sig difference for the current block */
            errs() << "passing " << current_block->getName();
            add_static_difference_and_validation_instruction(current_block, current_block_sig);
        }  
        traverse_basic_blocks(F);

        return true;
    }
    
  private:
    /* variable declarations */
    std::map<BasicBlock*, int> BBSig;
    GlobalVariable* gSig;
    std::map<BasicBlock*, Instruction*> basic_block_to_inst;
    //std::map<BasicBlock*, Value*> D_map;
    /* helper functions prototype declarations */
    void traverse_basic_blocks(Function &F);
    void get_basic_block_information(Function &F);
    void assign_signature_to_basic_blocks(Function &F);
    void print_basic_block_signature_values(Function &F);
    
    /* CFCSS function declarations */
    void add_static_difference_and_validation_instruction(BasicBlock* current_block, int current_block_sig);
    int assign_D_value_to_predecessor_blocks(BasicBlock* bb, bool isFirstPredecessor, int firstBBSig, std::multimap<BasicBlock*, Value*> &D_map);
    Instruction* add_xor_instruction(BasicBlock* bb, Instruction* insert_loc, int sig1, int sig2);
    void add_validate_G_instruction(BasicBlock* bb, IRBuilder<> Builder, Value* xor_with_D, int curr_sig);
    };
  //end of class
}

char Control::ID = 0;
static RegisterPass<Control> Y("Control", "CFCSS Implementation");

void Control::traverse_basic_blocks(Function &F){
    /*
    A function to traverse and count the number of instructions in a basic block
    */
    for (BasicBlock &BB : F){
        /*
        Print out the name of the basic block if it has one, and then the
        number of instructions that it contains
        */
        errs() << "Basic block (name=" << BB.getName() << ") has " << BB.size() << " instructions.\n";
        for (Instruction &I : BB){
            // Print the instruction
            errs() << I << "\n";
        }
    }

}

void Control::get_basic_block_information(Function &F){
    for (BasicBlock &BB : F){
        /* 
        Print out the name of the basic block if it has one, and then the
        number of instructions that it contains
        */
        if(BB.getUniquePredecessor() != NULL){
            errs() << "The unique predecessor is " << BB.getUniquePredecessor()->getName() << "\n"; 
        }
        else{
            if(BB.hasNPredecessors(0)){
                errs() << "The BB has no predecessors\n";
            }
            else{
                errs() << "The block has multiple predecessors as below:\n";
                for(BasicBlock *pred : predecessors(&BB)){
                    errs() << pred->getName() << "\n" ;
              }
            }
        }
    }
}

void Control::assign_signature_to_basic_blocks(Function &F){
    /* 
    assign the signature to basic block 
    */
    int block_index = 1;
    for (BasicBlock &BB : F){
        Control::BBSig.insert(std::make_pair(&BB, block_index++));
    }
    errs() << "The total number of basic blocks in the function " << F.getName() << " is " 
        << block_index - 1 << " \n";
}

void Control::print_basic_block_signature_values(Function &F){
    /*
    print the bb signature values after traversing them
    */
    errs() << "Printing the BB signature map\n"; 
    for (std::map<BasicBlock*, int>::iterator itr = Control::BBSig.begin(); itr != Control::BBSig.end(); ++itr) { 
        errs() << '\t' << itr->first->getName() << '\t' << itr->second << '\n'; 
    }    
}

void Control::add_static_difference_and_validation_instruction(BasicBlock* current_block, int current_block_sig){
    /* 
    ADDING THE SIGNATURE DIFFERENCE
    */
    Instruction* InsertionPointer = (Instruction*)(current_block->getFirstNonPHI());
    IRBuilder<> Builder(InsertionPointer);
    /* 
    Case - 1 - When the the current basic block is the entry block
    */
    if(pred_begin(current_block) == pred_end(current_block)) {
        errs() << "entering " << current_block->getName(); 
        LLVMContext &ctxt = current_block->getParent()->getContext();;
        Type *uintType = Type::getInt32Ty(ctxt);
        Value *constSig = ConstantInt::get(uintType, current_block_sig);
        Builder.SetInsertPoint(current_block->getTerminator());
        Builder.CreateStore(constSig, gSig);
        return;
    }
    /*
    Case - 2 - When the node is a special node
    special node - If the node has multiple incominfg edges from various basic blocks and the path executed is only decided at run time,
                   based on the program logic and input.
                   This is checked by checking for an unique predecessor
    */
    else if(current_block->getUniquePredecessor() == NULL){
        /* 
        Then we have no unique predecessor
        */
        std::multimap<BasicBlock*, Value*> D_map;
        bool isFirstPredecessor = true;
        int firstBBSig = 0;
        firstBBSig = assign_D_value_to_predecessor_blocks(current_block, isFirstPredecessor, firstBBSig, D_map);
        //add all Dvalues to a PHI node since don't know which edge is coming
        Instruction* InsertPt = (Instruction*)(current_block->getFirstNonPHI());
        IRBuilder<> Builder(InsertPt);
        LLVMContext &ctxt = current_block->getParent()->getContext();
        PHINode *PN = Builder.CreatePHI(Type::getInt32Ty(ctxt), 0, "D");
        for(std::multimap<BasicBlock*, Value*>::iterator iter = D_map.begin(); iter != D_map.end(); iter++){
            //errs() << "DMAP - " << iter->second<< iter->first->getName()<<"\n";
            // add an incoming edge and basic block mapping to the phi node
            PN->addIncoming(iter->second, iter->first);
        }
        int current_block_sig = BBSig[current_block];
        int current_block_sig_diff = current_block_sig ^ firstBBSig;
        LoadInst* G = Builder.CreateLoad(gSig);
        Value* init_xor = Builder.CreateXor(G, current_block_sig_diff);
        Value* xor_with_D = Builder.CreateXor(init_xor, PN);
        //check the result
        add_validate_G_instruction(current_block, Builder, xor_with_D, current_block_sig);      
    }
    else{
        /*
        Case - 3
        A normal non entry basic block
        */
        BasicBlock* unique_predecessor = current_block->getUniquePredecessor();
        int current_block_sig_diff = current_block_sig ^ BBSig[unique_predecessor];
        LoadInst* G = Builder.CreateLoad(gSig);
        Value* xor_with_G = Builder.CreateXor(G, current_block_sig_diff);
        add_validate_G_instruction(current_block, Builder, xor_with_G, current_block_sig);
        //update the gSig at last
        Builder.SetInsertPoint(current_block->getTerminator());
        Builder.CreateStore(xor_with_G, gSig);
    }

}
void Control::add_validate_G_instruction(BasicBlock* bb, IRBuilder<> Builder, Value* xor_with_D, int curr_sig){
    /*
    Add a new onstruction that compares the calculated signature value with global variable and G
    */
    Value* curr_sig_const = ConstantInt::get(xor_with_D->getType(), curr_sig);
    Value* cmp_inst = Builder.CreateICmpNE(xor_with_D, curr_sig_const);
    /*
    Remember the current place. add more instructions here
    */
    basic_block_to_inst[bb] = (Instruction*)cmp_inst;
}
int Control::assign_D_value_to_predecessor_blocks(BasicBlock* bb, bool isFirstPredecessor, int firstBBSig, std::multimap<BasicBlock*, Value*> &D_map){
    /*
    Assign a D value to all the bbs in the predecessors
    */
    for(pred_iterator PI = pred_begin(bb); PI != pred_end(bb); ++PI){
        Instruction* firstInstruction;
        BasicBlock* predecessor_block = *PI;
        if(basic_block_to_inst.find(predecessor_block) != basic_block_to_inst.end()){
            firstInstruction = basic_block_to_inst[predecessor_block];
        }
        else{
          firstInstruction = (Instruction*)(predecessor_block->getFirstNonPHI());
        }
        IRBuilder<> Builder(firstInstruction);
        if(isFirstPredecessor){
            isFirstPredecessor = false;
            //choose the first block to store 0 to D
            firstBBSig = BBSig[predecessor_block];
            Instruction* xor_variable = add_xor_instruction(predecessor_block, firstInstruction, firstBBSig, firstBBSig);
            D_map.insert(std::pair<BasicBlock*, Value*>(predecessor_block, (Value*)xor_variable));
        }
        else {
            //for all other preds except the first one
            int currSig = BBSig[predecessor_block];
            Instruction* xor_variable = add_xor_instruction(predecessor_block, firstInstruction, firstBBSig, currSig);
            D_map.insert(std::pair<BasicBlock*, Value*>(predecessor_block, (Value*)xor_variable));
        }      
    }
    return firstBBSig;
}


Instruction* Control::add_xor_instruction(BasicBlock* bb, Instruction* insert_loc, int sig1, int sig2){
  /*
  Add an xor instruction with the given arguments
  */
  IRBuilder<> Builder(insert_loc);
  LLVMContext &ctxt = bb->getParent()->getContext();
  Type* uintTp = Type::getInt32Ty(ctxt);

  Value *constant_sig1 = ConstantInt::get(uintTp, sig1);
  Value *constant_sig2 = ConstantInt::get(uintTp, sig2);
  Instruction* xor_inst = (Instruction*)(Builder.CreateXor(constant_sig1, constant_sig2, "D"));
  return xor_inst;
}