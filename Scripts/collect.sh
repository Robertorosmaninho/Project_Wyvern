#!/bin/bash

if [[ $ANALYZE -eq 1 ]]; then
  # this is left as an example for the user to collect their results.

Count=0
Summary=0

if [ -d "result" ]; then
  echo "Removing old result path"
  rm -r result/
fi
  mkdir result
  for f in $(find Benchs -name Details.csv | sed 's|/[^/]*$||'); do
    echo "======================================="
    echo "Count file found!"
    name=$(basename "$(realpath ${f})")
    name=${name%.*}
    if [ -d "result/$name" ]; then
      echo "Moving the file to result/$name"
      mv $f/Details.csv  result/$name/
    else
      echo "Creating result/$name"
      mkdir result/$name
      echo "Moving the file to result/$name"
      mv $f/Details.csv result/$name/
    fi
    let Count++
    echo ""
  done

  for f in $(find Benchs -name Summary.csv | sed 's|/[^/]*$||'); do
    echo "======================================="
    echo "Summary file  found!\n"
    name=$(basename "$(realpath ${f})")
    name=${name%.*}
    if [ -d "result/$name" ]; then
      echo "Moving the file to result/$name"
      mv $f/Summary.csv result/$name/
    else
      echo "Creating result/$name"
      mkdir result/$name
      echo "Moving the file to result/$name"
      mv $f/Summary.csv result/$name/
    fi
    let Summary++
    echo ""
   done

  echo "Count = $Count"
  echo "Summary = $Summary"
fi
