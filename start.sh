#!/bin/bash

echo \
"Hosszu
horolas
rugyfakaszto permetezes
mettszes
tavaszi nyitas
Malom telek
horolas
rugyfakaszto permetezes
mettszes
tavaszi nyitas
Szula
horolas
tavaszi nyitas
rugyfakaszto permetezes
mettszes
Malom telek
horolas
tavaszi nyitas
rugyfakaszto permetezes
mettszes
" \
> resources/vineyard_tasks.txt

rm resources/entries.bin
touch resources/entries.bin

./Main