i=0
while true; do
    echo $i
    i=$((i+1))
    [ $i -eq 5 ] && break
done