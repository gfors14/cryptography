#include <iostream>
#include "gmp.h"
#include "gmpxx.h"
#include <unistd.h>
#include <ctime>
#include <fstream>
#include <sstream>


using namespace std;

char * modular_inverse(mpz_t e, mpz_t n) 
{
    mpz_t res;
    mpz_init(res);
    if (mpz_invert(res, e, n) == 0) 
    {
        return NULL;
    }
    char *str = mpz_get_str(NULL, 10, res);
    mpz_clear(res);
    return str;
}

void generate_prime(mpz_t &random_num, int length, gmp_randstate_t rand_state)
{
    mpz_t bottom;
    mpz_t top;
    mpz_init(bottom);
    mpz_init(top);
    mpz_ui_pow_ui(bottom, 10, length-1);
    mpz_ui_pow_ui(top, 10, length);
    mpz_sub_ui(top, top, 1);
    mpz_urandomm(random_num, rand_state, top);
	mpz_nextprime(top, top);
    mpz_add(random_num, random_num, bottom);
	mpz_nextprime(bottom, bottom);
    mpz_clear(bottom);
    mpz_clear(top);
}
bool is_prime(const mpz_t num)
{
    int result = mpz_probab_prime_p(num, 25);
    return result;
}

int generate_e()
{
    int vals[] = {3,5,17,257,65537};
    srand(time(0));
    int index = rand()%5;
    return vals[index];
}	

void encrypt(mpz_t result, const mpz_t message, const mpz_t e, const mpz_t n) 
{
    mpz_powm(result, message, e, n); 
}

void decrypt(mpz_t result, const mpz_t cipher, const mpz_t d, const mpz_t n) 
{
    mpz_powm(result, cipher, d, n); 
}

void encrypt_file(const string& input_file, const string& output_file, const mpz_t e, const mpz_t n) 
{
    ifstream in_file(input_file);
    ofstream out_file(output_file);
    char ch;
    while (in_file.get(ch)) 
    {  
        mpz_t message, cipher;
        mpz_init_set_ui(message, static_cast<unsigned long>(ch)); 
        mpz_init(cipher);

        encrypt(cipher, message, e, n); 

        
        char *cipherText = mpz_get_str(NULL, 36, cipher);
        out_file << cipherText << " ";

        mpz_clear(message);
        mpz_clear(cipher);
        free(cipherText);
    }

    in_file.close();
    out_file.close();
    cout << "File encrypted to " << output_file << "\n";
}

void decrypt_file(const string& input_file, const string& output_file, const mpz_t d, const mpz_t n) 
{
    ifstream in_file(input_file);
    ofstream out_file(output_file);

    string cipherText;
    while (in_file >> cipherText) 
    {  
        mpz_t cipher, decrypted;
        mpz_init(cipher);
        mpz_init(decrypted);

        mpz_set_str(cipher, cipherText.c_str(), 36); 

        decrypt(decrypted, cipher, d, n); 

        
        out_file << static_cast<char>(mpz_get_ui(decrypted));

        mpz_clear(cipher);
        mpz_clear(decrypted);
    }

    in_file.close();
    out_file.close();
    cout << "File decrypted to " << output_file << "\n";
}


