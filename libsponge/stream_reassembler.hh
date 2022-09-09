#ifndef SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH
#define SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH

#include "byte_stream.hh"

#include <cstdint>
#include <map>
#include <string>

//! \brief A class that assembles a series of excerpts from a byte stream (possibly out of order,
//! possibly overlapping) into an in-order byte stream.
/*将字节流的一系列摘录(可能是无序的，可能是重叠的)汇编成有序字节流的类。*/
class StreamReassembler {
  private:
    // Your code here -- add private members as necessary.
    //_next_assembled_index之前的都已经重组完毕并且装进了bytesteam，[_next_assembled_index,+∞)后下标对应的还未装配
    size_t _next_assembled_index;
    //_unassembled_bytes_num
    size_t _unassembled_bytes_num;
    //设置为最后一个结束字符的下标
    size_t _eof_index;
    //_unassembled_string_mp存放整个数据，其中key为index，value为string，表示暂存的字符串
    std::map<size_t, std::string> _unassembled_string_mp;
    ByteStream _output;  //!< The reassembled in-order byte stream 重新组装的有序字节流
    size_t _capacity;    //!< The maximum number of bytes

  public:
    //! \brief Construct a `StreamReassembler` that will store up to `capacity` bytes.
    //         构造一个将存储最多“容量”字节的“StreamReassembler”。
    //! \note This capacity limits both the bytes that have been reassembled,
    //! and those that have not yet been reassembled.
    //         这种容量限制了已重组的字节和尚未重组的字节。
    StreamReassembler(const size_t capacity);

    //! \brief Receive a substring and write any newly contiguous bytes into the stream.
    //!        接收一个子字符串并将任何新连续的字节写入流。
    //! The StreamReassembler will stay within the memory limits of the `capacity`.
    //         StreamReassembler将停留在“容量”的内存限制内。
    //! Bytes that would exceed the capacity are silently discarded.
    //!        超过容量的字节将被静默丢弃。
    //! \param data the substring
    //! \param index indicates the index (place in sequence) of the first byte in `data`
    //               指示' data '中第一个字节的索引(按顺序排列)。
    //! \param eof the last byte of `data` will be the last byte in the entire stream
    //             ' data '的最后一个字节将是整个流中的最后一个字节
    void push_substring(const std::string &data, const uint64_t index, const bool eof);

    //! \name Access the reassembled byte stream 访问重新组装的字节流
    //!@{
    const ByteStream &stream_out() const { return _output; }
    ByteStream &stream_out() { return _output; }
    //!@}

    //! The number of bytes in the substrings stored but not yet reassembled
    //! 已存储但尚未重组的子字符串中的字节数
    //! \note If the byte at a particular index has been pushed more than once, it
    //! should only be counted once for the purpose of this function.
    //  如果位于特定索引的字节已经被推入多次，那么对于这个函数来说，它应该只计算一次。
    size_t unassembled_bytes() const;

    //! \brief Is the internal state empty (other than the output stream)?
    //         内部状态是空的(除了输出流)?
    //! \returns `true` if no substrings are waiting to be assembled
    //         如果没有子字符串等待组装，则返回' true '
    bool empty() const;
};

#endif  // SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH
