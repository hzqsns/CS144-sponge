#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.
// 内存中流控制字节流的虚拟实现

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`
// 您需要在' byte_stream.hh '的类声明中添加私有成员。

// template <typename... Targs>
// void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity)
    : deque_(), capacity_size_(capacity), written_size_(0), read_size_(0), end_input_(false), _error(false) {}

//! Write a string of bytes into the stream. Write as many
//! as will fit, and return how many were written.
//! \returns the number of bytes accepted into the stream
/*将一个字节串写入流。 写入尽可能多的内容，并返回已写入的内容。 返回流接受的字节数 */
size_t ByteStream::write(const string &data) {
    // DUMMY_CODE(data);
    if (end_input_)
        return 0;
    size_t write_size = std::min(data.size(), capacity_size_ - deque_.size());
    written_size_ += write_size;
    for (size_t i = 0; i < write_size; i++)
        deque_.push_front(data[i]);
    return write_size;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    // DUMMY_CODE(len);
    std::string res = "";
    size_t temp = len;
    auto it = deque_.rbegin();
    while (temp-- && it != deque_.rend()) {
        res += (*it);
        it++;
    }

    return res;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    //  DUMMY_CODE(len);
    size_t temp = len;
    while (!deque_.empty() && temp--) {
        deque_.pop_back();
        read_size_++;
    }
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    // DUMMY_CODE(len);
    std::string res = this->peek_output(len);
    this->pop_output(len);
    return res;
}

// Signal that the byte stream has reached its ending
void ByteStream::end_input() { end_input_ = true; }

//! \returns `true` if the stream input has ended
bool ByteStream::input_ended() const { return end_input_; }

//! \returns the maximum amount that can currently be read from the stream
size_t ByteStream::buffer_size() const { return deque_.size(); }

//! \returns `true` if the buffer is empty
bool ByteStream::buffer_empty() const { return deque_.empty(); }

//! \returns `true` if the output has reached the ending
bool ByteStream::eof() const { return end_input_ && deque_.empty(); }

//! Total number of bytes written
size_t ByteStream::bytes_written() const { return written_size_; }

//! Total number of bytes popped
size_t ByteStream::bytes_read() const { return read_size_; }

//! \returns the number of additional bytes that the stream has space for
size_t ByteStream::remaining_capacity() const { return capacity_size_ - deque_.size(); }
