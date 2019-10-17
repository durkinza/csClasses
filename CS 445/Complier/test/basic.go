package main
import (
		"fmt"
		"print"
		)
var x = 0
var y int
var z string = "test"

const a = 0
const c string = "test"

var g,h int

const i, j = 3
const k, l string = "test"

var m [4] int

type person1 struct {
    fname string
}

type person2 struct {
    fname string
    age  int
}
type person3 struct {
    fname string
    lname string
    age  int
}

func main ( ) {
	var d = 0.3
	d = 0.4
}

func inc(y int) int {
	var q int
    return y+h
}

func add(x, y, z int) int {
    return x + y
}
