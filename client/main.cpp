#include "ClientApplication.hpp"


//TODO: arg parse  -p "port" -a "address"

int main()
{
	ClientApplication App{};

	App.Start("127.0.0.1", "12333");
}