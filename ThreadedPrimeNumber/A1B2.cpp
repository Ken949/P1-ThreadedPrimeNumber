#include <iostream>
#include <fstream>
#include <thread>  
#include <string>
#include <chrono>
#include <ctime>
#include <vector>
#include <mutex>

std::mutex cout_mutex;

bool prime(int n) {
    //prints for testing
    if (n <= 1) { /*std::cout << "Not Prime" << std::endl;*/ return false; } //1 isnt counted as a prime number
    if (n == 2) { /*std::cout << "Prime" << std::endl;*/ return true; } //2 is a prime number
	if (n % 2 == 0) { /*std::cout << "Not Prime" << std::endl;*/ return false; } //even numbers arent prime

	for (int i = 3; i <= sqrt(n); i++) { //checks 3 until max, sqrt(n) is highest need to go
        //std::cout << "Inside Thread" << std::endl;
        if (n % i == 0) { /*std::cout << "Not Prime" << std::endl;*/ return false; }
	}
    /*std::cout << "Prime" << std::endl;*/
    return true;
}

void timestamp() {
    auto now = std::chrono::system_clock::now(); 
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);  // Convert to time_t

    // Allocate a buffer to hold the timestamp string
    char buf[100];

    ctime_s(buf, sizeof(buf), &now_time);

    // Remove Date
    std::string time_str(buf);

    std::cout << "[Timestamp: " << time_str.substr(11, 8) << "] ";
}

void thread_function(int number) { //to pass into thread
    bool result = prime(number); 
    {
        std::lock_guard<std::mutex> lock(cout_mutex);  // Lock to print results safely
        timestamp();  // Print timestamp
        std::cout << "Number: " << number << ", Result: " << (result ? "Prime" : "Not Prime") << std::endl;
    }
}


int main() {
    // Reading config file
    std::ifstream config("config.txt");

    if (!config.is_open()) {
        std::cerr << "Failed to open config file." << std::endl;
        return 1;
    }

    int Max = 0;
    bool maxNumberFound = false;

    std::string line;

    // Parsing config file
    while (std::getline(config, line)) {
        size_t pos = line.find('='); // find the '=' character
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);       // text before '='
            std::string value = line.substr(pos + 1);    // text after '='

            if (key == "maxNumber") {
                try {
                    Max = std::stoi(value);  // Attempt to convert to integer
                    maxNumberFound = true;
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Error: Invalid value for 'maxNumber'. It must be an integer." << std::endl;
                    return 1;
                } catch (const std::out_of_range& e) {
                    std::cerr << "Error: 'maxNumber' is out of range for an integer." << std::endl;
                    return 1;
                }
            }
        }
    }

    // Check if maxNumber was found and is valid
    if (!maxNumberFound) {
        std::cerr << "Error: 'maxNumber' key not found in the config file." << std::endl;
        return 1;
    }

    if (Max <= 0) {
        std::cerr << "Error: 'maxNumber' must be a positive integer." << std::endl;
        return 1;
    }

    //Threading Start
	std::cout << "Starting linear search to find prime numbers until " << Max << " while using threads for each number" << std::endl;

    //Thread Storage
    std::vector<std::thread> threads;
  
    //Thread Creation
    for (int i = 0; i <= Max; i++) {
        threads.push_back(std::thread(thread_function, i));
    }

    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join(); 
    }

    return 0;

}
