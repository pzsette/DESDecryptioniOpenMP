/*
 * Decrypter.h
 *
 *  Created on: Apr 30, 2020
 *      Author: pietro
 */

#ifndef DECRYPTER_H_
#define DECRYPTER_H_

#include <string>
#include <vector>

using namespace std;

class Decrypter {
public:
	Decrypter(const string& dictionaryPath, const string& salt);

	void decrypt(int nThreads);
	void decryptSeq();

	void setPassword(string psw);
	string getPassword();

private:
	vector<string> fullDict;
	string salt;

	string password;
};

#endif /* DECRYPTER_H_ */
