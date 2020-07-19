#include <iostream>
#include "Decrypter.h"

using namespace std;

int main() {
	string salt = "PZ";
	Decrypter d("dict_100000.txt", salt);

	d.setPassword("boston44");

	cout<<"Decrypting: "<<d.getPassword()<<endl;

	//sequential
	d.decryptSeq();

	//parallel
	int numThreads = 4;
	d.decrypt(numThreads);
}
