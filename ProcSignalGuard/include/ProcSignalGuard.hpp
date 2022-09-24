#ifndef PROCSIGNALGUARD_HPP_
#define PROCSIGNALGUARD_HPP_

#include <initializer_list>
#include <utility>
#include <csignal>
#include <functional>


/**
* @brief The class blocks signals are passed to its constructor for the whole process.
*/
class ProcSignalGuard
{
 public:
	ProcSignalGuard(std::initializer_list<int> signals);

	~ProcSignalGuard();

	ProcSignalGuard(ProcSignalGuard&&) = delete;
	ProcSignalGuard(const ProcSignalGuard&) = delete;
	ProcSignalGuard operator=(ProcSignalGuard&&) = delete;
	ProcSignalGuard operator=(const ProcSignalGuard&) = delete;

	
	/**
	* @brief Waiting for signals, when signals occur, a handler is called.
	*/
	void Wait(const std::function<void(int signum)>& handler);
 private:
	sigset_t mSignalSet;
};

#endif //PROCSIGNALGUARD_HPP_
