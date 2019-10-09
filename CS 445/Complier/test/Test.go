package main
import "math/rand"

func add(x int, y int) int {
    return x + y
}

func main() {
	var dd float64
	const yy = 1
	dd = 0.0
	dd += 1
	fmt.Println(dd)
	fmt.Println(add(42, 13))
	const Pi float64 = 3.14
	var i, sum int
	sum = 0
	for i = 0; i < 10; i++ {
		sum += i
	}
	fmt.Println(sum)

	sum = 1
	for sum < 1000 {
		sum += sum
	}

	// infinite loop
	//for {
	//}

	if dd < 0 {
	   fmt.Println(add(42, 13))
	} else if dd < 10 {
	   fmt.Println(add(42, 13))
	} else {
	   fmt.Println(add(42, 13))
	} 
}



type person struct {
    name string
    age  int
}

func addtwo(x, y int) int {
	var z = 2
	x == z
	x != z
	x &&  z
	x ||  z
	x =  z
	x +=  z
	x ++
	x -=  z
	x --
	x <  z
	x <=  z
	x > z
	x >= z
	x -  z
	x +  z
	x %  z
	x /  z
	x *  z
	x !=  z
	x.z
	var b = 1
	var c = 1
	for x; x<z; z++ {}
	for y; y>=2; x+=3 {}
	return x + y
	var a [32]int

	var m map[string]int
	m["Bell Labs"] = int{ 40.68433, -74.39967, }
	m["Google"] = int{37.42202, -122.08408 }
}
