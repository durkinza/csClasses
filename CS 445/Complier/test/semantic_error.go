package main

var z int

func add(x, y int){
	var z int
	return x+g // error is here
}

func main(){
	z = 2
	add(1, z)
}


