// Задание 2
// Прогресс бар

#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <vector>
#include <mutex>
#include <condition_variable>

// glob vars

int calcTime = 1000;	// calculation time in ms, default 1000ms
int threadNum = 2;		// used threads, default 2
std::atomic<bool> exitFlag = false;
std::mutex mute;
std::mutex barMute;
int barFillRatio = 0;
std::atomic<int> completed_threads = 0;
std::condition_variable cond;

// defines

#define BAR_WIDTH 50

// Functions

void dummy_func(int, int thread_index);

void clear_screen();

void manage_screen();

void start_calc(int, int);

void progress_bar(int, int);

// ---

using namespace std::chrono_literals;

int main(int argc, char** argv)
{
	clear_screen();

	std::cout << "Multithread imitation.\n\n";
	std::cout << "Set delay (in milliseconds): ";
	std::cin >> calcTime;
	std::cout << "Set number of threads (int): ";
	std::cin >> threadNum;	

	manage_screen();

	start_calc(calcTime, threadNum);		

	// EXIT
	std::cout << std::endl;
	std::cout << "Press ENTER to continue...";
	std::cin.get();
	return EXIT_SUCCESS;
} // !main()

void dummy_func(int timer, int thread_index)
{
	std::lock_guard<std::mutex> lock(mute);
	barFillRatio = BAR_WIDTH / threadNum;

	auto start = std::chrono::steady_clock::now();	
	auto delay = std::chrono::milliseconds(timer);

	std::cout << "--------------\n";

	std::cout << "Thread number #" << thread_index << '\n';
	std::cout << "Thread ID: " << std::this_thread::get_id << '\n';

	std::this_thread::sleep_for(delay);

	auto end = std::chrono::steady_clock::now();

	auto delta = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	
	std::cout << "Thread execution time: " << delta.count();

	int tmp = completed_threads.load();
	tmp++;
	completed_threads.store(tmp);

	std::cout << "\n--------------\n";	

	progress_bar(barFillRatio, threadNum);

} // !dummy_func()

void clear_screen()
{
#ifdef _WIN32
	std::system("cls");
#else
	// Assume POSIX
	std::system("clear");
#endif

} // !clear_screen()

void manage_screen()
{
	clear_screen();
	std::cout << "Multithread imitation.\n\n";
	std::cout << "Delay:\t\t" << calcTime << '\n';
	std::cout << "Threads:\t" << threadNum << '\n';
	
} // !manage_screen()

void start_calc(int delay, int threads)
{
	std::vector<std::thread> runner;
	
	for (int i = 0; i < threads; ++i)
	{
		runner.push_back(std::thread(dummy_func, delay, i+1));
	}	

	// joins

	for (int i = 0; i < threads; ++i)
	{
		runner[i].join();
	}	

} // !start_calc()

void progress_bar(int ratio, int threads)
{
	// vars

	char fillIncomplete = ' ';
	char fillComplete = '#';	
	int barWidth = BAR_WIDTH;	
	
	std::cout << "EXECUTION PROGRESS: ";
	std::cout << '[';
		
	int pos = completed_threads.load() * ratio;

	for (int i = 0; i < barWidth; ++i)
	{
		if (i < pos)
		{
			std::cout << fillComplete;
		}
		else if (i == pos)
		{
			std::cout << ">";
		}
		else
		{		
			std::cout << fillIncomplete;
		}
	}
		
	if (threadNum != completed_threads.load())
		std::cout << "] " << int( (ratio * completed_threads.load()) * 2 ) << " %\r";
	else
		std::cout << "] 100 %\r";

	std::cout.flush();

	std::cout << '\n';		
		
} // !rogress_bar()