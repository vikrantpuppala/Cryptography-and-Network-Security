#include<bits/stdc++.h>
using namespace std;

#define MATRIX_SIZE 5

pair<int, int> getnextemptypos(pair<int, int> emptypos) {
	if(emptypos.first == -1)
		return make_pair(0, 0);
	emptypos.second++;
	if(emptypos.second == MATRIX_SIZE) {
		emptypos.first++;
		emptypos.second=0;
	}
	return emptypos;
}

string getciphertext(vector<vector<char> > &keytable, map<char, pair<int, int> > &alphabets, char c1, char c2) {
	pair<int, int> pos1 = alphabets[c1], pos2 = alphabets[c2];
	string result="";
	
	//check if in the same row
	if(pos1.first == pos2.first) {
		result+=keytable[pos1.first][(pos1.second+1)%MATRIX_SIZE];
		result+=keytable[pos2.first][(pos2.second+1)%MATRIX_SIZE];
	}
	//or in the same column
	else if(pos1.second == pos2.second) {
		result+=keytable[(pos1.first+1)%MATRIX_SIZE][pos1.second];
		result+=keytable[(pos2.first+1)%MATRIX_SIZE][pos2.second];
	}
	//or find horizontal opposites in rectangle
	else {
		result+=keytable[pos1.first][pos2.second];
		result+=keytable[pos2.first][pos1.second];
	}
	return result;
}

string getdecipheredtext(vector<vector<char> > &keytable, map<char, pair<int, int> > &alphabets, char c1, char c2) {
	pair<int, int> pos1 = alphabets[c1], pos2 = alphabets[c2];
	string result="";
	
	//check if in the same row
	if(pos1.first == pos2.first) {
		int cpos1 = (pos1.second-1)%MATRIX_SIZE, cpos2 = (pos2.second-1)%MATRIX_SIZE;
		if(cpos1<0)
			cpos1+=MATRIX_SIZE;
		if(cpos2<0)
			cpos2+=MATRIX_SIZE;
		result+=keytable[pos1.first][cpos1];
		result+=keytable[pos2.first][cpos2];
	}
	//or in the same column
	else if(pos1.second == pos2.second) {
		int cpos1 = (pos1.first-1)%MATRIX_SIZE, cpos2 = (pos2.first-1)%MATRIX_SIZE;
		if(cpos1<0)
			cpos1+=MATRIX_SIZE;
		if(cpos2<0)
			cpos2+=MATRIX_SIZE;
		result+=keytable[cpos1][pos1.second];
		result+=keytable[cpos2][pos2.second];
	}
	//or find horizontal opposites in rectangle
	else {
		result+=keytable[pos1.first][pos2.second];
		result+=keytable[pos2.first][pos1.second];
	}
	return result;
}

int main(void) {
	string key, plaintext, ciphertext, decipheredtext;
	map<char, pair<int, int> > alphabets;
	vector<char> temp(MATRIX_SIZE, '#');
	vector<vector<char> > keytable(MATRIX_SIZE, temp);
	pair<int, int> emptypos = make_pair(-1, -1);
	
	cout<<"Enter the key you want to use: ";
	getline(cin, key);
	
	for(char c='A'; c<='Z'; c++) 
		alphabets[c] = make_pair(-1, -1);
	
	//fill the key tabled first with the distinct characters in the key
	for(int i=0; i<key.size(); i++) {
		if(!isalpha(key[i]))
			continue;
		key[i] = toupper(key[i]);
		if(key[i] == 'Q')
			continue;
		if(alphabets[key[i]].first == -1) {
			emptypos = getnextemptypos(emptypos);
			alphabets[key[i]] = emptypos;
			keytable[emptypos.first][emptypos.second]=key[i]; 
		}
	}
	
	//fill the key table with the rest of the keys
	for(char c='A'; c<='Z'; c++)  {
		if(c=='Q')
			continue;
		if(alphabets[c].first == -1) {
			emptypos = getnextemptypos(emptypos);
			alphabets[c] = emptypos;
			keytable[emptypos.first][emptypos.second]=c;
		}
	}
	
	cout<<endl<<"The keytable for the given key is"<<endl;
	for(int i=0; i<MATRIX_SIZE; i++) {
		for(int j=0; j<MATRIX_SIZE; j++)
			cout<<keytable[i][j]<<" ";
		cout<<endl;
	}
	
	cout<<endl<<"Enter the plaintext: ";
	getline(cin, plaintext);
	
	//format the plaintext for conversion to ciphertext
	for(int i=plaintext.size()-1; i>=0; i--) {
		if(!isalpha(plaintext[i])) {
			plaintext.erase(i, 1);
			continue;
		}
		plaintext[i] = toupper(plaintext[i]);
		if(plaintext[i] == 'Q')
			plaintext[i] = 'P';
	}
	if(plaintext.size()%2)
		plaintext+='Z';
		
	cout<<endl<<"The formatted plaintext is "<<plaintext<<endl;
	
	for(int i=0; i<plaintext.size(); i+=2) 
		ciphertext+=getciphertext(keytable, alphabets, plaintext[i], plaintext[i+1]);
	
	cout<<endl<<"The cipher text is "<<ciphertext<<endl<<endl;
	cout<<"Applying reverse logic to get deciphered text.."<<endl;
	//deciphering the cipher text
	for(int i=0; i<ciphertext.size(); i+=2)
		decipheredtext+=getdecipheredtext(keytable, alphabets, ciphertext[i], ciphertext[i+1]);
		
	cout<<endl<<"The deciphered text is "<<decipheredtext<<endl<<endl;
}
