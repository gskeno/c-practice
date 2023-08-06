# !/bin/bash
for dir in `ls` ;
do
	echo $dir

    if test -e "${dir}.c"; then
      echo "${dir}"
      rm "${dir}"
    fi

    if test -e "${dir}.dSYM"; then
      echo "${dir}.dSYM"
      rm -r "${dir}.dSYM"
    fi

done

cd udp  
for dir in `ls` ;
do
	echo $dir

    if test -e "${dir}.c"; then
      echo "${dir}"
      rm "${dir}"
    fi

    if test -e "${dir}.dSYM"; then
      echo "${dir}.dSYM"
      rm -r "${dir}.dSYM"
    fi

done