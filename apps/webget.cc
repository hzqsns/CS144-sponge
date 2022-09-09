#include "socket.hh"
#include "util.hh"

#include <cstdlib>
#include <iostream>

using namespace std;

void get_URL(const string &host, const string &path) {
    // Your code here.

    // You will need to connect to the "http" service on
    // the computer whose name is in the "host" string,
    // then request the URL path given in the "path" string.

    // Then you'll need to print out everything the server sends back,
    // (not just one call to read() -- everything) until you reach
    // the "eof" (end of file).
    /*
    您将需要连接到计算机上的“http”服务，其名称在“host”字符串中，然后请求在“path”字符串中给出的URL路径。
    然后，您需要打印服务器返回的所有内容(不仅仅是一个read()调用——所有内容)，直到到达“eof”(文件结束)。
    */
    TCPSocket http_tcp;
    http_tcp.connect(Address(host, "http"));
    http_tcp.write("GET " + path + " HTTP/1.1\r\n");
    http_tcp.write("Host: " + host + "\r\n");
    http_tcp.write("Connection: close\r\n");
    http_tcp.write("\r\n");

    while (
        !http_tcp.eof()) {  //除非获取到EOF，否则必须循环从远程服务器读取信息。因为网络数据的传输可能断断续续，需要多次
                            // read。
        cout << http_tcp.read();
    }
    http_tcp.close();

    cerr << "Function called: get_URL(" << host << ", " << path << ").\n";
    cerr << "Warning: get_URL() has not been implemented yet.\n";
}

int main(int argc, char *argv[]) {
    try {
        if (argc <= 0) {
            abort();  // For sticklers: don't try to access argv[0] if argc <= 0.
        }

        // The program takes two command-line arguments: the hostname and "path" part of the URL.
        // Print the usage message unless there are these two arguments (plus the program name
        // itself, so arg count = 3 in total).
        if (argc != 3) {
            cerr << "Usage: " << argv[0] << " HOST PATH\n";
            cerr << "\tExample: " << argv[0] << " stanford.edu /class/cs144\n";
            return EXIT_FAILURE;
        }

        // Get the command-line arguments.
        const string host = argv[1];
        const string path = argv[2];

        // Call the student-written function.
        get_URL(host, path);
    } catch (const exception &e) {
        cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
