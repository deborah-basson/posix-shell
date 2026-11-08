i=0
while true; do
    echo $i
    i=$((i+1))
    if [ $i -eq 3 ]; then break; fi
done