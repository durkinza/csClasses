package main


// test all single parameters
func oneType_int(x int){}

func oneType_map(x map[string]int){}

func oneType_array(x[12]int){}

// test return types
func oneType_int__returns_int(x int)int{}
func oneType_int__returns_map(x int)map[string]int{}
func oneType_int__returns_array(x int)[32]int{}

// test assigning 2 variables the same type
func oneType_doubleint (x,y int){}

func oneType_doublemap (x,y map[string]int){}

func oneType_doubarray (x,y [21]int){}

// test assigning 7 variables the same type
func oneType_heptint (a,b,c,d,e,f,g int){}

func oneType_heptmap (a,b,c,d,e,f,g map[string]int){}

func oneType_heptarray (a,b,c,d,e,f,g [21]int){}


// test two types of variables
func twoTypes_string_int (z string, y int){}

func twoTypes_int_map (x int, z map[string]int){}

func twoTypes_map_int (z map[string]int, x int){}

func twoTypes_int_array (x int, z [32]int){}

func twoTypes_array_int (x [32]int, z int){}

//  test 2 variables of one type and 1 of another
func twoTypes_doubleInt_map(x,y int, z map[string]int){}

func twoTypes_doubleInt_array (x, y int, z [32]string ){}

func twoTypes_doubeMap_int (x,y map[string]int, z int){}

func twoTypes_doubleArray_int (x, y [32]int, z int){}

// test 2 vairables of each of two types
func twoTypes_doubleInt_doubleInt(x, y int, z, a int){}

func twoTypes_doubleInt_doubleMap(x, y int, z, a map[string]int){}

func twoTypes_doubleInt_doubleArray(x, y int, z, a [32]int){}

func twoTypes_doubleArray_doubleInt(x, y [32]int, z, a int){}

func twoTypes_doubleArray_doubleArray(x, y [32]int, z, a [32]int){}

func twoTypes_doubleArray_doubleMap(x, y [32]int, z, a map[string]int){}

func twoTypes_doubleMap_doubleInt(x, y map[string]int, z, a int){}

func twoTypes_doubleMap_doubleArray(x, y map[string]int, z, a [12]int){}

func twoTypes_doubleMap_doubleMap(x, y map[string]int, z, a map[int]int){}

