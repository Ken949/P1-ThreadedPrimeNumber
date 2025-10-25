#include <iostream>
#include <fstream>
#include <thread>  
#include <string>
#include <chrono>
#include <ctime>
#include <vector>
#include <mutex>

std::mutex result_mutex;

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

void timestamp(std::string& time) { //need reference to add timestamp to output
    auto now = std::chrono::system_clock::now(); 
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);  // Convert to time_t

    // Allocate a buffer to hold the timestamp string
    char buf[100];

    ctime_s(buf, sizeof(buf), &now_time);

    // Remove Date
    std::string time_str(buf);

    //std::cout << "[Timestamp: " << time_str.substr(11, 8) << "] "; 
    time += "[Timestamp: " + time_str.substr(11, 8) + "] ";  // Store timestamp in referenced output

}

void thread_function(int thread_id, int min, int max, std::vector<std::string>& output_list) { //to pass into thread, reference output vector to edit in main
    for (int i = min;i < max+1;i++) {
        std::string output;
        timestamp(output); //puts timestamp into output string
        output += "Thread " + std::to_string(thread_id) + ", Number: " + std::to_string(i) + ", Result: " + (prime(i) ? "Prime" : "Not Prime");
		
        //mutex for accessing shared resource
        {
			std::lock_guard<std::mutex> lock(result_mutex); // Lock mutex for thread-safe output, will error if no mutex
            output_list.push_back(output); //store output in referenced output vector
        }
    }

    {
        std::lock_guard<std::mutex> lock(result_mutex); // Lock mutex for thread-safe output
        std::cout << "[Thread " << thread_id << " is done]" << std::endl;  // Notify when the thread finishes its task
    }
}
//Variation A2B1: Wait until all threads are done then print, Division of Search Range
int main() {
    //reading config files
    std::ifstream config("config.txt");

	if (!config.is_open()) {
		std::cerr << "Failed to open config file." << std::endl;
		return 1;
	}

    int Config_Threads = 0;
    int Max = 0;

    std::string line;

    while (std::getline(config, line)) {
        size_t pos = line.find('='); // find the '=' character
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);       // text before '='
            std::string value = line.substr(pos + 1);   // text after '='

            if (key == "numThreads") Config_Threads = std::stoi(value);
            else if (key == "maxNumber") Max = std::stoi(value);
        }
    }

    //Threading Start
	std::cout << "Starting " << Config_Threads << " threads to find prime numbers until " << Max << std::endl;

    //How many numbers per thread
	int number_per_thread = Max / Config_Threads;

    //Thread Storage
    std::vector<std::thread> threads;

    //Output Storage (Output after threads are done)
    std::vector<std::string> results;

  
    //Thread Creation
    for (int i = 0; i < Config_Threads; i++) {
        int start = i * number_per_thread; //start at 0
		int end = (i + 1) * number_per_thread - 1; // -1 to avoid overlap

		if (i == Config_Threads - 1) { // Last thread takes any remaining numbers
            end = Max;
        }

        //for checking
		//std::cout << "Start: " << start << " End: " << end << std::endl;

        threads.push_back(std::thread(thread_function, i + 1, start, end, std::ref(results)));

        
    }

    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join(); 
    }

    std::cout << "All threads finished. Here are the results:\n";

    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Slept for " << 2 << " seconds." << std::endl;

    for (const auto& line : results) {
        std::cout << line << std::endl;  // Print each line of the output
    }

    return 0;

}
