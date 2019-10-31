package main
import "fmt"

func foo ( x int) {
	return x
}

func main ( ) {
	var in int
	var fl float64
	var bo bool
	var st string


	var a map[string] int
	var b map[int] string
	var c map[float64] bool
	var d map[bool] float64


	var arr_int [20] int;
	var arr_string [20] string
	var arr_bool [20] bool
	var arr_float64 [20] float64

	// testing literal usage in maps
	a["hello"] = 1
	b[1] = "world"
	c[0.3] = false
	d[true] = 1.32


	// testing variable usage
	a[st] = in
	b[in] = st
	c[fl] = bo
	d[bo] = fl

	arr_int[0] = 1
	arr_string[15] = "hello"
	arr_bool[2] = false
	arr_float64[2] = 3.2

	//
	// ERRORS
	// 

	a["hello"] = false
	a[false] = "world"
	a[bo] = st
	a[fl] = in

	arr_int[0] = false
	arr_string[15] = false
	arr_bool[2] = 4
	arr_float64[2] = "hello"

	arr_int[false] = 1
	arr_string[false] = "hello"
	arr_bool["string"] = false
	arr_float64[2.3] = 3.2

	in = foo(1)
}



