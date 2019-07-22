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
   // _list.push(BB);
   // _path.push(BB);
    _visited[BB] = true;
   // errs() << "--->" << BB->getName() << "\n";
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
/*
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
}*/

bool DepthFirstSearch::hasPathWithoutUse(Value* v, Function* F){
  BasicBlock* BB = &F->getEntryBlock();

  if(hasUse(BB, v))
    return false;

  this->reset_visited();
  this->addBasicBlock(BB); 
  bool error = false;
  int ways, bb;

  do{
    BB = _path.back();
    if(error){
      _path.pop();
      BB = _list.top();
      _list.pop();
      error = false;
    }

    Instruction* I = BB->getTerminator();

    if(ReturnInst* ret = dyn_cast<ReturnInst>(I))
      if(!hasUse(BB, v))
        return true;

    for(unsigned i = 0; i < I->getNumOperands(); i++){
      ways = 0, bb = 0;
      Value *V = I->getOperand(i);
      if(BasicBlock* Sucessor = dyn_cast<BasicBlock>(V)){
        if(!hasUse(Sucessor, v)){
          errs() << Sucessor->getName() << "\n";
          this->addBasicBlock(Sucessor);
        }else{
          _list.push(BB);
          ways++;
        }
        bb++;
      } 
    }
    if(ways == bb)
      error = true;
  }while(!_list.empty());

  return false;
}


bool DepthFirstSearch::hasBlockWithoutUse(Value* v, BasicBlock* BB){

  Instruction* I = BB->getTerminator();
  //I->dump(); 
  //errs() << BB->getName() << "\n";
  //errs() << "Terminator: " << BB->getTerminator()->getOpcodeName() << "\n";

  if(hasUse(BB, v)){
   // errs() << BB->getName() << " - HasUse: " << "Yes" << "\n";
    return false;
  }else{
    _path.push(BB);
  }

  if(ReturnInst* ret = dyn_cast<ReturnInst>(I)){
    if(!hasUse(BB, v)){
     // errs() << BB->getName() << " - HasUse: " << "No" << "\n";
      return true;
    }else{
     // errs() << BB->getName() << " - HasUse: " << "Yes" << "\n";
      return false;
    }
  }
  for(unsigned i = 0; i < I->getNumOperands(); i++){
    Value *V = I->getOperand(i);
    if(BasicBlock* Sucessor = dyn_cast<BasicBlock>(V)){
   //   errs() << "--> " << Sucessor->getName() << "\n";
      if(!has_visited(Sucessor)){
        this->addBasicBlock(Sucessor);
        bool verify = this->hasBlockWithoutUse(v, Sucessor);
   //     errs() << Sucessor->getName() << " - HasUse: " << verify << "\n";
        if(verify)
          return true;
      }else{
        errs() << "Sucessor have been visited already!\n";
      }
    }
  }
//  errs() << "###########################\n";
//  errs() << "Passou direto!\n \t BB: " << BB->getName() << "\n \t T: ";
//  I->dump();
  return false;
}



bool DepthFirstSearch::runOnFunction(Function &F){
  errs() << "=============================\n";
  errs() << "FunctionName: " << F.getName() << "\n"; 
  this->set_unvisited(&F);

  for(auto arg = F.arg_begin(); arg != F.arg_end(); ++arg){
    //errs() << "Arg: " << arg->getName() << "\n";
    BasicBlock *BB  = &F.getEntryBlock();
  //  errs() << "\n";
    this->addBasicBlock(BB);
    bool result = hasBlockWithoutUse(arg, BB);
    if(result){
      errs() << "Path without use of " << arg->getName() << ": \n";
      while(!_path.empty()){
        errs() << "\t" << _path.front()->getName() << "\n";
        _path.pop();
      }
    }
    this->reset_visited();
    while(!_path.empty())
      _path.pop();  
  }

//this->dump();
  this->clear();
  return false;
}

char DepthFirstSearch::ID = 0;
static RegisterPass<DepthFirstSearch> X("DFS", 
"Depth First Search on the Control Flow Graph of the Function", false, false);
