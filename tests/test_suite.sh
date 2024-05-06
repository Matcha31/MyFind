#!/bin/sh

cd $(dirname "$0")

test_myfind ()
{
    cd ./filetest/
    arg="$1"
    ../../myfind $arg > ../out 2> /dev/null
    find $arg > ../src 2> /dev/null
    cd ..
    d="$(diff -u out src)"
    ret=$?
    if [ $ret -eq 0 ]; then
        echo SUCCESS
    else
        echo FAILED wit args $arg
    fi
    rm src out
}

echo "\n\nTEST MYFIND\n"
test_myfind ""
test_myfind "-print"
test_myfind "-name bar"
test_myfind "-name \"bar\""
test_myfind "-name ba*"
test_myfind "-name bar -a -name bar"
test_myfind "\\! -name bar"
test_myfind "bar -type f"
test_myfind "-name bar -o -name baz"
test_myfind "\\( -name bar -o -name baz \\)"
test_myfind "\\! \\( \\( -name b* -a -name f* \\) \\! \\( -name *.c -name *.h \\) \\)"
test_myfind "-user matcha"
test_myfind "-name *.c -o -name bar"
test_myfind "-perm 755"
test_myfind "-perm 458"
test_myfind "-perm 856"
test_myfind "-perm 590"
test_myfind "-perm 644"
test_myfind "-perm 000"
test_myfind "-perm 755 -a -type d"
test_myfind "-type f -a -name ba*"
test_myfind "-type d"
test_myfind "-type f"
test_myfind "-type a"
test_myfind "-type ff"
test_myfind "-type l"
test_myfind "foo -print"
test_myfind "foo -name bar -name baz"
test_myfind "-type f,d,y"
test_myfind "-type f,d"
test_myfind "-perm -100"
test_myfind "-perm /100"
test_myfind "-perm -458"
test_myfind "-perm 458"
test_myfind "-group matcha"
test_myfind "tests/filetest/foo -group matcha"
test_myfind "-user salome.berger"
test_myfind "-group nonsense"
test_myfind "-user nonsense"
test_myfind "! -name src"
test_myfind "-print -a ! -type d -a -print"
test_myfind "-print -a -print -a -type f -a ! -type d -o ! -type f -a -print"
test_myfind "\\( -print \\)"
test_myfind "\\( -name bar \\)"
test_myfind "\\( \\( -print \\) -a \\( -type f -o -type d \\) -a \\( ! -name bar -a ! -name bidule \\) \\)"
