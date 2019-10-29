package main
var v bool
var w float64
var x int
var y string
const z = 0

func add (){
	var a = 0
	var b = 1
	var c,d int

	var e = ""
	var f = "world"
	var g,h string

	var i = 0.0
	var j = 2.2
	var k,l float64

	var m = false
	var n = true
	var o,p bool

	// assignment
	x = 2
	y = "hello"
	v = true
	w = 9.9

	// addition
	x = 1 + 2
	x = a + 1
	x = c + 2
	x = a + b
	x = c + d
	x = a + d

	// concatination
	y = "hello" + "world"
	y = e + "test"
	y = h + "test"
	y = e + f
	y = g + h
	y = e + h

	// multiplication
	x = 1 * 1
	x = a * 1
	x = 1 * a
	x = c * 1
	x = 1 * c
	x = a * b
	x = c * d
	x = a * c
	x = c * a

	i = 1.2 * 4.2
	i = k * 3.2
	i = 3.2 * j
	i = i * j
	i = k * j
	i = i * l
	i = k * j


	// divide
	x = 1 / 1
	x = a / 1
	x = 1 / c
	x = a / b
	x = c / d

	i = 92.3 / 4.2
	i = k / 0.9
	i = 1.3 / j
	i = i / j
	i = k / l
	i = i / l
	i = k / j

	// BOOL
	v = true
	v = m
	v = a < b

	// OR
	v = true || true
	v = true || n
	v = m || true
	v = true || o
	v = p || true
	v = m || n
	v = n || m
	v = n || a < b

	// ANDAND
	v = true && true
	v = true && n
	v = true && p
	v = n && true
	v = o && false
	v = n && m
	v = o && p
	v = n && p
	v = o && m

	// increment
	x += 2
	i += 4.2
	y += "world"

	// decrement
	x -= 1
	i -= 3.2


	//
	// ERRORS
	//

	// int + string
	x + y
	// string + int
	y + x
	// float + int
	w + x
	// int + float
	x + w
	// float + string
	w + y
	// string + float
	y + w

	// int * float
	x * w
	// float * int
	w * x
	// string * int
	y * x
	// int * string
	x * y
	// float * string
	w * y
	// string * float
	y * w


	// division by zero
	x / 0
	// int / float
	x / w
	// float / int
	w / x
	// string / int
	y / x
	// int / string
	x / y
	// string / float
	y / w
	// float / string
	w / y

	// int = float64
	x = w
	// float64 = int
	w = x
	// int = string
	x = y
	// string = int
	y = x
	// string = float
	y = w
	// float = string
	w = y
	// bool = int
	v = x

	// integer to boolean error
	v = 1
	// string || int
	v || 1
	// int || string
	v || y
	// int || int
	1 || v

	// bool && int
	v && 1
	// bool && int
	v && x
	/// bool && string
	v && y
	// string && bool
	y && v

	// literal = int
	1 = x

	// error here of assignemnt to const
	z = 1
	z = y

}
