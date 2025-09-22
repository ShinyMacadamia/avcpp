#include "bitstreamfilter.h"
#include "averror.h"
#include "libavcodec/bsf.h"
#include "packet.h"

namespace av {
// Move constructor
BitStreamFilter::BitStreamFilter(BitStreamFilter&& other)
{
    m_raw = other.m_raw;
    m_is_initialized = other.m_is_initialized;
    other.reset();
    other.m_is_initialized = false;
}

// Free swap for lvalues (needed so std::swap(*this, other) works)
void swap(BitStreamFilter& lhs, BitStreamFilter& rhs) noexcept
{
    std::swap(lhs.m_raw, rhs.m_raw);
    std::swap(lhs.m_is_initialized, rhs.m_is_initialized);
}

BitStreamFilter& BitStreamFilter::operator=(BitStreamFilter&& other)
{
    BitStreamFilter temp(std::move(other));
    std::swap(*this, temp);
    return *this;
}

BitStreamFilter::~BitStreamFilter()
{
    if (m_raw) {
        av_bsf_free(&m_raw);
    }
}

void BitStreamFilter::setName(std::string_view name, OptionalErrorCode ec)
{
    const AVBitStreamFilter* filter = av_bsf_get_by_name(name.data());
    if (!filter) {
        throws_if(ec, Errors::BitStreamFilterNotFound);
        return;
    }
    if (av_bsf_alloc(filter, &m_raw) != 0) {
        throws_if(ec, Errors::BitStreamAllocFail);
        return;
    }
}

void BitStreamFilter::setInCodecParameters(CodecParametersView codecpar, OptionalErrorCode ec)
{
    if (m_raw && m_raw->par_in) {
        codecpar.copyTo(m_raw->par_in, ec);
    }
}

void BitStreamFilter::setOutCodecParameters(CodecParametersView codecpar, OptionalErrorCode ec)
{
    if (m_raw && m_raw->par_in) {
        codecpar.copyTo(m_raw->par_in, ec);
    }
}

void BitStreamFilter::init(OptionalErrorCode ec)
{
    if (auto ret = av_bsf_init(m_raw); ret != 0) {
        throws_if(ec, ret, ffmpeg_category());
        return;
    }
    m_is_initialized = true;
}

bool BitStreamFilter::isInitilized() const
{
    return m_is_initialized;
}

CodecParametersView BitStreamFilter::codecParameters() const
{
    return m_raw ? m_raw->par_out : nullptr;
}

void BitStreamFilter::sendPacket(Packet& packet, OptionalErrorCode ec)
{
    if (!isInitilized()) {
        throws_if(ec, Errors::BitStreamAllocFail);
        return;
    }
    if (auto ret = av_bsf_send_packet(m_raw, packet.isNull() ? nullptr : packet.raw()); ret != 0) {
        throws_if(ec, ret, ffmpeg_category());
        return;
    }
}

Packet BitStreamFilter::receivePacket(OptionalErrorCode ec)
{
    Packet packet;
    if (!isInitilized()) {
        throws_if(ec, Errors::BitStreamAllocFail);
        return packet;
    }
    if (auto ret = av_bsf_receive_packet(m_raw, packet.isNull() ? nullptr : packet.raw()); ret != 0) {
        throws_if(ec, ret, ffmpeg_category());
        return packet;
    }
    packet.setComplete(true);
    return packet;
}
} // namespace av
