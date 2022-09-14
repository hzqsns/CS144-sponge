#include "tcp_sender.hh"

#include "tcp_config.hh"

#include <random>

// Dummy implementation of a TCP sender

// For Lab 3, please replace with a real implementation that passes the
// automated checks run by `make check_lab3`.

// template <typename... Targs>
// void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! \param[in] capacity the capacity of the outgoing byte stream
//! \param[in] retx_timeout the initial amount of time to wait before retransmitting the oldest outstanding segment
//! \param[in] fixed_isn the Initial Sequence Number to use, if set (otherwise uses a random ISN)
TCPSender::TCPSender(const size_t capacity, const uint16_t retx_timeout, const std::optional<WrappingInt32> fixed_isn)
    : _isn(fixed_isn.value_or(WrappingInt32{random_device()()}))
    , _initial_retransmission_timeout{retx_timeout}
    , _stream(capacity) {}

uint64_t TCPSender::bytes_in_flight() const { return {}; }

//! \brief create and send segments to fill as much of the window as possible
//         创建和发送段以尽可能多地填充窗口
void TCPSender::fill_window() {}

//! \param ackno The remote receiver's ackno (acknowledgment number)
//               远程接收者的ackno(确认号)
//! \param window_size The remote receiver's advertised window size
//                     远程接收者的通告窗口大小
void TCPSender::ack_received(const WrappingInt32 ackno, const uint16_t window_size) {
    // DUMMY_CODE(ackno, window_size);
}

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
//                                自最后一次调用此方法以来的毫秒数
void TCPSender::tick(const size_t ms_since_last_tick) {
    // DUMMY_CODE(ms_since_last_tick);
}

unsigned int TCPSender::consecutive_retransmissions() const { return {}; }

//! \brief Generate an empty-payload segment (useful for creating empty ACK segments)
//         生成空的有效负载段(用于创建空的ACK段)
void TCPSender::send_empty_segment() {}
