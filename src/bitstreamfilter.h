// #pragma once
//
// #include "averror.h"
// #include "avutils.h"
// #include "codecparameters.h"
// #include "ffmpeg.h"
// #include "packet.h"
// #include <string_view>
// extern "C" {
// #include <libavcodec/bsf.h>
// }
//
// namespace av {
// class BitStreamFilter : public FFWrapperPtr<AVBSFContext>, public noncopyable
// {
// public:
//     BitStreamFilter() = default;
//     BitStreamFilter(BitStreamFilter&& other);
//     friend void swap(BitStreamFilter& lhs, BitStreamFilter& rhs) noexcept;
//     BitStreamFilter& operator=(BitStreamFilter&& other);
//     ~BitStreamFilter();
//
//     void setName(std::string_view name, OptionalErrorCode ec);
//
//     void setInCodecParameters(CodecParametersView codecpar, OptionalErrorCode ec);
//     void setOutCodecParameters(CodecParametersView codecpar, OptionalErrorCode ec);
//
//     void init();
//
//     bool isInitilized() const;
//
//     void sendPacket(Packet& packet, OptionalErrorCode ec);
//     void receivePacket(Packet& packet, OptionalErrorCode ec);
//
// private:
//     bool m_is_initialized = false;
// };
//
// } // namespace av
