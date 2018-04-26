strindex() {
  x="${1%%$2*}"
  [[ "$x" = "$1" ]] && result=-1 || result="${#x}"
}

gettime() {
  strindex $1 $2
  # echo $result
   || result=0
  if [[ result > -1 ]]; then
    result=${a%%[^0-9]*}
    echo "timer in s"
  else
    echo "timer in ?"
fi
#
  echo $result
}