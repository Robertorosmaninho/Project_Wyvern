#!/bin/bash

if [[ $DIFF -eq 1 ]]; then
	############################## Merge Files ##############################
  cd result/
#Creates a temporary directory to receive the prof files and compile into one
  if [ -d "Count" ]; then
    rm -rf Count
    mkdir Count
  else
    mkdir Count
  fi

  #Find, copy, rename and move each prof.csv to the temporary directory
  a=1
  for f in $(find . -name Details.csv); do
    cp $f det_$a.csv
    mv det_$a.csv ./Count
    let a=a+1
  done

  echo "Creating a Big Count file"
  cd Count
  sed -n 1p det_1.csv > Count_total.csv
  sed 1d det_*.csv >> Count_total.csv
  cp Count_total.csv ./../
  cd ..
  rm -r Count

   #Creates a temporary directory to receive the prof files and compile into one
  if [ -d "Summary" ]; then
    rm -rf Summary
    mkdir Summary
  else
    mkdir Summary	
	fi

  #Find, copy, rename and move each prof.csv to the temporary directory
	a=1
	for s in $(find . -name Summary.csv); do
    cp $s s$a.csv
		mv s$a.csv ./Summary
		let a=a+1
	done

	echo "Creating a Big Summary file"
	cd Summary 
	sed -n 1p s1.csv > Summary_total.csv
	sed 1d s*.csv >> Summary_total.csv
	cp Summary_total.csv ./../
	cd ..
	rm -r Summary

fi
