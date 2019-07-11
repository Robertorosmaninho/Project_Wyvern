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


void DepthFirstSearch::reset_visited(Function &F){
  for(BasicBlock &BB : F)
    _visited.insert(std::pair<BasicBlock*, bool>(&BB, false));
}

bool DepthFirstSearch::has_visited(BasicBlock* BB){
 auto it = _visited.find(BB);
 return it->second;
}

void DepthFirstSearch::addBasicBlock(BasicBlock* BB){
  if(!has_visited(BB)){
    _list.push(BB);
    _path.push(BB);
    _visited[BB] = true;
    errs() << BB->getName() << "\n";
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

bool DepthFirstSearch::hasPathWithoutUSe(Value* v){
  std::queue<BasicBlock*> path = _path;
  BasicBlock* entry = path.front();
  path.pop();

  std::stack<BasicBlock*> line;
  line.push(entry);
  while(path.empty()){
    BasicBlock* BB = path.front();
    path.pop();
    
    if(!isSucessor(BB, line.top()))
      path.push(BB);

    if(!hasUse(BB, v) && isReturnBlock(BB) && isSucessor(BB, line.top()))
      return true;
        
    if(!hasUse(BB, v) && isSucessor(BB, line.top()))
      line.push(BB);  
  }


}

bool DepthFirstSearch::runOnFunction(Function &F){
  errs() << "=============================\n";
  errs() << "FunctionName: " << F.getName() << "\n"; 

  BasicBlock* BB = &F.getEntryBlock();
  this->reset_visited(F);
  this->addBasicBlock(BB); 

  do{
    BB = _list.top();
    _list.pop();
    Instruction* I = BB->getTerminator();
//    if(!isa<ReturnInst>(I)){
      for(unsigned i = 0; i < I->getNumOperands(); i++){
        Value *v = I->getOperand(i);
        if(BasicBlock* Sucessor = dyn_cast<BasicBlock>(v)){
          this->addBasicBlock(Sucessor); 
        }
      }
  //  }
  }while(!_list.empty());

  //this->dump();
  this->clear();
  return false;
}

char DepthFirstSearch::ID = 0;
static RegisterPass<DepthFirstSearch> X("DFS", 
"Depth First Search on the Control Flow Graph of the Function", false, false);
