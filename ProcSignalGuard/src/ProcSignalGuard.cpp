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

