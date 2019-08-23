if [ -d DFS ]; then
 	rm -r DFS
  mkdir DFS
else
	mkdir DFS
fi

if [ ! -d result ]; then
	echo "Result not found!"
	exit 1
fi

if [ -d DFS ]; then
	rm -r DFS
	mkdir DFS
else
	mkdir DFS
fi

if [ -d Summary ]; then
	rm -r Summary
	mkdir Summary
else
	mkdir Summary
fi

cd result

a=1
for f in $(find . -name "*_DFS_Result.csv" ); do
	cp $f det_$a.csv
	mv det_$a.csv ../DFS
	let a=a+1
done

b=1
echo "Compiling all DFS Results into a single file!"
cd ../DFS
sed -n 1p det_1.csv > DFS_total.csv 
for g in $(find . -name "det_*.csv"); do
	sed 1d det_$b.csv >> DFS_total.csv
	let b=b+1
done

cp DFS_total.csv ../
cd ../result

a=1
for f in $(find . -name "Summary_*.csv" ); do
  cp $f sum_$a.csv
  mv sum_$a.csv ../Summary
  let a=a+1
done

b=1
echo "Compiling all Summary Results into a single file!"
cd ../Summary
sed -n 1p sum_1.csv > Summary_total.csv 
for g in $(find . -name "sum_*.csv"); do
  sed 1d sum_$b.csv >> Summary_total.csv
  let b=b+1
done

cp Summary_total.csv ../
cd ..
rm -r DFS Summary


