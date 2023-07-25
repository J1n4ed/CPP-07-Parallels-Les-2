// Задание 2
// Прогресс бар

#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <random>
#include <Windows.h>

//

struct execution_unit
{
public:

	execution_unit()
	{
		finished = false;
		percentDone = 0.0;
	}
	
	// methods

	/*
	check if unit's thread is finished
	*/
	bool check()
	{
		return finished;
	}
	
	/*
	set completed
	*/
	void set_finished()
	{
		finished = true;
	}

	/*
	set unit's thread id
	*/
	void set_id(std::thread::id id)
	{
		unitId = id;
	}

	/*
	check unit's thread id
	*/
	bool check_id(std::thread::id id)
	{
		if (unitId == id)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/*
	get unit id
	*/
	std::thread::id get_id()
	{
		return unitId;
	}

	/*
	set unit index
	*/
	void set_num(int num)
	{
		unitNum = num;
	}

	/*
	get unit index
	*/
	int get_num()
	{
		return unitNum;
	}

	/*
	get progress
	*/
	float get_progress()
	{
		return percentDone;
	}

	/*
	set progress
	*/
	void set_progress(float percent)
	{
		percentDone = percent;
	}

	/*
	Set ongoing exec delta time
	*/
	void set_time(double Time)
	{
		execTime = Time;
	}

	/*
	get stored time delta
	*/
	double get_time()
	{
		return execTime;
	}

private:
	bool finished;
	float percentDone;
	std::thread::id unitId;
	double execTime;
	int unitNum;
};

// glob vars

int calcTime = 1000;	// calculation time in ms, default 1000ms
int threadNum = 2;		// used threads, default 2
std::atomic<bool> exitFlag = false;
std::mutex mute;
std::mutex barMute;
int barFillRatio = 0;
std::condition_variable cond;
std::vector<execution_unit> execution_pool;

// defines

#define BAR_WIDTH 50

// Functions

void dummy_func(int, int thread_index);

void clear_screen();

void progress_bar(float);

// ---

using namespace std::chrono_literals;

int main(int argc, char** argv)
{
	// VAR

	bool exitFlag = false;

	// BODY

	clear_screen();

	std::cout << "Multithread imitation.\n\n";
	std::cout << "Set average delay (in milliseconds): ";
	std::cin >> calcTime;
	std::cout << "Set number of threads (int): ";
	std::cin >> threadNum;		

	std::vector<std::thread> runner;

	for (int i = 0; i < threadNum; ++i)
	{
		int t = calcTime;
		
		t += (i * 100);

		runner.push_back(std::thread(dummy_func,t , i + 1));
	}

	while (!exitFlag)
	{
		for (auto & unit : execution_pool) // check if all threads are done
		{
			exitFlag = unit.check();
		}

		if (!exitFlag) // skip to exit if exitflag is set
		{
			clear_screen();
			std::cout << "Multithread imitation.\n\n";
			std::cout << "Average delay:\t\t" << calcTime << '\n';
			std::cout << "Threads:\t" << threadNum << '\n';

			std::cout << "\n |= ==================================================================================== = | \n\n";

			for (auto& unit : execution_pool)
			{
				
				std::cout << "--- EXEC UNIT --------------------------------------------\n";
				std::cout << "Thread number #" << unit.get_num() << '\n';
				std::cout << "Thread ID: " << unit.get_id() << '\n';
				std::cout << "Thread execution time: " << unit.get_time() << '\n';
				progress_bar(unit.get_progress());
				std::cout << "\nSTATUS: ";

				if (unit.check())
				{
					std::cout << "FINISHED!";
				}
				else
				{
					std::cout << "IN PROGRESS...";
				}
				std::cout << '\n';
				std::cout << "----------------------------------------------------------\n";
				
			}			
			// std::cout.flush();
		}

	} // main loop

	std::cout << "\nAll threads completed!\n";	

	// joins	
	for (int i = 0; i < threadNum; ++i)
	{
		runner[i].join();
	}

	// EXIT
	std::cout << std::endl;
	std::cout << "Press ENTER to continue...";
	std::cin.get();
	return EXIT_SUCCESS;
} // !main()

void dummy_func(int timer, int thread_index)
{
	// PREPS
	typedef std::chrono::duration<double, std::milli> duration;
	// std::lock_guard<std::mutex> lock(mute);
	barFillRatio = BAR_WIDTH / threadNum;
	std::thread::id id = std::this_thread::get_id();
	execution_unit thee_unit;
	thee_unit.set_id(id);
	thee_unit.set_num(thread_index);
	execution_pool.push_back(thee_unit);
	float progress = 0.0;

	// EXEC		

	auto start = std::chrono::steady_clock::now();

	/*
		10 dummy 'tasks' with randomized execution time in each thread
	*/
	for (int i = 0; i < 10; ++i)
	{
		
		auto delay = std::chrono::milliseconds(timer);
		std::this_thread::sleep_for(delay);
		auto end = std::chrono::steady_clock::now();
		duration delta = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		progress += 0.10;
		for (auto& unit : execution_pool)
		{
			if (id == unit.get_id())
			{
				unit.set_time(delta.count());
				unit.set_progress(progress);
			}
		}
	} // end dummy

	for (auto& unit : execution_pool)
	{
		if (id == unit.get_id())
		{
			unit.set_finished();
		}
	}

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

void progress_bar(float progress)
{
	// vars

	char fillIncomplete = ' ';
	char fillComplete = '#';	
	int barWidth = BAR_WIDTH;	
	
	std::cout << "EXECUTION PROGRESS: ";
	std::cout << '[';
		
	int pos = (progress * 100) / 2;

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
		
	std::cout << "] " << pos * 2 << " %\r";	
		
} // !rogress_bar()