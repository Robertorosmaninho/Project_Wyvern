#Usage: This is a simple bash that runs into a single path, this bash is able to input a file for execute the program just use "./exec.sh < YourInputHere" 

LLVM_PATH="${HOME}/llvm/build/bin"
COMPILER=$LLVM_PATH/clang++
OPT=$LLVM_PATH/opt
COMPILE_FLAGS= 
RUN_OPTIONS=
BASEDIR="$(pwd)"
JOBS=1
CHLORIS="$BASEDIR/../../.."
suffix="dylib"
INPUT=$1

if [[ $(uname -s) == "Linux" ]]; then
  suffix="so"
fi

bench_name=$(basename "$BASEDIR").bc


function clean() {
  echo "Removing old files"
  rm -f *.bc
  rm -f *.ibc
  rm -f *.rbc
  rm -f *.o
  rm -f *.exe
  rm -f *.txt
	rm -f *.csv
}

function compile(){
  echo "Compiling"
  for f in $(find . -name '*.cpp'); do 
    $COMPILER -I -S -c -emit-llvm $COMPILER_FLAGS  $f -o ${f%.*}.bc 
    $OPT -S ${f%.*}.bc -o ${f%.*}.ll
  done
 	
  echo "Generating a big bc"
  #Generate all bcs into a big bc
  $LLVM_PATH/llvm-link -S *.bc -o $bench_name
  $OPT -S $bench_name  -o $bench_name

  #$LLVM_PATH/llc -filetype=obj $prf_name -o $obj_name	
  #echo "Executing parall"
  #parallel --tty --jobs=4  $COMPILER $COMPILE_FLAGS \
  #  -Xclang -disable-O0-optnone \
  #  -g -S -c -emit-llvm {} -o {.}.bc ::: *.bc
}

function instrument(){
libLazyProfitability=${HOME}/Desktop/ESCOLAS/LAC/D_Project/Project_Wyvern/src/build/lib/LazyProfitability/libLazyProfitability.dylib
	
 
  echo "Intrumenting"
  # Optmize
  $OPT -S -load $libLazyProfitability -LazyProf  $bench_name -o ${bench_name%.*}.ibc

  echo "Compiling to .o"
  # Compile our instrumented file, in IR format, to x86:
  $LLVM_PATH/llc -filetype=obj ${bench_name%.*}.ibc -o ${bench_name%.*}.o
}

function run(){

	echo "Compiling all the program"
	$COMPILER ${bench_name%.*}.o -o ${bench_name%.*} -L. -lcd
	
	echo "Executing"
  if [ -z "$INPUT" ] ; then
	./${bench_name%.*} $RUN_OPTIONS
  else 
	echo "${bench_name%.*} input < $INPUT"
	./${bench_name%.*} < $INPUT
  fi
}

function collect() {
  echo "Collecting *.csv"	
  directory_name=${bench_name%.*}_results

  if [ -d "$directory_name" ]; then
    mkdir -p $directory_name
    mv prof.csv static.csv $directory_name
  fi
}

clean ;
compile ;
instrument ;
#run ;
#collect ;
