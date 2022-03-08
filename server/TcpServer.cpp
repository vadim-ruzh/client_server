#include <iostream>
#include <queue>
#include <boost/asio.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <string>
#include <thread>

//class Context
//{
//    using Task = std::function<void()>;
//
//public:
//    class Work;
//    class Strand;
//
//    Context()
//        : m_isWorking(false)
//    {
//    }
//
//	void post(Task task)
//	{
//        boost::unique_lock<boost::shared_mutex> tasksLock(m_tasksChange);
//		m_tasks.push(task);
//	}
//
//    void poll()
//	{
//        Task task;
//        {
//            boost::upgrade_lock<boost::shared_mutex> sharedTasksLock(m_tasksChange);
//            if (m_tasks.empty())
//                return;
//
//            boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueTasksLock(sharedTasksLock);
//            task = m_tasks.front();
//            m_tasks.pop();
//
//           /* {
//                boost::shared_lock<boost::shared_mutex> tasksLock(m_tasksChange);
//                if (m_tasks.empty())
//                    return;
//            }
//
//	        {
//		        boost::unique_lock<boost::shared_mutex> tasksLock(m_tasksChange);
//            	task = m_tasks.front();
//            	m_tasks.pop();
//	        }*/
//        }
//        task();
//	}
//
//    void run()
//    {
//        bool empty = false;
//	    do 
//	    {
//            {
//                boost::shared_lock<boost::shared_mutex> tasksLock(m_tasksChange);
//                empty = m_tasks.empty();
//            }
//            poll();
//        } while (!empty || m_isWorking);
//    }
//
//private:
//    boost::shared_mutex m_tasksChange;
//
//    std::queue<Task> m_tasks;
//    bool m_isWorking;
//};
//
//class Context::Work
//{
//public:
//    Work(Context& context)
//	    : m_context(context)
//    {
//        m_context.m_isWorking = true;
//    }
//    ~Work()
//    {
//        m_context.m_isWorking = false;
//    }
//private:
//    Context& m_context;
//};
//
//class Context::Strand
//{
//public:
//    Strand(Context& context)
//        : m_context(context)
//		, m_task(nullptr)
//		, thread([this]() { Run(); })
//    {
//    }
//
//    void post(Task task)
//    {
//        m_tasks.push(task);
//        // m_context.post(task);
//
//    }
//    
//private:
//    void Run()
//    {
//        while (true)
//        {
//            if (!m_tasks.empty())
//            {
//                auto& task = m_tasks.front();
//                bool& isProcessed = m_context.post(task);
//                while (!isProcessed)
//                {
//                }
//            }
//        }
//    }
//
//    std::queue<Task> m_tasks;
//    Context& m_context;
//    boost::thread thread;
//
//    Task* m_task;
//};
//

class IMessageProcessor
{
public:
    virtual void Process(std::shared_ptr<boost::asio::ip::tcp::socket> sock, char* request, size_t size) = 0;
};

class Callback
{
public:
    Callback(std::shared_ptr<IMessageProcessor> processor, std::shared_ptr<IMessageProcessor> errorProcessor)
		: m_processor(std::move(processor))
		, m_errorProcessor(std::move(errorProcessor))
	{
	    
    }

    void operator()(std::shared_ptr<boost::asio::ip::tcp::socket> sock, boost::system::error_code ec)
    {
        if (boost::system::errc::operation_canceled == ec
            || boost::asio::error::operation_aborted == ec
            || boost::this_thread::interruption_requested())
        {
            return;
        }

        char request[256];
        //Чтение запроса из сокета
        try
        {
            sock->async_read_some(boost::asio::buffer(request), boost::asio::use_future).get();
        }
        catch (const std::exception&)
        {

        }

        if (boost::this_thread::interruption_requested())
        {
            return;
        }
        
        char requestType = request[0];
        if(requestType == 0)
        {
            m_errorProcessor->Process(sock, request, 256);
        }
        else
        {
            m_processor->Process(sock, request, 256);
        }


        sock->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    }

    std::shared_ptr<IMessageProcessor> m_processor;
    std::shared_ptr<IMessageProcessor> m_errorProcessor;
};

class MessageProcessor
    : public IMessageProcessor
{
public:
	void Process(std::shared_ptr<boost::asio::ip::tcp::socket> sock, char* request, size_t size) override
	{
        //Поиск ip-адресса по заданному запросу
        boost::asio::io_context context;
        boost::asio::ip::tcp::resolver resolver{ context };
        auto query = boost::asio::ip::tcp::resolver::query{ request,"80" };

        boost::system::error_code errorCode;
        auto iter = resolver.resolve(query, errorCode);
        if (errorCode)
        {
            boost::asio::write(sock, boost::asio::buffer("adress not found"));
        }
        else
        {
            boost::asio::ip::tcp::endpoint anwser = *iter;
            char adress[256];
            strcpy_s(adress, anwser.address().to_string().c_str());

            std::cout << "recieve " << "=" << request << std::endl;
            std::cout << "send " << "=" << adress << std::endl;

            boost::asio::write(sock, boost::asio::buffer(adress));
        }
	}
};

