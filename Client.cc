/*
 *          King of Knights
 *          19-08-2017
 */

#include <boost/asio.hpp>
#include <fstream>

void fileWritting(boost::asio::ip::tcp::socket &socket1)
{
    try
    {

        boost::asio::streambuf streambuf;
        boost::system::error_code errorCode;
        boost::asio::streambuf::mutable_buffers_type buf = streambuf.prepare(20);
        size_t sizeReceived = boost::asio::read(socket1, buf, errorCode);
        streambuf.commit(sizeReceived);

        std::istream streambuf1(&streambuf);
        std::string filename{};
        streambuf1 >> filename;

        std::ofstream file;
        file.open("Test.txt");
        assert(file.is_open());

        while (true)
        {
            boost::asio::streambuf streambuf2;
            boost::system::error_code error_code;
            boost::asio::streambuf::mutable_buffers_type buffers_type = streambuf2.prepare(64);
            size_t  size = boost::asio::read(socket1, buffers_type, error_code);
            if (size == 0)
                break;
            streambuf2.commit(size);

            std::istream istream(&streambuf2);
            std::string data{};
            istream >> data;
            file.write(data.data(), size);
        }
        file.close();
        socket1.close();
    }
    catch (boost::system::error_code &error_code)
    {
        std::cerr << error_code.message() << std::endl;
    }
    catch (std::exception &exception)
    {
        std::cerr << exception.what() << std::endl;
    }
}

int main(int argc, char *argv[])
{
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket1(io_service);
    boost::asio::ip::tcp::resolver resolver(socket1.get_io_service());
    boost::asio::ip::tcp::resolver::query query("localhost", "9999");
    boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
    boost::asio::connect(socket1, iterator);

    fileWritting(socket1);
    io_service.run();
    return 0;
}
