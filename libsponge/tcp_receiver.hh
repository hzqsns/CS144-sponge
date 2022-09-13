#ifndef SPONGE_LIBSPONGE_TCP_RECEIVER_HH
#define SPONGE_LIBSPONGE_TCP_RECEIVER_HH

#include "byte_stream.hh"
#include "stream_reassembler.hh"
#include "tcp_segment.hh"
#include "wrapping_integers.hh"

#include <optional>

//! \brief The "receiver" part of a TCP implementation.

//! Receives and reassembles segments into a ByteStream, and computes
//! the acknowledgment number and window size to advertise back to the
//! remote TCPSender.
//  接收并将片段重新组合成ByteStream，并计算出确认号码和窗口大小，以回馈给远程TCPSender。
class TCPReceiver {
    //! Our data structure for re-assembling bytes.
    StreamReassembler _reassembler;

    //! The maximum number of bytes we'll store.
    size_t _capacity;
    //! 给包设置的 isn编号
    WrappingInt32 _isn;
    bool _set_syn_flag;
    bool _set_fin_flag;

  public:
    //! \brief Construct a TCP receiver
    //!
    //! \param capacity the maximum number of bytes that the receiver will
    //!                 store in its buffers at any give time.
    //  接收器在任何时候都会在其缓冲区内存储的最大字节数。
    TCPReceiver(const size_t capacity)
        : _reassembler(capacity), _capacity(capacity), _isn(0), _set_syn_flag(false), _set_fin_flag(false) {}

    //! \name Accessors to provide feedback to the remote TCPSender
    //!@{

    //! \brief The ackno that should be sent to the peer
    //! \returns empty if no SYN has been received
    //!
    //! This is the beginning of the receiver's window, or in other words, the sequence number
    //! of the first byte in the stream that the receiver hasn't received.
    //  这是接收方窗口的开始，或者换句话说，这是接收方尚未收到的流中第一个字节的序列号。
    std::optional<WrappingInt32> ackno() const;

    //! \brief The window size that should be sent to the peer
    //!
    //! Operationally: the capacity minus the number of bytes that the
    //! TCPReceiver is holding in its byte stream (those that have been
    //! reassembled, but not consumed).
    //! 操作上：容量减去TCPReceiver在其字节流中持有的字节数（那些已经被重新组装但未被消耗的字节）。重新组装，但没有被消耗）。
    //! Formally: the difference between (a) the sequence number of
    //! the first byte that falls after the window (and will not be
    //! accepted by the receiver) and (b) the sequence number of the
    //! beginning of the window (the ackno).
    //  形式上：(a)窗口之后的第一个字节的序列号（不会被接收方接受）与(b)窗口开始的序列号（ackno）之间的差。(b)
    //  窗口开始的序列号（ackno）。

    size_t window_size() const;
    //!@}

    //! \brief number of bytes stored but not yet reassembled
    //  已存储但尚未重新组装的字节数
    size_t unassembled_bytes() const { return _reassembler.unassembled_bytes(); }

    //! \brief handle an inbound segment
    void segment_received(const TCPSegment &seg);

    //! \name "Output" interface for the reader
    //!@{
    ByteStream &stream_out() { return _reassembler.stream_out(); }
    const ByteStream &stream_out() const { return _reassembler.stream_out(); }
    //!@}
};

#endif  // SPONGE_LIBSPONGE_TCP_RECEIVER_HH
