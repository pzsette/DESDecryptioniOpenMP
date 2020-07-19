/*
 * Decrypter.cpp
 *
 *  Created on: Apr 30, 2020
 *      Author: pietro
 */


#include "Decrypter.h"
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <crypt.h>
#include <string>
#include <omp.h>
#include <chrono>
#include <iostream>


using namespace std;

Decrypter::Decrypter(const string& dictionaryPath, const string& salt) {
	this-> salt = salt;

	string line;
	ifstream dictionary(dictionaryPath);

	if(!dictionary) {
		throw runtime_error("Could not open file");
	}

	while (std::getline(dictionary, line)) {
		fullDict.push_back(line);
	}
}

void Decrypter::decrypt(int nThreads) {
	cout<<"PARALLEL DECRYPTION"<<endl;

	volatile bool found = false;
	string psw = "";
	chrono::duration<double> timePassed;
	auto start = chrono::high_resolution_clock::now();

	#pragma omp parallel num_threads(nThreads)
		{
			struct crypt_data data;
			data.initialized = 0;

		#pragma omp for schedule(static)
			for (int i=0; i < fullDict.size(); i++) {

				if (found == true) {continue;}

				char *cwe = crypt_r(fullDict[i].c_str(), salt.c_str(), &data);
				if (strcmp(cwe, password.c_str())==0) {
					found = true;
					psw = fullDict[i];
				}
			}
		}

		if (found==true) {
			auto end = chrono::high_resolution_clock::now();
			timePassed = end - start;
			cout<<"PASSWORD FOUND:"<<psw<<" TIME: "<<timePassed.count()<<endl;
		} else {
			cout<<"PASSWORD NOT FOUND"<< endl;
		}
}

void Decrypter::decryptSeq() {
	cout<<"SEQUENTIAL DECRYPTION"<<endl;
	string psw = "";
	chrono::duration<double> timePassed;
	int i=0;
	bool found=false;
	auto start = chrono::high_resolution_clock::now();
	while(!found && i<fullDict.size()) {

		char *cwe = crypt(fullDict[i].c_str(), salt.c_str());

		if (strcmp(cwe, password.c_str())==0) {
			found = true;
			auto end = chrono::high_resolution_clock::now();
			timePassed = end - start;
			psw = fullDict[i];
		} else {
			i++;
		}
	}

	if (found==true) {
		cout<<"PASSWORD FOUND:"<<psw<<" TIME: "<<timePassed.count()<<endl;
	} else {
		cout<<"PASSWORD NOT FOUND"<< endl;
	}
}

void Decrypter::setPassword(string psw) {
	this->password = strdup(crypt(psw.c_str(), salt.c_str()));
}

string Decrypter::getPassword() {
	return this->password;
}
