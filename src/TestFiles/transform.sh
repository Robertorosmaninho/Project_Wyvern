CLANG=~/llvm/build/bin/clang
OPT=~/llvm/build/bin/opt

ProgramName=$1

$CLANG -S -emit-llvm -Xclang -disable-O0-optnone $ProgramName.c -o $ProgramName.ll
$OPT -S -mem2reg $ProgramName.ll -o $ProgramName.ssa.ll

$OPT -dot-postdom $ProgramName.ssa.ll
#dot -Tpdf *.dot -o *.pdf
#open *.pdf 
