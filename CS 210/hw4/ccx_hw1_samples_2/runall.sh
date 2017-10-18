for f in *.ccx
do
	../ccx_start "$f" > $f.output
done
for f in *.cci
do
	../ccx_start "$f" > $f.output
done
