#!/usr/bin/env bash
strindex() {
  x="${1%%$2*}"
  [[ "$x" = "$1" ]] && result=-1 || result="${#x}"
}

gettime() {
  strindex $1 "s"
  echo $result
  if [[ result > -1 ]]; then
    #result=$(echo $1 | grep -o -E '[0-9]+')
    #result="${1//[!0-9]/}"
    echo "timer in s" $result
  else
    echo "timer in ?"
  fi
#
  echo $result
}