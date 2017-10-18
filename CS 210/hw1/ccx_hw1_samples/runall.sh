for f in *.ccx
do
	../a.out "$f" > $f.output
done
for f in *.cci
do
	../a.out "$f" > $f.output
done
