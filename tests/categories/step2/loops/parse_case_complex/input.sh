for i in a b c x y z; do
    case $i in
        a|b|c) echo letter ;;
        x|y|z) echo xyz ;;
        *) echo other ;;
    esac
done