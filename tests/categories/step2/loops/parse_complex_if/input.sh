if true; then
    if false; then
        echo nested_if_false
    elif true; then
        echo nested_elif_true
    else
        echo nested_else
    fi
else
    echo outer_else
fi