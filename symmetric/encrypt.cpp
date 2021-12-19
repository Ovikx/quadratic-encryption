#include <iostream>
#include <math.h>
#include <vector>
#include <tuple>
#include <random>
#include <time.h>
#include <fstream>
#include <string>

using namespace std;

class QuadraticEncryption{

    public:
        string key;
        string delim = "<<>>";
        int key_length;

        QuadraticEncryption(int key_length){
            this->key_length = key_length;
            this->key = generate_key();
        }

       tuple<float, int, int> get_coefficients(string k){
            float a = 0;
            int b = 0;
            int c = 0;
            int first_delim_pos = k.find(delim);
            int second_delim_pos = k.rfind(delim);

            for (char ch : k.substr(0, first_delim_pos)){
                a += int(ch);
            }
            a = 1/a;
            for (char ch : k.substr(first_delim_pos+delim.length(), k.length()-(k.length()-second_delim_pos)-(first_delim_pos+delim.length()))){ // 012<<>>345<<>>6789  18-(18-10)-(3+4)
                b += int(ch);
            }

            if (second_delim_pos != k.length()-delim.length()){
                for (char ch : k.substr(second_delim_pos+delim.length())){
                    c += int(ch);
                }
            }
            return make_tuple(a, b, c);
        }

        string generate_key(){
            random_device rd;
            mt19937 gen(rd());
            gen.seed(time(0));
            uniform_int_distribution<> distr(32, 126);
            uniform_int_distribution<> rand_delim1(1, key_length - 1);

            int delim1_pos = rand_delim1(gen);
            uniform_int_distribution<> rand_delim2(delim1_pos+delim.length()+1, key_length+delim.length()); // 012345678<<>>9

            int delim2_pos = rand_delim2(gen);
            string random_key;

            for (int i = 0; i < key_length; i++){
                random_key += char(distr(gen));
            }

            random_key.insert(delim1_pos, delim);
            random_key.insert(delim2_pos, delim);

            return random_key;
        }

        int calculate_Y(float a, int b, int c, int x){
            return int(a*(x*x) + b*x + c);
        }

        double calculate_X(double a, int b, int c, int y){
            return (sqrt(4*a*(y-c)+b*b)-b)/(2*a);
        }

        string encrypt(string message, string k, bool file_output = false){
            string encrypted;
            string char_delimiter(3, '|');
            float a;
            int b;
            int c;

            tie(a, b, c) = get_coefficients(k);
            for (char ch : message){
                encrypted += to_string(calculate_Y(a, b, c, int(ch))) + char_delimiter;
            }

            if (file_output){
                ofstream outfile("encrypted.txt");
                outfile << encrypted << endl;
                cout << "Encrypted text saved in current directory." << endl;
                outfile.close();
            }

            return encrypted;
        }

        string decrypt(string message, string k){
            double a;
            int b;
            int c;
            tie(a, b, c) = get_coefficients(k);
            string decrypted;
            string current_num;

            for (char ch : message){
                if (int(ch) == 124){
                    if (current_num != ""){
                        decrypted += char(round(calculate_X(a, b, c, stoi(current_num))));
                        current_num = "";
                    }
                    continue;
                }
                current_num += ch;
            }
            return decrypted;
        }

        void save_key(){
            ofstream outfile("key.txt");
            outfile << key << endl;
            cout << "Key saved in current directory." << endl;
        }

        
};

int main(){
    QuadraticEncryption q(64);

    q.encrypt("Hello to anyone reading this!", q.key, true);
    q.save_key();

    string message; 
    string key;

    ifstream message_file("encrypted.txt");
    ifstream key_file ("key.txt");
    

    if (message_file.is_open()){
        getline(message_file, message);
        message_file.close();
    }

    if (key_file.is_open()){
        getline(key_file, key);
        key_file.close();
    }

    cout << "Decrypted message: " << q.decrypt(message, key);
    return 0;
}