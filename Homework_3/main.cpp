// Задание 3
// Защищенный обмен данными

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

class Data
{
public:

	Data() = delete;
	Data(int, float);

	// methods

	std::mutex& get_mutex()
	{
		return mute;
	}

	int get_d1()
	{
		return data1;
	}

	float get_d2()
	{
		return data2;
	}

	void set_d1(int d1)
	{
		data1 = d1;
	}

	void set_d2(float d2)
	{
		data2 = d2;
	}

protected:

private:

	// mutex

	std::mutex mute;

	// data

	int data1;
	float data2;
}; // !Data

// Functions

// lock()
void swap1(Data& data1, Data& data2);

// scoped_lock()
void swap2(Data& data1, Data& data2);

// unique_lock()
void swap3(Data& data1, Data& data2);

int main(int argc, char** argv)
{
	// VARS

	Data d1{ 5, 3.2 };
	Data d2{ 7, 2.8 };

	std::vector<std::thread> runner1;	
	std::vector<std::thread> runner2;
	std::vector<std::thread> runner3;

	// Body

	// LOCK

	std::cout << "\nLOCK ATTEMPTS\n";

	for (int i = 0; i < 3; ++i)
	{
		runner1.push_back(std::thread(swap1, std::ref(d1), std::ref(d2)));
	}

	std::cout << '\n';

	for (auto& elem : runner1)
	{
		elem.join();
	}

	// SCOPED LOCK

	std::cout << "\nSCOPED LOCK ATTEMPTS\n";

	for (int i = 0; i < 3; ++i)
	{
		runner2.push_back(std::thread(swap1, std::ref(d1), std::ref(d2)));
	}

	std::cout << '\n';

	for (auto& elem : runner2)
	{
		elem.join();
	}

	// UNIQUE LOCK

	std::cout << "\nUNIQUE LOCK ATTEMPTS\n";

	for (int i = 0; i < 3; ++i)
	{
		runner3.push_back(std::thread(swap1, std::ref(d1), std::ref(d2)));
	}

	std::cout << '\n';

	for (auto& elem : runner3)
	{
		elem.join();
	}

	// EXIT
	std::cout << std::endl;
	std::cout << "Press ENTER to continue...";
	std::cin.get();
	return EXIT_SUCCESS;

} // !main()

Data::Data(int d1, float d2) : data1(d1), data2(d2) {};

/*
LOCK
*/
void swap1(Data& data1, Data& data2)
{
	data1.get_mutex().lock();
	data2.get_mutex().lock();

	std::lock_guard<std::mutex> ld1(data1.get_mutex(), std::adopt_lock);
	std::lock_guard<std::mutex> ld2(data2.get_mutex(), std::adopt_lock);

	std::cout << "\nDATA SWAP\n";
	std::cout << "data1, d1 = " << data1.get_d1() << ", d2 = " << data1.get_d2() << '\n';
	std::cout << "data2, d2 = " << data2.get_d1() << ", d2 = " << data2.get_d2() << '\n';

	int tmp_d1 = data1.get_d1();
	float tmp_d2 = data1.get_d2();

	data1.set_d1(data2.get_d1());
	data1.set_d2(data2.get_d2());

	data2.set_d1(tmp_d1);
	data2.set_d2(tmp_d2);

	std::cout << "\nAFTER SWAP\n";
	std::cout << "data1, d1 = " << data1.get_d1() << ", d2 = " << data1.get_d2() << '\n';
	std::cout << "data2, d2 = " << data2.get_d1() << ", d2 = " << data2.get_d2() << '\n';

	std::cout << "\n-------------\n";

} // !swap1()

/*
SCOPED LOCK
*/
void swap2(Data& data1, Data& data2)
{
	std::scoped_lock(data1.get_mutex(), data2.get_mutex());

	std::cout << "\nDATA SWAP\n";
	std::cout << "data1, d1 = " << data1.get_d1() << ", d2 = " << data1.get_d2() << '\n';
	std::cout << "data2, d2 = " << data2.get_d1() << ", d2 = " << data2.get_d2() << '\n';

	int tmp_d1 = data1.get_d1();
	float tmp_d2 = data1.get_d2();

	data1.set_d1(data2.get_d1());
	data1.set_d2(data2.get_d2());

	data2.set_d1(tmp_d1);
	data2.set_d2(tmp_d2);

	std::cout << "\nAFTER SWAP\n";
	std::cout << "data1, d1 = " << data1.get_d1() << ", d2 = " << data1.get_d2() << '\n';
	std::cout << "data2, d2 = " << data2.get_d1() << ", d2 = " << data2.get_d2() << '\n';

	std::cout << "\n-------------\n";	

} // !swap2()

/*
UNIQUE LOCK
*/
void swap3(Data& data1, Data& data2)
{
	std::unique_lock<std::mutex> ul1(data1.get_mutex());
	std::unique_lock<std::mutex> ul2(data1.get_mutex());

	ul1.lock();
	ul2.lock();

	std::cout << "\nDATA SWAP\n";
	std::cout << "data1, d1 = " << data1.get_d1() << ", d2 = " << data1.get_d2() << '\n';
	std::cout << "data2, d2 = " << data2.get_d1() << ", d2 = " << data2.get_d2() << '\n';

	int tmp_d1 = data1.get_d1();
	float tmp_d2 = data1.get_d2();

	data1.set_d1(data2.get_d1());
	data1.set_d2(data2.get_d2());

	data2.set_d1(tmp_d1);
	data2.set_d2(tmp_d2);

	std::cout << "\nAFTER SWAP\n";
	std::cout << "data1, d1 = " << data1.get_d1() << ", d2 = " << data1.get_d2() << '\n';
	std::cout << "data2, d2 = " << data2.get_d1() << ", d2 = " << data2.get_d2() << '\n';

	std::cout << "\n-------------\n";

	ul1.unlock();
	ul2.unlock();

} // !swap3()