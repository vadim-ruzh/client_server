#ifndef PROCSIGNALGUARD_HPP_
#define PROCSIGNALGUARD_HPP_

#include <initializer_list>
#include <utility>
#include <csignal>
#include <functional>

class ProcSignalGuard
{
 public:
	ProcSignalGuard(std::initializer_list<int> signals);

	~ProcSignalGuard();

	// TODO: use boost::noncopyble
	ProcSignalGuard(ProcSignalGuard&&) = delete;
	ProcSignalGuard(const ProcSignalGuard&) = delete;
	ProcSignalGuard operator=(ProcSignalGuard&&) = delete;
	ProcSignalGuard operator=(const ProcSignalGuard&) = delete;

	// TODO: move out of class scope
	template<typename Func>
	void Wait(Func&& functor)
	{
		int sig;
		sigwait(&mSignalSet, &sig);

		std::invoke(std::forward<Func>(functor), sig);
	}
 private:
	sigset_t mSignalSet;
};

#endif //PROCSIGNALGUARD_HPP_
