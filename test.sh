g++ -std=c++17 -o contract contract.cpp || exit 1

for (( i = 1; i < 10; i += 1 )) 
do
	echo $i $(./contract "tests/tiny00$i.gr" "tests/tiny00$i.tww")
done
echo 10 $(./contract "tests/tiny010.gr" "tests/tiny010.tww")
