#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

// template <typename... Targs>
// void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

//! \brief handle an inbound segment
void TCPReceiver::segment_received(const TCPSegment &seg) {
    // DUMMY_CODE(seg);
    TCPHeader _tcp_header = seg.header();
    // LISTEN：等待 SYN 包的到来。若在 SYN 包到来前就有其他数据到来，则必须丢弃。
    if (!_set_syn_flag) {
        if (!_tcp_header.syn) {
            return;
        }
        _isn = _tcp_header.seqno;
        _set_syn_flag = true;
    }

    // SYN_RECV：获取到了 SYN 包，此时可以正常的接收数据包

    uint64_t _absolute_seqno = unwrap(_tcp_header.seqno, _isn, _reassembler.stream_out().bytes_written());
    uint64_t _stream_index = _tcp_header.syn ? 0 : _absolute_seqno - 1;
    // 此时没有被return的话我们继续往下进行判断，有可能接收到fin包
    // FIN_RECV：获取到了 FIN 包，此时务必终止 ByteStream 数据流的输入。
    if (_tcp_header.fin)
        _set_fin_flag = true;
    _reassembler.push_substring(seg.payload().copy(), _stream_index, _tcp_header.fin);
}

//! \name Accessors to provide feedback to the remote TCPSender
//!@{

//! \brief The ackno that should be sent to the peer
//! \returns empty if no SYN has been received
//!
//! This is the beginning of the receiver's window, or in other words, the sequence number
//! of the first byte in the stream that the receiver hasn't received.
//  这是接收方窗口的开始，或者换句话说，这是接收方尚未收到的流中第一个字节的序列号。
optional<WrappingInt32> TCPReceiver::ackno() const {
    if (!_set_syn_flag)
        return {};
    if (_set_fin_flag && _reassembler.stream_out().input_ended()) {
        return wrap(_reassembler.stream_out().bytes_written() + 2, _isn);
    }

    return wrap(_reassembler.stream_out().bytes_written() + 1, _isn);
}

//! \brief The window size that should be sent to the peer
//!
//! Operationally: the capacity minus the number of bytes that the
//! TCPReceiver is holding in its byte stream (those that have been
//! reassembled, but not consumed).
//! 操作上：容量减去TCPReceiver在其字节流中持有的字节数（那些已经被重新组装但未被消耗的字节）。
//  重新组装，但没有被消耗）。
//! Formally: the difference between (a) the sequence number of
//! the first byte that falls after the window (and will not be
//! accepted by the receiver) and (b) the sequence number of the
//! beginning of the window (the ackno).
//  形式上：(a)窗口之后的第一个字节的序列号（不会被接收方接受）与(b)窗口开始的序列号（ackno）之间的差。(b)
//  窗口开始的序列号（ackno）。
size_t TCPReceiver::window_size() const { return _capacity - _reassembler.stream_out().buffer_size(); }
