countdown() {
    if [ $1 -gt 0 ]; then
        echo $1
        countdown $(($1-1))
    fi
}
countdown 3