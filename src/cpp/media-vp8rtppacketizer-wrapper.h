#ifndef MEDIA_VP8RTPPACKETIZER_WRAPPER_H
#define MEDIA_VP8RTPPACKETIZER_WRAPPER_H

#include <memory>
#include <unordered_set>

#include <napi.h>
#include <rtc/rtc.hpp>

class VP8RtpPacketizerWrapper : public Napi::ObjectWrap<VP8RtpPacketizerWrapper>
{
public:
  static Napi::FunctionReference constructor;
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  VP8RtpPacketizerWrapper(const Napi::CallbackInfo &info);
  ~VP8RtpPacketizerWrapper();
  std::shared_ptr<rtc::VP8RtpPacketizer> getPacketizerInstance();

  // Functions
  Napi::Value getRtpPacketizationConfig(const Napi::CallbackInfo &info);
  void addToChain(const Napi::CallbackInfo &info);

  // Callbacks

private:
  std::shared_ptr<rtc::VP8RtpPacketizer> mPacketizerPtr = nullptr;
  Napi::ObjectReference mRtpConfigObject;
};

#endif // MEDIA_VP8RTPPACKETIZER_WRAPPER_H
