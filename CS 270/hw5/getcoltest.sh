echo
echo "getcol 1 2 3 < getcoltest.txt"
getcol 1 2 3 < getcoltest.txt

echo
echo "getcol 1 1 2 2 < getcoltest.txt"
getcol 1 1 2 2 < getcoltest.txt

echo
echo "getcol 3 2 1 < getcoltest.txt"
getcol 3 2 1 < getcoltest.txt

echo
echo "getcol 1 100 < getcoltest.txt"
getcol 1 100 < getcoltest.txt

echo
echo "getcol 7 < getcoltest.txt"
getcol 7 < getcoltest.txt

echo
echo "getcol 700 < getcoltest.txt"
getcol 700 < getcoltest.txt

echo
echo "getcol -- 0 -1 < getcoltest.txt"
getcol -- 0 -1 < getcoltest.txt

echo
echo "getcol 1:100 < getcoltest.txt"
getcol 1:100 < getcoltest.txt

echo
echo "getcol 1:1 2:2 < getcoltest.txt"
getcol 1:1 2:2 < getcoltest.txt

echo
echo "getcol 1:2 2:3 < getcoltest.txt"
getcol 1:2 2:3 < getcoltest.txt

echo
echo "getcol 1:4 10:4 < getcoltest.txt"
getcol 1:4 10:4 < getcoltest.txt

echo
echo "getcol 1:100:2 < getcoltest.txt"
getcol 1:100:2 < getcoltest.txt

echo
echo "getcol 2:100:2 < getcoltest.txt"
getcol 2:100:2 < getcoltest.txt

echo
echo "getcol 2:100:3 < getcoltest.txt"
getcol 2:100:3 < getcoltest.txt

echo
echo "getcol 1:4 10:4:-1 < getcoltest.txt"
getcol 1:4 10:4:-1 < getcoltest.txt

echo
echo "getcol 3 4:5 6:8:2 1 2 < getcoltest.txt"
getcol 3 4:5 6:8:2 1 2 < getcoltest.txt

echo
echo "getcol -it -oT 1:10 < getcoltest.txt"
getcol -it -oT 1:10 < getcoltest.txt

echo
echo "getcol -i' '  1:10 < getcoltest.txt"
getcol -i' '  1:10 < getcoltest.txt

echo
echo "getcol 5:7 < getcoltestshort.txt"
getcol 5:7 < getcoltestshort.txt

echo
echo "tr ' ' ',' < getcoltest.txt | getcol -i','  1:10"
tr ' ' ',' < getcoltest.txt | getcol -i','  1:10

echo
echo "tr ' ' ',' < getcoltest.txt | getcol -i',' -o' : '  1:10"
tr ' ' ',' < getcoltest.txt | getcol -i',' -o' : '  1:10

echo
echo "sed 's/ /  :   /g' < getcoltest.txt | getcol -i':' -o';' 1:10"
sed 's/ /  :   /g' < getcoltest.txt | getcol -i':' -o';' 1:10

echo
echo "sed 's/ /  :   /g' < getcoltest.txt | getcol -i':' -o';' -s 1:10"
sed 's/ /  :   /g' < getcoltest.txt | getcol -i':' -o';' -s 1:10

echo
echo "sed 's/ /  :   /g' < getcoltest.txt | getcol -i':' -o' & ' -l' \\' -s 1:10"
sed 's/ /  :   /g' < getcoltest.txt | getcol -i':' -o' & ' -l' \\' -s 1:10

