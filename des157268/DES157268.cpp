#include<bits/stdc++.h>
using namespace std;

#define NUM_OF_ROUNDS 16
#define PC1_KEY_SIZE 56
#define PLAINTEXT_BLOCK_SIZE 64
#define SUBKEY_SIZE 48

int toInt(string binText)
{
	int p = 1;
	int ans = 0;
	for( int i=binText.length()-1;i>=0;i-- )
	{
		if(binText[i]=='1')
			ans+=p;
		p = p*2;
	}
	
	return ans;
}

string toBinary(int x, int len=8)
{
	int x1 = x;
	string str = "";
	for( int i=0;i<len;i++ )
	{
		if(x%2==1)
			str += '1';
		else
			str += '0';
		x /= 2;
	}
	if(x!=0)
		throw "Character not completely converted into binary";
	
	reverse(str.begin(), str.end());
	
	if(toInt(str)!=x1)
	{
		ostringstream errMsg;
		errMsg<<"Binary representation not consistent:\nOriginal Character : ";
		errMsg<<x1;
		errMsg<<"\nConverted from binary representation : ";
		errMsg<<toInt(str)<<endl;
		
		string errStr = errMsg.str();
		
		char *cerrMsg = new char[errStr.length()+5];
		strcpy(cerrMsg, errStr.c_str());
		
		return cerrMsg;
	}
	
	return str;
}

string getBitRepresentation(string plaintext) {
	string bits_plaintext="";
	
	for(int i=0;i<plaintext.length();i++)
		bits_plaintext += toBinary((unsigned char)plaintext[i]);
	
	return bits_plaintext;
}

string initialPermutation(string plaintext) {
	int permPos[] = {
						58, 50, 42, 34, 26, 18, 10, 2, 
						60, 52, 44, 36, 28, 20, 12, 4, 
						62, 54, 46, 38, 30, 22, 14, 6, 
						64, 56, 48, 40, 32, 24, 16, 8, 
						57, 49, 41, 33, 25, 17, 9, 1, 
						59, 51, 43, 35, 27, 19, 11, 3, 
						61, 53, 45, 37, 29, 21, 13, 5, 
						63, 55, 47, 39, 31, 23, 15, 7
					};
	
	string ans = "";
	for( int i=0;i<64;i++ )
		ans += plaintext[ permPos[i]-1 ];
	return ans;
}


string permutedChoiceOne(string key) {
	string pc1_key = "";
	int perm_pos[] = {
						57, 49, 41, 33, 25, 17, 9, 
						1, 58, 50, 42, 34, 26, 18, 
						10, 2, 59, 51, 43, 35, 27, 
						19, 11, 3, 60, 52, 44, 36,
						63, 55, 47, 39, 31, 23, 15, 
						7, 62, 54, 46, 38, 30, 22, 
						14, 6, 61, 53, 45, 37, 29, 
						21, 13, 5, 28, 20, 12, 4
					};
	
	for( int i=0;i<56;i++ )
		pc1_key += key[ perm_pos[i]-1 ];
	
	return pc1_key;
}

string permutedChoiceTwo(string key) {
	string newKey = "";
	int permPos[] = {
						14, 17, 11, 24, 1, 5, 3, 28, 
						15, 6, 21, 10, 23, 19, 12, 4, 
						26, 8, 16, 7, 27, 20, 13, 2, 
						41, 52, 31, 37, 47, 55, 30, 40, 
						51, 45, 33, 48, 44, 49, 39, 56, 
						34, 53, 46, 42, 50, 36, 29, 32
					};
	
	for( int i=0;i<48;i++ )
		newKey += key[ permPos[i]-1 ];
	
	return newKey;
}

vector<string> subkeyGeneration(string pc1_key) {
	int bits_to_rotate[NUM_OF_ROUNDS] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
	
	vector<string> subkeys;
	for(int i=0; i<NUM_OF_ROUNDS; i++) {
		string left_part = pc1_key.substr(0, PC1_KEY_SIZE/2), right_part = pc1_key.substr(PC1_KEY_SIZE/2, PC1_KEY_SIZE/2);
		left_part = left_part.substr(bits_to_rotate[i], left_part.size()-bits_to_rotate[i]) + left_part.substr(0, bits_to_rotate[i]);
		right_part = right_part.substr(bits_to_rotate[i], right_part.size()-bits_to_rotate[i]) + right_part.substr(0, bits_to_rotate[i]);
		pc1_key = left_part + right_part;
		subkeys.push_back(permutedChoiceTwo(pc1_key));
	}
	return subkeys;
}

string expansion(string right_part) {
	string ans = "";
	int permPos[] = {
						32, 1, 2, 3, 4, 5, 
						4, 5, 6, 7, 8, 9, 
						8, 9, 10, 11, 12, 13, 
						12, 13, 14, 15, 16, 17, 
						16, 17, 18, 19, 20, 21, 
						20, 21, 22, 23, 24, 25, 
						24, 25, 26, 27, 28, 29, 
						28, 29, 30, 31, 32, 1
					};
	
	for( int i=0;i<48;i++ )
		ans += right_part[ permPos[i]-1 ];
	return ans;
}

string stringXOR(string str1, string str2)
{
	for( int i=0;i<str1.length();i++ )
		str1[i] = (str1[i]+str2[i]-'0'-'0')%2 + '0';
	
	return str1;
}

