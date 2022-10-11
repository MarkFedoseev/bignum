#include <climits>
#include <iostream>
#include <vector>
#include "../bignum/bignum.h"
#include <math.h>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {

	if (!(argc > 1)) {
		cout << "No input file specified \n Abort";
		return -1;
	}

	const string INPUT_FILE = argv[1];
	const string OUTPUT_FILE = argc > 2 ? argv[2] : "bignum_test_output.out";

	bigint bi1;
	bigint bi2;
	ofstream wr;
	ifstream rd;
	rd.open(INPUT_FILE);
	
	
	string bis = "";
	
	string bi1_str = "";
	string bi2_str = "";
	
	string r = "";
	int input_counter = 0;
	while (getline(rd, r)) {
		if (!input_counter)
			bi1_str = r;
		else
			bi2_str = r;
		input_counter++;
	}
	
	rd.close();
	

	bi1 = bigint(bi1_str);
	bi2 = bigint(bi2_str);
	

	bigint sum = bi1 + bi2;
	bigint difference = bi1 - bi2;
	bigint product = bi1 * bi2;
	pair<bigint, bigint> quot = bi1 / bi2;
	

	wr.open(OUTPUT_FILE);
	
	wr << sum.get_string() << "\n";
	wr << difference.get_string() << "\n";
	wr << product.get_string() << "\n";
	wr << quot.first.get_string() << "\n";
	wr << quot.second.get_string() << "\n";
	wr.close();
	
	return 0;
}
