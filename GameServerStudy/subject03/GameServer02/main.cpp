#include <iostream>
#include <thread>
#include "Server.h"
#include "Timer.h"

int main()
{
	Server server;
	server.Init();
	Timer timer(server.getRoomMangerInServer(), 7, 3);

	std::thread runThread1([&]() {server.Run(); });
	std::thread runThread2([&]() {timer.RunTimer(); });
	std::cout << "press any key to exit";
	getchar();

	timer.setTimerFinish();
	runThread2.join();
	server.Stop();
	runThread1.join();

	return 0;
}