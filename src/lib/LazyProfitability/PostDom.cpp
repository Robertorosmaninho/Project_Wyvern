#include "PostDom.h"

void PostDom::set_entry(BasicBlock *BB){
  this->_entry = BB;
}

BasicBlock* PostDom::get_entry(){
  return this->_entry;
}

void PostDom::set_end(BasicBlock *BB){
  this->_end = BB;
}

BasicBlock* PostDom::get_end(){
  return _end;
}

void PostDom::set_usesOfVarible(BasicBlock *BB, Value v){
  std::map<Value*, std::vector<BasicBlock*>>::iterator it;
  it = this->_var_uses->find(&v);
  if(it != this->_var_uses->end()){
    it->second.push_back(BB);   
  }else{
    std::vector<BasicBlock*> _usesOfVariable;
    _usesOfVariable.push_back(BB); 
    this->_var_uses->insert(std::pair<Value*, std::vector<BasicBlock*> >
                                            (&v, _usesOfVariable));
  }
}

std::vector<BasicBlock*>* PostDom::get_usesOfVariable(Value v){
  std::map<Value*, std::vector<BasicBlock*>>::iterator it;
  it = this->_var_uses->find(&v);
  if(it != this->_var_uses->end()){
    return &it->second;
  }else{
    errs() << "This variable doesn`t have any uses on the program\n";
    return nullptr;
  }
} 

bool PostDom::VariablePostDominates(PostDominatorTree PDT, BasicBlock *entry, 
                                    std::vector<BasicBlock*> UsesOfVariable){
  int _post_dom_count = 0;
  for(int i = 0; i != UsesOfVariable.size(); i++){
    BasicBlock *BB = UsesOfVariable[i];
    DomTreeNodeBase<BasicBlock> *x = PDT.getNode(entry);
    DomTreeNodeBase<BasicBlock> *y = PDT.getNode(BB);
    if(PDT.dominates(x, y))
      _post_dom_count++;
  }

  if(_post_dom_count)
    return true;
  else
    return false;
}

