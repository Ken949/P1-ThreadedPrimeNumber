#include <iostream>
#include <fstream>
#include <thread>  
#include <string>


bool prime(int n) {
	if (n <= 1) return false; //1 isnt counted as a prime number
    if (n == 2) return true; //2 is a prime number

	for (int i = 3; i <= n; ++i) { //checks 3 until max
		if (n % i == 0) return false;
	}
    return true;
}


int main() {
    std::ifstream config("config.txt");

	if (!config.is_open()) {
		std::cerr << "Failed to open config file." << std::endl;
		return 1;
	}

    int Threads = 0;
    int Max = 0;

    //reading config
    std::string line;

    while (std::getline(config, line)) {
        size_t pos = line.find('='); // find the '=' character
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);       // text before '='
            std::string value = line.substr(pos + 1);   // text after '='

            if (key == "numThreads") Threads = std::stoi(value);
            else if (key == "maxNumber") Max = std::stoi(value);
        }
    }

	std::cout << "Starting " << Threads << " threads to find prime numbers up to " << Max << std::endl;




    // Waiting for thread to finish
    

    return 0;
}
