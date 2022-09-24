#include "ProcSignalGuard.hpp"

ProcSignalGuard::ProcSignalGuard(std::initializer_list<int> signals) : mSignalSet()
{
	for (int sig : signals)
	{
		sigaddset(&mSignalSet, sig);
	}

	sigprocmask(SIG_BLOCK, &mSignalSet, 0);
}

ProcSignalGuard::~ProcSignalGuard()
{
	sigprocmask(SIG_UNBLOCK, &mSignalSet, 0);
}

void ProcSignalGuard::Wait(const std::function<void(int)>& handler)
{
	int sig;
	sigwait(&mSignalSet, &sig);
	handler(sig);
}

