//===_____ DFS.h - Depth First Search on a Control Flow Graph ___c++_____===//
//
//          Compilers Laboratoy at Federal University of Minas Gerais
//
//===____________________________________________________________________===//
/// Author: Roberto Rosmaninho
/// Last change: 07/11/19
///
/// This file contains the declaration of Depth First Control class which give
/// us the DFS on a CFG of a given function.
//===____________________________________________________________________===//

#include "DFS.h"

void DepthFirstSearch::getAnalysisUsage(AnalysisUsage &AU) const{
  AU.setPreservesAll();
}


void DepthFirstSearch::set_unvisited(Function *F){
  for(BasicBlock &BB : *F)
    _visited.insert(std::pair<BasicBlock*, bool>(&BB, false));
}

void DepthFirstSearch::reset_visited(){
  for(auto it = _visited.begin(); it != _visited.end(); ++it)
    it->second = false;
}

bool DepthFirstSearch::has_visited(BasicBlock* BB){
 auto it = _visited.find(BB);
 return it->second;
}

void DepthFirstSearch::addBasicBlock(BasicBlock* BB){
  if(!has_visited(BB)){
    _visited[BB] = true;
  }
}

void DepthFirstSearch::clear(){
  while(!_list.empty())
    _list.pop();
  while(!_path.empty())
    _path.pop();
  _visited.erase (_visited.begin(), _visited.end()); 
}

void DepthFirstSearch::dump(){
  std::queue<BasicBlock*> aux = _path;
  errs() << "Printing The Path of DFS:\n";
  for(unsigned i = 0; i < _path.size(); i++){
    BasicBlock* BB = aux.front();
    errs() << BB->getName() << "\n";
    aux.pop();
  }
}

std::queue<BasicBlock*> DepthFirstSearch::get_DFS(){
  return this->_path;
}

bool DepthFirstSearch::hasUse(BasicBlock* BB, Value* V){
  for(Instruction &I : *BB){
    for(unsigned i = 0; i < I.getNumOperands(); i++){
      Value* v = I.getOperand(i);
      if(v == V)
        return true;
    }
  }
  return false;
}


bool DepthFirstSearch::isReturnBlock(BasicBlock* BB){
  if(isa<ReturnInst>(BB->getTerminator()))
    return true;

  return false;
}

bool DepthFirstSearch::isSucessor(BasicBlock* BB,BasicBlock* Sucessor){
  Instruction* I = BB->getTerminator();
  for(unsigned i = 0; i < I->getNumOperands(); i++){
    Value *v = I->getOperand(i);
     if(BasicBlock* Sucessor_real = dyn_cast<BasicBlock>(v))
      if(Sucessor == Sucessor_real)
        return true;
  }
  return false;
}

bool DepthFirstSearch::hasBlockWithoutUse(Value* v, BasicBlock* BB){

  Instruction* I = BB->getTerminator();

  if(hasUse(BB, v))
    return false;
  else
    _path.push(BB);

  if(ReturnInst* ret = dyn_cast<ReturnInst>(I)){
    if(!hasUse(BB, v)){
      return true;
    }else{
      return false;
    }
  }
  for(unsigned i = 0; i < I->getNumOperands(); i++){
    Value *V = I->getOperand(i);
    if(BasicBlock* Sucessor = dyn_cast<BasicBlock>(V)){
      if(!has_visited(Sucessor)){
        this->addBasicBlock(Sucessor);
        bool verify = this->hasBlockWithoutUse(v, Sucessor);
        if(verify)
          return true;
      }else{
        //Sucessor have been visited already!
        continue;
      }
    }
  }
  return false;
}



bool DepthFirstSearch::runOnFunction(Function &F){
  this->set_unvisited(&F);

  for(auto arg = F.arg_begin(); arg != F.arg_end(); ++arg){
    BasicBlock *BB  = &F.getEntryBlock();
    this->addBasicBlock(BB);
    bool result = hasBlockWithoutUse(arg, BB);
    if(result){
      while(!_path.empty()){
        _path.pop();
      }
    }
    this->reset_visited();
    while(!_path.empty())
      _path.pop();  
  }

  this->clear();
  return false;
}

char DepthFirstSearch::ID = 0;
static RegisterPass<DepthFirstSearch> X("DFS", 
"Depth First Search on the Control Flow Graph of the Function", false, false);