string substitution(string text) {
	int sbox[][64] = 	{
							{
								14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7, 
								0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8, 
								4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0, 
								15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13
							},
							{
								15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10, 
								3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5, 
								0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15, 
								13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9
							},
							{
								10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8, 
								13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1, 
								13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7, 
								1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12
							},
							{
								7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15, 
								13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9, 
								10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4, 
								3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14
							},
							{
								2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9, 
								14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6, 
								4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14, 
								11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3
							},
							{
								12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11, 
								10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8, 
								9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6, 
								4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13
							},
							{
								4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1, 
								13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6, 
								1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2, 
								6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12
							},
							{
								13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7, 
								1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2, 
								7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8, 
								2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
							}
						};
	
	string ans = "";
	
	for( int i=0;i<text.length();i+=6 )
	{
		int x = toInt( text.substr(i, 1)+text.substr(i+5, 1) );
		int y = toInt( text.substr(i+1, 4) );
		
		int new_val = sbox[i/8][x*16+y];
		ans += toBinary(new_val, 4);
	}
	
	return ans;
}

string permutation(string right_part) {
	int permPos[] = {
						16, 7, 20, 21, 29, 12, 28, 17, 
						1, 15, 23, 26, 5, 18, 31, 10, 
						2, 8, 24, 14, 32, 27, 3, 9, 
						19, 13, 30, 6, 22, 11, 4, 25
					};
	
	string ans = "";
	for( int i=0;i<32;i++ )
		ans += right_part[ permPos[i]-1 ];
	
	return ans;
}

string inversePermutation( string text )
{
	int permPos[] = {
						40, 8, 48, 16, 56, 24, 64, 32, 
						39, 7, 47, 15, 55, 23, 63, 31, 
						38, 6, 46, 14, 54, 22, 62, 30, 
						37, 5, 45, 13, 53, 21, 61, 29, 
						36, 4, 44, 12, 52, 20, 60, 28, 
						35, 3, 43, 11, 51, 19, 59, 27, 
						34, 2, 42, 10, 50, 18, 58, 26, 
						33, 1, 41, 9, 49, 17, 57, 25, 
					};
	
	string ans = "";
	for( int i=0;i<64;i++ )
		ans += text[ permPos[i]-1 ];
	return ans;
}

string toText(string str)
{
	string ans = "";
	
	for( int i=0;i<str.length();i+=8 )
		ans += (unsigned char)toInt( str.substr(i, 8) );
	return ans;
}

string roundFunction(string right_part, string subkey) {
	right_part = expansion(right_part);
	right_part = stringXOR(right_part, subkey);
	right_part = substitution(right_part);
	right_part = permutation(right_part);
	return right_part;
}

string desRound(string plaintext_block, string subkey) {
	string left_part = plaintext_block.substr(0, PLAINTEXT_BLOCK_SIZE/2), right_part = plaintext_block.substr(PLAINTEXT_BLOCK_SIZE/2, PLAINTEXT_BLOCK_SIZE/2);
	string left_result = right_part, right_result;
	right_part = roundFunction(right_part, subkey);
	right_result = stringXOR(left_part, right_part);
	return left_result+right_result;
}

string leftRightSwap(string block) {
	return block.substr(PLAINTEXT_BLOCK_SIZE/2, PLAINTEXT_BLOCK_SIZE/2) + block.substr(0, PLAINTEXT_BLOCK_SIZE/2);
}

string des(string bits_plaintext, string bits_key, bool decipher) {
	string ip_plaintext, pc1_key, block_ciphertext, ciphertext;
	
	pc1_key = permutedChoiceOne(bits_key);
	vector<string> subkeys = subkeyGeneration(pc1_key);
	
	for(int block=0; block<bits_plaintext.size()/64; block++) {
//		cout<<endl<<endl<<"CONSIDERING BLOCK "<<block+1<<" OF PLAINTEXT"<<endl;
		string plaintext_block = bits_plaintext.substr(PLAINTEXT_BLOCK_SIZE*block, PLAINTEXT_BLOCK_SIZE);
//		cout<<"Plaintext block: "<<plaintext_block<<endl;
		
		ip_plaintext = initialPermutation(plaintext_block);
//		cout<<"Initial Permutation of plaintext block: "<<ip_plaintext<<endl;
		block_ciphertext = ip_plaintext;
		for(int i=0; i<NUM_OF_ROUNDS; i++) {
			string subkey;
			if(decipher == true)
				subkey = subkeys[NUM_OF_ROUNDS-i-1];
			else 
				subkey = subkeys[i];
//			cout<<"Subkey for this round: "<<subkey<<endl;
			block_ciphertext = desRound(block_ciphertext, subkey);
//			cout<<"Output of round "<<i+1<<" is "<<block_ciphertext<<endl;
		}
		block_ciphertext = leftRightSwap(block_ciphertext);
		block_ciphertext = inversePermutation(block_ciphertext);
		
		ciphertext+=block_ciphertext;
	}
	return ciphertext;
}

int main() {
	string plaintext, key, bits_plaintext, bits_key;
	cout<<"Enter the plaintext: ";
	cin>>plaintext;
	cout<<"Enter the 8 character key: ";
	cin>>key;
	while(plaintext.size()%8) 
		plaintext+=" ";
//	cout<<plaintext<<endl;
	bits_plaintext = getBitRepresentation(plaintext);
	bits_key = getBitRepresentation(key);
	cout<<endl<<"Plaintext in bits: "<<bits_plaintext<<endl;
	cout<<"Key in bits: "<<bits_key<<endl;
	string bits_ciphertext = des(bits_plaintext, bits_key, false);
	cout<<endl<<"Ciphertext in bits: "<<bits_ciphertext<<endl;
	string ciphertext = toText(bits_ciphertext);
	cout<<"Ciphertext in characters: "<<ciphertext<<endl;
	string bits_decipheredtext = des(bits_ciphertext, bits_key, true);
	string decipheredtext = toText(bits_decipheredtext);
	cout<<endl<<"Deciphered text: "<<decipheredtext<<endl;
}

