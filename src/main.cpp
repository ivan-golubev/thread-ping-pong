#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <string>
#include <condition_variable>
#include <stop_token>
#include <format>
#include <syncstream>
#include <chrono>

using namespace std::literals::chrono_literals;

struct Message {
	int counter;
	std::string message;
};

void runPingPong(std::mutex& mtx, std::condition_variable& cv, std::queue<Message>& msg_queue, std::stop_source& stop_src) {
	std::stop_token stop_tkn = stop_src.get_token();
	std::thread::id this_id = std::this_thread::get_id();
	std::unique_lock lck(mtx, std::defer_lock);

	while (!stop_tkn.stop_requested()) {
		lck.lock();
		cv.wait(lck, [&msg_queue]{ return !msg_queue.empty(); });
		Message incoming_msg = msg_queue.front();
		msg_queue.pop();
		std::osyncstream(std::cout) << std::format("T[{}] received [c={}, t={}]", this_id, incoming_msg.counter, incoming_msg.message) << "\n";
		int counter = incoming_msg.counter + 1;
		if (counter <= 100)
			msg_queue.emplace(counter, std::format("Hello from T[{}]", this_id));
		else
			stop_src.request_stop(); // final message sent - close the communication channels
		lck.unlock();
		cv.notify_one();
		std::this_thread::sleep_for(10ms);
	}
}

int main() {
	std::mutex mtx;
	std::condition_variable cv;
	std::queue<Message> msg_queue;
	std::stop_source stop_src;

	msg_queue.emplace(0, "first message");

	{
		std::jthread t1(runPingPong, std::ref(mtx), std::ref(cv), std::ref(msg_queue), std::ref(stop_src));
		std::jthread t2(runPingPong, std::ref(mtx), std::ref(cv), std::ref(msg_queue), std::ref(stop_src));
	}

	std::osyncstream(std::cout) << "Finished execution\n";
	return 0;
}
