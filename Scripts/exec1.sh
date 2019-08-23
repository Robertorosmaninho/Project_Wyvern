   mkdir DFS
  cd result
  a=1
  for f in $(find . -name "*_DFS_Result.csv"); do
    cp $f det_$a.csv
    mv det_$a.csv ./../DFS
    let a=a+1
  done

cd ../DFS
b=1
sed -n 1p det_1.csv > DFS_Result.csv
for g in $(find . -name "det_*.csv"); do
   sed 1d det_$b.csv >> DFS_Result.csv
   let b=b+1
done
open DFS_Result.csv
