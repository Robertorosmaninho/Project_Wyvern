  a=1
  for f in $(find . -name "*_PostDom_Result.csv"); do
    cp $f det_$a.csv
    let a=a+1
  done

b=1
sed -n 1p det_1.csv > PostDom_Result.csv
for g in $(find . -name "det_*.csv"); do
   sed 1d det_$b.csv >> PostDom_Result.csv
   let b=b+1
done
open PostDom_Result.csv
