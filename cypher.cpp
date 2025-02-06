#include <iostream>
#include <cstring>
#include <string.h>
#include <algorithm>
#include <vector>

using namespace std;



string substitution(string text, string key, bool crypt)
{
	string alphabet = "abcdefghijklmnopqrstuvwxyz";
	string code;
	
	int length = key.length();
	
	for(int i =0;  i< length; i++)
		code += key[i];
	
	for(int i = 0; i< key.length(); i ++)
	{
		for(int k=0; k<25; k++)
		{
			if(alphabet[k] == key[i])
				alphabet.erase(remove(alphabet.begin(), alphabet.end(), alphabet[k]), alphabet.end());
		}
	} 
	cout << alphabet << endl;
	
	
	int m = 0;
	for(int i = length; i<26; i++)
	{
		code += alphabet[m];
		m++;
	}
		
		cout << code << endl;
		
	alphabet = "abcdefghijklmnopqrstuvwxyz";
	
	for(int i = 0; i< text.length(); i ++)
	{
		for(int k=0; k<25; k++)
		{
			if(text[i] == alphabet[k])
				text[i] = code[k];
		}
	} 
	cout << text << endl;
	
}


string affine(string text, int a, int b, bool crypt)
{
	string alphabet = "abcdefghijklmnopqrstuvwxyz";
	
	cout << alphabet[25] << endl;
	
	int z;
	
	for(int i = 0; i <text.length(); i++)
	{
		int x;
		for(int k = 0; k < 25; k++)
		{
			if(alphabet[k] == text[i])
				x = k;
		}
		
		z = (a*x +b) % 26;
		
		text[i] = alphabet[z];
		
		
	}
	
	cout << text << endl;
	
}

string vigenere(string text, string keyword)
{
	string encode;
	int k = 0;
	//cout << text.length() << endl;
	for(int i = 0; i < text.length(); i++)
	{
		if(isspace(text[i]))
		{
			encode[i] = ' ';
		}
		else
		{
			encode[i] = keyword[k];
			//cout << encode[i] << endl;
			k++;
			if(k > keyword.length())
			{
				k = 0;
			}
		}
		
	}
	for(int i = 0; i < 11; i++)
		cout << encode[i];
	
}


char shift(char x, int k, bool crypt)
{
	//uppercase
	if(x >= 'A' && x <= 'Z')
	{
		x=x+k;
		if(x>'Z')
			x-=26;
		
		return x;
		
	}
	
	//lowercase
	if(x >= 'a' && x <= 'z')
	{
		x=x+k;
		if(x>'z')
			x-=26;
		
		return x;
		
	}
		
	return x;
}


int main()
{
	
	bool crypt;
	/*
	
	
	int k;
	cout << "Enter the key (0-25): " << endl;
	cin >> k;
	char message[1000];
	cout << "Enter the message" << endl;
	cin.getline(message, 1000);
	cin.getline(message, 1000);
	
	for(int i=0; i<strlen(message); i++)
		message[i] = shift(message[i], k);
	cout << message << endl;
	
	return 0; */
	string text;
	string key;
	
	cout << "enter the text " << endl;
	getline(cin, text);
	cout << "enter key" << endl;
	cin >> key;
	cout << text << endl;
	cout << key << endl;
	vigenere(text, key);
}


/*

string alphabet = "abcdefghijklmnopqrstuvwxyz";
	char keyalpha[25];
	int length = key.length();
	int m = 0;
	//cout << length << endl;
	
	for(int i = 0; i< key.length(); i ++)
	{
		for(int k=0; k<25; k++)
		{
			if(alphabet[k] == key[i])
				alphabet.erase(remove(alphabet.begin(), alphabet.end(), alphabet[k]), alphabet.end());
		}
	} 
	
	for(int i = 0; i < length; i++)
		keyalpha[i] = key[i];
	
	for(int i = length; i < 26; i++)
	{
		keyalpha[i] = alphabet[m];
		m++;
	}	
	
	for( int i=0; i <26; i++)
		cout << keyalpha[i] << endl;



	for(int i=0; i<text.length(); i++)
	{
		char temp = text[i];
		for(int k=0; k<26; k++)
		{
			if(alphabet[k] == temp)
			{
				text[i] = keyalpha[k];
				
			}
			
			
		}
		
	}
	cout << text << endl;
	return text;
	
	*/