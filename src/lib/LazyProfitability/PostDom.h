//=====______ PostDom.h - Calculate uses of PostDominatorTree _*_ c++ _*_====//
//
//          Compilers Laboratoy at Federal University of Minas Gerais
//
//===____________________________________________________________________===//
///
///
/// This file contains the declaration of PostDom class which calculates
/// whether the use of a variable post dominates the program entry. 
//===____________________________________________________________________===//

#ifndef SRC_LIB_LAZYPROFITABILITY_POSTDOM_H
#define SRC_LIB_LAZYPROFITABILITY_POSTDOM_H

#include <llvm/Analysis/PostDominators.h>
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"

using namespace llvm;

namespace{
  struct PostDom : public FunctionPass{
    private:
      BasicBlock *_entry, *_end;
      std::vector<BasicBlock> _usesOfVariable;
      std::map<Value*, std::vector<BasicBlock*> > *_var_uses; 
    
    public:
      void set_entry(BasicBlock BB);
      BasicBlock* get_entry();

      void set_end(BasicBlock BB);
      BasicBlock* get_end();
      
      void set_usesOfVarible(BasicBlock BB, Value v);      
      std::vector<BasicBlock*>* get_usesOfVariable(Value v);

      bool VariablePostDominates(PostDominatorTree PDT, BasicBlock *entry, 
                                 std::vector<BasicBlock*> UsesOfVariable);
      static char ID;
      PostDom() : FunctionPass(ID){}
      void getAnalysisUsage(AnalysisUsage &AU) const;
      virtual bool runOnFunction(Function &F);
  };
}
#endif
