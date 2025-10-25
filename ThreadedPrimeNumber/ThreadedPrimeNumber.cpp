#include <iostream>
#include <fstream>
#include <thread>  
#include <string>


bool prime(int n) {
    //prints for testing
    if (n <= 1) { /*std::cout << "Not Prime" << std::endl;*/ return false; } //1 isnt counted as a prime number
    if (n == 2) { /*std::cout << "Prime" << std::endl;*/ return true; } //2 is a prime number
	if (n % 2 == 0) { /*std::cout << "Not Prime" << std::endl;*/ return false; } //even numbers arent prime

	for (int i = 3; i <= sqrt(n); i++) { //checks 3 until max, sqrt of n is all thats needed when checking prime
        //std::cout << "Inside Thread" << std::endl;
        if (n % i == 0) { /*std::cout << "Not Prime" << std::endl;*/ return false; }
	}
    /*std::cout << "Prime" << std::endl;*/
    return true;
}

void thread_function(int thread_id, int min, int max) { //to pass into thread
    std::cout << "Thread Entered" << std::endl;
    for (int i = min;i < max+1;i++) {
       std::cout << i << " from thread " << thread_id << " is " << (prime(i) ? "Prime" : "Not Prime") << std::endl;
    }
}


int main() {
    //reading config files
    std::ifstream config("config.txt");

	if (!config.is_open()) {
		std::cerr << "Failed to open config file." << std::endl;
		return 1;
	}

    int Threads = 0;
    int Max = 0;

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


    //Threading Start
	std::cout << "Starting " << Threads << " threads to find prime numbers until " << Max << std::endl;

    //How many numbers per thread
	int number_per_thread = Max / Threads;
  
    //Thread Creation
    for (int i = 0; i < Threads; i++) {
        int start = i * number_per_thread; //start at 0
		int end = (i + 1) * number_per_thread - 1; // -1 to avoid overlap

		if (i == Threads - 1) { // Last thread takes any remaining numbers
            end = Max;
        }

		std::cout << "Start: " << start << " End: " << end << std::endl;
        thread_function((i + 1), start, end);




    }

    return 0;

}
