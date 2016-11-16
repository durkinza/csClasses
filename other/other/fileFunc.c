#include <iostream>
#include <fstream>

using namespace std;

int main(){

    fstream fi;
    fstream fo;
    fi.open("test", ios::in);
    fo.open("test", ios::app);
    if(!fo){
     cout << "could not open output file";
    }
    if(!fi){
        cout << "could not open file";
    }else{
	fo << 1000000;
	//cout << fs << endl;
	int x , a, z, y;
	//fs << 100;
	fi >> x >> y >> z >> a;
	cout << x << endl << y << endl << z << endl << a;
	fi.close();
    }
}
