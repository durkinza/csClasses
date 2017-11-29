(* Zane Durkin (durk7832) *)
val username = "durk7832";
val args = CommandLine.arguments();
val filename = hd args;
print filename;
val file = TextIO.openOut filename;
TextIO.output(file, username^"\n");
fun printArg arg = 
	if arg = [] then(
		[]
	) else (
		TextIO.output(file, hd arg ^"\n");
		printArg(tl arg)
	);

printArg args;
TextIO.output(file, username^"\n");
TextIO.closeOut file;
OS.Process.exit(OS.Process.success);
