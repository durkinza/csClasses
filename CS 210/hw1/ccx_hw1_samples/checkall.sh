for f in *.output
do
	echo "diff of $f"
	diff "$f" "${f%.output}.lexer.out"
done