int main()
{
	int choice;
	while (1>0)
	{
		cout << "Welcome to my RSA encryption program! Please enter What would you like to do."  << endl;
		cout << "1. Create a Public Key and a Private Key " << endl;
		
		cout << "2. Encrypt a file" << endl;
		cout << "3. Decrypt a file" << endl;
		cout << "4. Exit " << endl;

		cin >> choice;
        
		if (choice == 1) // Create public and private key
        {
            int length;
            cout << "Enter the length: ";
            cin >> length;
            
            mpz_t p, q, n, r, e;
            mpz_inits(p, q, n, r, e, NULL);
            gmp_randstate_t rand_state;
            gmp_randinit_mt(rand_state);
            gmp_randseed_ui(rand_state, time(NULL));
            
            
            while (!is_prime(p)) { generate_prime(p, length, rand_state); usleep(6969); } //ensures p and Q are properly generated to be prime, and usleep so they are different
            while (!is_prime(q)) { generate_prime(q, length, rand_state); } 
            
            mpz_mul(n, p, q);
            mpz_t temp1, temp2;
            mpz_inits(temp1, temp2, NULL);
            mpz_sub_ui(temp1, p, 1);
            mpz_sub_ui(temp2, q, 1);
            mpz_mul(r, temp1, temp2);
            
            int real_e = generate_e();
            mpz_set_si(e, real_e);
            char *d = modular_inverse(e, r);

            while (d == NULL) 
            {
                real_e = generate_e();
                mpz_set_si(e, real_e);
                d = modular_inverse(e, r);
            }

            cout << "Generated RSA Key: " << endl;
            cout << "p = "; mpz_out_str(stdout, 10, p); cout << endl;
            cout << "q = "; mpz_out_str(stdout, 10, q); cout << endl; //outputs all pqnred
            cout << "n = "; mpz_out_str(stdout, 10, n); cout << endl;
            cout << "r = "; mpz_out_str(stdout, 10, r); cout << endl;
            cout << "e = " << real_e << endl;
            cout << "d = " << d << endl;
			
			string temp;
			cout << "Enter a private key file: " << endl;
			cin >> temp;
			
			ofstream privatekey_file(temp);
			
			cout << "enter a public key file: " << endl;
			cin >> temp; 
			ofstream publickey_file(temp);
			
            if (privatekey_file.is_open())
            {
                privatekey_file << mpz_get_str(NULL, 10, n) << "\n" << d << "\n";
                privatekey_file.close();
            }
			else
			{
				cout << "Private key file not open! " << endl;
				break;
			}
            if (publickey_file.is_open())
            {
                publickey_file << mpz_get_str(NULL, 10, n) << "\n" << real_e << "\n";
                publickey_file.close();
            }
			else
			{
				cout << "public Key file not open! " << endl;
				break;
			}

            mpz_clears(p, q, n, r, temp1, temp2, e, NULL);
            delete d;
        }
        else if (choice == 2) // Encrypt
        {
            string input_file, output_file;
            cout << "Enter the input file to encrypt: ";
            cin >> input_file;
            cout << "Enter the output file for encrypted data: ";
		    cin >> output_file;
			
			string temp;
			cout << "enter a public key file: " << endl;
			cin >> temp; 
			ifstream publickey_file(temp);
			
           
            if (!publickey_file.is_open())
            {
                cout << "public key file not opened!" << endl;
                break;
            }

            mpz_t e, n;
            mpz_inits(e, n, NULL);
            string e_str, n_str;
            getline(publickey_file, n_str);
            getline(publickey_file, e_str);
            mpz_set_str(n, n_str.c_str(), 10);
            mpz_set_str(e, e_str.c_str(), 10);
            publickey_file.close();

            encrypt_file(input_file, output_file, e, n);

            mpz_clears(e, n, NULL);
        }
        else if (choice == 3) // Decrypt 
        {
            string input_file, output_file;
            cout << "Enter the input file to decrypt: ";
            cin >> input_file;
            cout << "Enter the output file for decrypted data: ";
            cin >> output_file;

            string temp;
			cout << "Enter a private key file: " << endl;
			cin >> temp;
			
			ifstream privatekey_file(temp);
			
		
            if (!privatekey_file.is_open())
            {
                cout << "private key file not opened! " << endl;
                break;
            }

            mpz_t d, n;
            mpz_inits(d, n, NULL);
            string d_str, n_str;
            getline(privatekey_file, n_str);
            getline(privatekey_file, d_str);
            mpz_set_str(n, n_str.c_str(), 10);
            mpz_set_str(d, d_str.c_str(), 10);
            privatekey_file.close();

            decrypt_file(input_file, output_file, d, n);

            mpz_clears(d, n, NULL);
        }
        else if (choice == 4) 
        {
            return 0;
        }
        else
        {
            cout << "Invalid option. Please select again.\n";
        }
    }
        return 0;
}