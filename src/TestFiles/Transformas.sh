OPT=~/llvm/build/bin/opt

ProgramName=$1

dmd $1.d
ldc2 -output-ll $1.d

$OPT -S $ProgramName.ll -o $ProgramName.ssa.ll
$OPT -dot-postdom -dot-regions -dot-cfg -dot-dom $ProgramName.ssa.ll
