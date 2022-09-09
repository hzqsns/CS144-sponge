#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`
// 您需要在' stream_reassemble .hh '的类声明中添加私有成员。

// template <typename... Targs>
// void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity)
    : _next_assembled_index(0)
    , _unassembled_bytes_num(0)
    , _eof_index(-1)
    , _unassembled_string_mp()
    , _output(capacity)
    , _capacity(capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
/*
该函数从逻辑流中接收一个子字符串(也称段)，可能是无序的字节，
并将任何新的字节汇编起来连续的子字符串，并按顺序将它们写入输出流。
*/

void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    // DUMMY_CODE(data, index, eof);

    // 1.首先处理传入字符串与前面的重叠情况
    //   根据迭代器找到前一个字符串
    // 首先根据upper_bound找到大于index的第一个字符串
    auto pos_iter = _unassembled_string_mp.upper_bound(index);
    // 这样它的前一个字符串就一定是比index小的那个字符串了
    if (pos_iter != _unassembled_string_mp.begin()) {
        pos_iter--;
    }
    // 重叠处理之后新字符串的起始位置
    size_t new_idx = index;
    //判断这个字符串与当前push进来的字符串的重叠情况
    if (pos_iter != _unassembled_string_mp.end() && pos_iter->first <= index) {
        string _pre_string = pos_iter->second;
        size_t _pre_index_start = pos_iter->first;
        size_t _pre_index_end = _pre_index_start + _pre_string.size() - 1;

        // 既然这个字符串是前一个字符串，那我们只用考虑是否重叠
        // 一种就是部分重叠，即当前字符串的前部与前一个字符串的后部重叠
        // 前面重叠的部分我们就不要了，最后再更新
        if (_pre_index_end >= index) {
            new_idx = _pre_index_end + 1;
        }
    } else if (index < _next_assembled_index) {
        // 如果当前字符串前面没有字符串
        // 那这个时候我们就看它的前面有没有已经被读取，如果index <
        // _next_assembled_index，说明这个字符串前面有一部分已经被push进bytesteam了
        // 那么这个时候就从_next_assembled_index开始读起，_next_assembled_index之前的数据就不要了，因为已经加载进bytesteam了
        // 所以new_idx就是_next_assembled_index
        new_idx = _next_assembled_index;
    }

    // index是data第一个字节对应的下标，因为前面我们有可能要舍弃一部分字节
    // 所以我们要到data的中间new_idx开始处理，那么对应data真正的下标开始位置就是new_idx - index
    size_t _data_start_pos = new_idx - index;
    ssize_t _data_size = data.size() - _data_start_pos;

    // 2.然后再处理字符串与后面的重叠情况
    // 找到与当前字符串距离最近的后一个字符串
    pos_iter = _unassembled_string_mp.lower_bound(new_idx);
    // 这里我们需要注意的一个点就是，前面的字符串我们只用考察一次重叠的情况
    // 但是后面重叠的字符串我们需要考虑多次
    // 比如有可能当前的字符串范围很大包含了后面很多字符串
    while (pos_iter != _unassembled_string_mp.end() && new_idx <= pos_iter->first) {
        string _post_string = pos_iter->second;
        size_t _post_index_start = pos_iter->first;
        size_t _post_index_end = _post_index_start + _post_string.size();
        size_t _data_end_pos = new_idx + _data_size;
        // 如果存在重叠区域
        if (_data_end_pos > _post_index_start) {
            // 如果只是部分重叠，即前一个字符串的后部与后一个字符串的前部产生重叠
            // 注意这里不能加等号，等号就意味着完全重叠了
            if (_data_end_pos < _post_index_end) {
                //_post_index_start-1  - new_idx + 1
                _data_size = _post_index_start - new_idx;
                break;
            } else {  // 否则就是完全重叠，即前一个字符串全部把后一个字符串全部包含了
                // 那么我们此时去除掉这个完全重叠的字符串即可
                // 因为我们下一步就会push进bytesteam中，如果push不了我们也会将这个大的字符串重新插入_unassembled_string_mp中
                _unassembled_bytes_num -= _post_string.size();
                pos_iter = _unassembled_string_mp.erase(pos_iter);
                continue;
            }
        } else {  //如果不存在重叠区域，直接break掉
            break;
        }
    }

    // 我们此时找出了插入的new_idx以及插入长度_data_size
    // 但是我们还要查看我们能够接收的下标范围和当前插入的是否符合要求
    // _next_assembled_index是下一个可接收的下标
    // _capacity是总的容量，即相当于红色和绿色区域加起来的最大可接收大小
    // _output.buffer_size()即相当于bytesteam已经接收的大小，相当于绿色区域
    // 于是_end_accept_index就相当于找到了可以接收下标中最远的下标
    size_t _end_accept_index = _next_assembled_index + _capacity - _output.buffer_size();

    // 可以接收的范围为[_next_assembled_index,_end_accept_index]
    // 如果_end_accept_index 比 new_idx要小的话说明当前的下标就不在我们接收的范围里面,只能被迫丢弃
    // if (_end_accept_index < new_idx) {
    //     return;
    // } else

    if (_end_accept_index >= new_idx) {  //如果能接收那就插入我们找出的字符
        if (_data_size > 0) {
            string _push_data = data.substr(_data_start_pos, _data_size);
            if (new_idx ==
                _next_assembled_index) {  //如果当前起始下标正好与_next_assembled_index对应，插入到bytesteam中
                size_t _written_size = _output.write(_push_data);
                _next_assembled_index += _written_size;
                if (_written_size <
                    _push_data.size()) {  //说明bytesteam已经满了，这个时候多的部分就要插入到_unassembled_string_mp中
                    string _spare_data = _push_data.substr(_written_size, _push_data.size() - _written_size);
                    _unassembled_bytes_num += _spare_data.size();
                    _unassembled_string_mp.insert({_next_assembled_index, _spare_data});
                    //_unassembled_string_mp.insert(make_pair(_next_assembled_index, std::move(_spare_data)));
                }
            } else {  //否则，插入到_unassembled_string_mp中
                string _spare_data = _push_data.substr(0, _push_data.size());
                _unassembled_bytes_num += _spare_data.size();
                _unassembled_string_mp.insert({new_idx, _spare_data});
                //_unassembled_string_mp.insert(make_pair(new_idx, std::move(_spare_data)));
            }
        }
    }

    // 3.最后再处理一遍_unassembled_string_mp中看是否有能插入的
    auto iter = _unassembled_string_mp.begin();
    while (iter != _unassembled_string_mp.end()) {
        if (iter->first == _next_assembled_index) {
            size_t _written_num = _output.write(iter->second);
            _next_assembled_index += _written_num;
            if (_written_num < iter->second.size()) {
                string _spare_data = iter->second.substr(_written_num);
                size_t _spare_data_size = iter->second.size() - _written_num;

                _unassembled_bytes_num += _spare_data_size;
                //插入没写到bytesteam的字符串
                _unassembled_string_mp.insert({_next_assembled_index, _spare_data});
                //_unassembled_string_mp.insert(make_pair(_next_assembled_index, std::move(_spare_data)));

                //同时还要删除掉原来的字符串，因为已经部分写到bytesteam了
                _unassembled_bytes_num -= iter->second.size();
                _unassembled_string_mp.erase(iter);

                //因为这个时候bytesteam已经写满了，所以不再循环了，直接break
                break;
            }
            _unassembled_bytes_num -= iter->second.size();
            iter = _unassembled_string_mp.erase(iter);
        } else {
            break;
        }
    }

    //判断eof
    if (eof) {
        _eof_index = index + data.size();
    }

    if (_eof_index <= _next_assembled_index) {
        _output.end_input();
    }
}
//! The number of bytes in the substrings stored but not yet reassembled
//! 已存储但尚未重组的子字符串中的字节数
//! \note If the byte at a particular index has been pushed more than once, it
//! should only be counted once for the purpose of this function.
//  如果位于特定索引的字节已经被推入多次，那么对于这个函数来说，它应该只计算一次。
size_t StreamReassembler::unassembled_bytes() const { return _unassembled_bytes_num; }

//! \brief Is the internal state empty (other than the output stream)?
//         内部状态是空的(除了输出流)?
//! \returns `true` if no substrings are waiting to be assembled
//         如果没有子字符串等待组装，则返回' true '
bool StreamReassembler::empty() const { return _unassembled_bytes_num == 0; }
