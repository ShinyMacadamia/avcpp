#pragma once

#include "averror.h"
#include "avutils.h"
#include "codecparameters.h"
#include "ffmpeg.h"
#include "packet.h"
#include <string_view>
extern "C" {
#include <libavcodec/bsf.h>
}

namespace av {
class BitStreamFilter : public FFWrapperPtr<AVBSFContext>, public noncopyable
{
public:
    BitStreamFilter() = default;
    BitStreamFilter(BitStreamFilter&& other);
    friend void swap(BitStreamFilter& lhs, BitStreamFilter& rhs) noexcept;
    BitStreamFilter& operator=(BitStreamFilter&& other);
    ~BitStreamFilter();

    void setName(std::string_view name, OptionalErrorCode ec = throws());

    void setInCodecParameters(CodecParametersView codecpar, OptionalErrorCode ec = throws());
    void setOutCodecParameters(CodecParametersView codecpar, OptionalErrorCode ec = throws());

    void init();

    bool isInitilized() const;

    CodecParametersView codecParameters() const;

    void sendPacket(Packet& packet, OptionalErrorCode ec = throws());
    void receivePacket(Packet& packet, OptionalErrorCode ec = throws());

private:
    bool m_is_initialized = false;
};

} // namespace av
