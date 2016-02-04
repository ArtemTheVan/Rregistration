#!/bin/bash

for s in $(ls|grep esu); do
mv $s ${s:4}
done