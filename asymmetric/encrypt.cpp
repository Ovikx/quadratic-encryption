#include <iostream>
#include <vector>
#include <random>
#include <time.h>
#include <math.h>

using namespace std;

class QuadraticEncryption{

    public:
        string public_key;
        string private_key;
        int key_length;
        int filter_size;

        QuadraticEncryption(int key_length){
            this->filter_size = generate_filter_size();
            this->key_length = key_length;
            this->public_key = generate_public_key();
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

        float calculate_Y(float a, int b, int c, int x){
            return a*(pow(x, 2)) + b*x + c;
        }

        string encrypt(string message, string key){
            key = key.substr(1);
            string encrypted;
            string char_delimiter(filter_size, '|');
            string start_variation_delimiter(filter_size, '<');
            string end_variation_delimiter(filter_size, '>');
            
            for (int i = 0; i < (public_key.length() - (2*filter_size+1)); i++){
                string a_string = public_key.substr(0,i+1);
                float a = 0;

                for (char c : a_string){
                    a += int(c);
                }

                a = 1/a;

                for (int j = 0; j < (public_key.length() - (i+1 + 2*filter_size)); j++){
                    string b_string = public_key.substr(i+filter_size+1, j+1);
                    string c_string = public_key.substr(i+2*filter_size+j+2);
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
            return encrypted;
        }
};

int main(){
    QuadraticEncryption q(50);
    //q.encrypt("hi world, how are you today? I'm looking forward to meeting everyone on this fine night!", q.public_key);
    cout << q.public_key << "\n" << q.encrypt("hi!", q.public_key);
    return 0;
}