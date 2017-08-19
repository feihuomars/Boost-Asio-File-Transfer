/*
 *          King of Knights
 *          19-08-2017
 */

#include <boost/asio.hpp>
#include <fstream>

void fileReading(std::string &filename, boost::asio::ip::tcp::socket &socket)
{
    try
    {
        std::ifstream file;
        file.open(filename);
        if (!file.is_open())
        {
            std::cerr << __TIMESTAMP__ <<" File is Not Opened!" << std::endl;
            exit(1);
        }

        boost::asio::async_write(socket, boost::asio::buffer(filename),
                                 [](const boost::system::error_code &error_code, size_t size)
                                 {
                                     if (error_code != nullptr)
                                         std::cout << __TIMESTAMP__ << "Transfer Started!" << std::endl;
                                 });

        while (!file.eof())
        {
            std::string string{};
            boost::system::error_code error_code;
            file >> string;
            boost::asio::write(socket, boost::asio::buffer(string), error_code);
        }
        file.close();
    }
    catch (boost::system::error_code &errorCode)
    {
        std::cerr << errorCode.message() << std::endl;
    }
    catch (std::exception &exception)
    {
        std::cerr << exception.what() << std::endl;
    }
}

int main(int argc, char* argv[])
{
    try
    {
        boost::asio::io_service io_service;
        boost::asio::ip::tcp::socket socket1(io_service);
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 9999);
        boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint);
        boost::system::error_code errorCode;
        acceptor.accept(socket1, errorCode);

        std::string string("item.txt");
        fileReading(string, socket1);
        io_service.run();
    }
    catch (std::exception &exception)
    {
        std::cerr << exception.what() << std::endl;
    }

    return 0;
}
