package main
import "fmt"
// single line comment

var logicalValue bool
// global scope variable declaration
var a, b, c, d6, x, y45, g0var, d6Var int
var dubVal float64

func assignments() {	// automatic semi-colon insertion 
	a = 1
	b += 2
	c -= 3
}

func Operators() {
var w, x, y, z, q int
	x = -1
	if x < y {
		x=2
	}
	if x < y && y <= z || z < q && 
	q != x {
	   x = y
	   y = z
	} else {
	   w = x + y * z / q - 4
	   }
	logicalValue = w > 20
}

func WhileStuff() {
var x int
   x = 0
   for ; x < 20; {
	   x += d6
	  return
	  }
}

func RollTheDice() {
	d6Var = 6
	y45 = d6Var
}

func main() {
	RollTheDice()
	fmt.Println("stuff and things")
//	fmt.Scan(&g0var)
	g0var = 2
}

// class syntax
type pet struct {
   hunger int
   happy int
}

func play(p pet) {
   var choice int
   fmt.Println("What should we play?")
   fmt.Println( "Fetch (1) \n Roll over (2)" )
//   fmt.Scan(&choice)
   choice = 2
   if choice == 1 {
      fmt.Println("Fetch, eh")
      p.happy += 10
   } else if choice == 2 {
      fmt.Println("Roll over?  Really?")
      p.hunger += 1
   }else {
      fmt.Println("invalid choice")
   }
}


// class variable declaration
func MakeAPet() {
	// local scope variable declaration
var p pet
   play(p)
   fmt.Printf("%v\n", p.happy)
}

func stringOps() string {
	var S1, S2 string
	S1 = "One"
	S2 = S1 + "Two"	// explicit concatenation
	return S2
}

func listOps() [3]int {
	var L [3]int
	L[0] = 1; L[1] = 2; L[2] = 3
	return L
}
