#!/bin/bash

a=letter_of_alphabet
letter_of_alphabet=z

echo

echo "a=$a"

eval a=\$$a
echo "Now a=$a"
echo

t=table_cell_3
table_cell_3=24
echo "\"table_cell_3\"=$table_cell_3"
echo -n "dereferenced \"t\" ="
eval echo \$$t

exit 0
