#include "wrapping_integers.hh"

// Dummy implementation of a 32-bit wrapping integer

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

// template <typename... Targs>
// void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) {
    //(absolute seqno + isn) mod 2^32 == seqno
    //因为seqno已经是uint32_t类型了 相当于自动mod 2^32，所以我们只用把n转换成uint32_t然后返回即可
    return WrappingInt32{isn + static_cast<uint32_t>(n)};
}

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
//! TCP连接的两个流都有自己的ISN。
//  一个流从本地TCPSender到远程TCPReceiver，并具有一个ISN。
//  另一个流从远程TCPSender到本地TCPReceiver，有一个不同的ISN。
//
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    // DUMMY_CODE(n, isn, checkpoint);
    // uint64_t INT32_MOD = 1l << 32;
    // WrappingInt32 _seqno_checkpoint = wrap(checkpoint, isn);
    // bool flag = true;
    // uint32_t offset;
    // if (_seqno_checkpoint.raw_value() > n.raw_value()) {
    //     offset = _seqno_checkpoint.raw_value() - n.raw_value();
    //     offset = n.raw_value() - _seqno_checkpoint.raw_value();
    //     flag = false;
    // }

    // uint64_t ans;
    // if (flag) {
    //     ans = checkpoint - static_cast<uint64_t>(offset);
    // } else {
    //     ans = checkpoint + static_cast<uint64_t>(offset);
    // }

    // if (wrap(ans, isn).raw_value() == n.raw_value()) {
    //     std::cout << "ans= " << ans << " n=" << n.raw_value() << " checkpoint=" << checkpoint
    //               << " _seqno_checkpoint=" << _seqno_checkpoint.raw_value() << std::endl;
    //     return ans;
    // }
    WrappingInt32 wrap_checkpoint = wrap(checkpoint, isn);
    int32_t diff = n - wrap_checkpoint;
    int64_t res = checkpoint + diff;
    if (res < 0) {
        return res + (1ul << 32);
    }
    return res;

    // uint64_t ans2 = checkpoint + static_cast<uint64_t>(offset);
    // if (wrap(ans2, isn).raw_value() == n.raw_value()) {
    //     std::cout << "ans2" << std::endl;
    //     return ans2;
    // }
    // std::cout << "none" << std::endl;
    // return 0;

    // 暴力做法
    // uint64_t INT32_RANGE = 1l << 32;
    // uint32_t offset = (n.raw_value() - isn.raw_value() + INT32_RANGE) % INT32_RANGE;
    // uint64_t res = offset;
    // uint64_t diff = checkpoint > res ? checkpoint - res : res - checkpoint;
    // for (int i = 32; i <= 64; i++) {
    //     uint64_t tmp = 1l << i;
    //     uint64_t difference = ((tmp + offset) > checkpoint) ? (tmp + offset - checkpoint) : (checkpoint - tmp -
    //     offset); if (difference < diff) {
    //         res = tmp + offset;
    //         diff = difference;
    //     }
    //     //264  270
    // }
    // std::cout << "res= " << res << " n=" << n.raw_value() << " checkpoint=" << checkpoint << " isn=" <<
    // isn.raw_value()
    //           << std::endl;
    // return res;
}
