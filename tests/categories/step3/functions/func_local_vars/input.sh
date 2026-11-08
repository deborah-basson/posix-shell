f() {
    X=local
    echo $X
}
X=global
f
echo $X