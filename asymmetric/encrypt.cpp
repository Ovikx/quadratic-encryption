#include <iostream>
#include <vector>
#include <random>
#include <time.h>
#include <fstream>

using namespace std;

class QuadraticEncryption{

    public:
        string public_key;
        string private_key;
        bool file_output;
        int key_length;
        int filter_size;

        QuadraticEncryption(int key_length, bool file_output = false){
            this->filter_size = generate_filter_size();
            this->key_length = key_length;
            this->public_key = generate_public_key();
            this->file_output = file_output;
        }

        int generate_filter_size(){
            random_device rd;
            mt19937 gen(rd());
            gen.seed(time(0));
            uniform_int_distribution<> distr(4,7);
            
            return distr(gen);
        }

        string generate_public_key(){
            random_device rd;
            mt19937 gen(rd());
            gen.seed(time(0));
            uniform_int_distribution<> distr(32, 126);

            string random_key = to_string(filter_size);
            for (int i = 0; i < key_length; i++){
                random_key += char(distr(gen));
            }

            return random_key;
        }

        int calculate_Y(float a, int b, int c, int x){
            return int(a*(x*x) + b*x + c);
        }

        string encrypt(string message, string key){
            string encrypted;
            string a_string;
            string b_string;
            string c_string;
            string char_delimiter(filter_size, '|');
            string start_variation_delimiter(filter_size, '<');
            string end_variation_delimiter(filter_size, '>');
            key = key.substr(1);
            
            for (int i = 0; i < (public_key.length() - (2*filter_size+1)); i++){
                a_string = public_key.substr(0,i+1);
                float a = 0;

                for (char c : a_string){
                    a += int(c);
                }

                a = 1/a;

                for (int j = 0; j < (public_key.length() - (i+1 + 2*filter_size)); j++){
                    b_string = public_key.substr(i+filter_size+1, j+1);
                    c_string = public_key.substr(i+2*filter_size+j+2);
                    int b = 0;
                    int c = 0;

                    for (char ch : b_string){
                        b += int(ch);
                    }

                    for (char ch : c_string){
                        c += int(ch);
                    }
                    
                    encrypted += start_variation_delimiter;

                    for (char ch : message){
                        encrypted += to_string(calculate_Y(a, b, c, int(ch))) + char_delimiter;
                    }

                    encrypted += end_variation_delimiter;
                }

            }

            if (file_output){
                ofstream outfile("encrypted.txt");
                outfile << encrypted << endl;
                outfile.close();
            }
            
            return encrypted;
        }
};

int main(){
    QuadraticEncryption q(50);
    //q.encrypt("hi world, how are you today? I'm looking forward to meeting everyone on this fine night!", q.public_key);
    cout << q.public_key << "\n" << q.encrypt("hi how are you doing on this fine thursday night?", q.public_key);
    return 0;
}