class ErrorMessageProcessor : public IMessageProcessor
{
public:
    void Process(std::shared_ptr<boost::asio::ip::tcp::socket> sock, char* request, size_t size) override
    {
        std::cerr << "error";
    }
};

// IServer::Start -> TcpServer::Start

// new TcpServer
// IServer::Start | new TcpServer

// TcpServer memory
// 0x0 IServer | 0x1 TcpServer

enum ConnectionType
{
	Udp,
    Tcp
};

class IServer
{
public:
    using Connection = boost::asio::ip::tcp::socket;
    using Callback = std::function<void(std::shared_ptr<Connection>, boost::system::error_code)>;

    virtual void Start(Callback callback, uint16_t port, size_t workersCount = 2) = 0;
    virtual void Stop() = 0;
};

class TcpServer
    : public IServer
{

public:
    using Connection = boost::asio::ip::tcp::socket;
    using Callback = std::function<void(std::shared_ptr<Connection>, boost::system::error_code)>;

    ~TcpServer()
    {
        try
        {
            Stop();
        }
        catch (...)
        {
            // TODO: log exception
        }
    }

    void Start(Callback callback, uint16_t port, size_t workersCount = 2) override
    {
        std::unique_lock<std::mutex> stateLock(m_stateChange);

        if (m_context || m_acceptor || m_work)
        {
            throw std::exception();
        }

        m_context = std::make_shared<boost::asio::io_context>();
        m_work = std::make_unique<boost::asio::io_context::work>(*m_context);

        for (size_t i = 0; i < workersCount; ++i)
        {
            m_workers.create_thread(
                [context = m_context]
                {
                    try
                    {
                        context->run();
                    }
                    catch (...)
                    {

                    }
                });
        }

        boost::asio::ip::tcp::endpoint endp{
            boost::asio::ip::tcp::v4(), port
        };

        m_acceptor = std::make_unique<boost::asio::ip::tcp::acceptor>(*m_context, endp);

        auto sock = std::make_shared<Connection>(boost::asio::make_strand(*m_context));
        //Ожидание подключения и передача его в сокет 
        m_acceptor->async_accept(
            *sock, boost::bind(&HandleConnection, sock, callback, boost::asio::placeholders::error));
    }
    void Stop() override
    {
        std::unique_lock<std::mutex> stateLock(m_stateChange);

        if (m_acceptor)
            m_acceptor->cancel();

        m_work.reset();

        m_workers.interrupt_all();
        m_workers.join_all();

        m_acceptor.reset();
        m_context.reset();
    }

protected:
    virtual void HandleConnection(
        std::shared_ptr<Connection> sock, Callback callback, boost::system::error_code ec)
    {
        callback(sock, ec);

        if (boost::system::errc::operation_canceled == ec
            || boost::asio::error::operation_aborted == ec)
        {
            return;
        }

        auto sock = std::make_shared<Connection>(boost::asio::make_strand(*m_context));
        m_acceptor->async_accept(
            *sock, boost::bind(&HandleConnection, sock, boost::asio::placeholders::error));
    }

private:
    std::shared_ptr<boost::asio::io_context> m_context;
    std::unique_ptr<boost::asio::io_context::work> m_work;
    std::unique_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;

    boost::thread_group m_workers;

    std::mutex m_stateChange;

};

class UdpServer
    : public IServer
{
public:
    using Connection = boost::asio::ip::udp::socket;
    using Callback = std::function<void(std::shared_ptr<Connection>, boost::system::error_code)>;

    void Start(Callback callback, uint16_t port, size_t workersCount = 2) override;
    void Stop() override;
};

std::unique_ptr<IServer> MakeServer(ConnectionType ct)
{
    switch (ct)
	{
	    case Udp: return std::make_unique<UdpServer>();
	    case Tcp: return std::make_unique<TcpServer>();
    }
}

// LoggerServer memory
// 0x0 IServer | 0x1 TcpServer | 0x2 LoggerServer
class LoggerServer
    : public IServer
{
public:
    LoggerServer(std::unique_ptr<IServer> server)
	    : m_server(std::move(server))
    {
	    
    }

    void Start(Callback callback, uint16_t port, size_t workersCount = 2) override
    {  
        std::cout << "Start";

        m_server->Start(callback, port, workersCount);

        std::cout << "Start success";
    }
    void Stop() override
    {
        std::cout << "Stop";

        m_server->Stop();

        std::cout << "Stop success";
    }

//private:
//    void HandleConnection(
//        std::shared_ptr<Connection> sock, Callback callback, boost::system::error_code ec) override
//    {
//        std::cout << "HandleConnection";
//
//        TcpServer::HandleConnection(sock, callback, ec);
//
//        std::cout << "HandleConnection success";
//    }

private:
    std::unique_ptr<IServer> m_server;
};

int main()
{
    ConnectionType ct = ConnectionType::Udp;
    auto server = MakeServer(ct);
    /*
    Context context;
    Context::Work work(context);
    Context::Strand strand(context);
    */
    
    server = std::make_unique<LoggerServer>(std::move(server));

    server->Start(Callback{ std::make_shared<MessageProcessor>(), std::make_shared<ErrorMessageProcessor>() }, 6666);

    while(true)
    {
        bool isTerminate = false;
        if(isTerminate)
        {
            server->Stop();
            break;
        }
    }
}

