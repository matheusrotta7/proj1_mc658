#!/bin/bash
g++ flowshop.cpp -o bnb-fs -g
index=0
letter_i="i"
#!/bin/bash
for filename in ../Instancias/i*; do
    echo "running bnb on file $filename"
    # echo "./bnb-fs" "$filename" "param.in" ">" "$filename)_result"
    ./bnb-fs $filename param.in > $filename'_result'
done





###"Logs/$(basename "$filename" .txt)_Log$i.txt"
