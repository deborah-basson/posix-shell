x=0
while [ $x -lt 3 ]; do
    y=0
    while [ $y -lt 2 ]; do
        echo $x$y
        y=$((y+1))
    done
    x=$((x+1))
done