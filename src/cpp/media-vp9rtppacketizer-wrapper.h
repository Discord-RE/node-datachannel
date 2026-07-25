#ifndef MEDIA_VP9RTPPACKETIZER_WRAPPER_H
#define MEDIA_VP9RTPPACKETIZER_WRAPPER_H

#include <memory>
#include <unordered_set>

#include <napi.h>
#include <rtc/rtc.hpp>

class VP9RtpPacketizerWrapper : public Napi::ObjectWrap<VP9RtpPacketizerWrapper>
{
public:
  static Napi::FunctionReference constructor;
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  VP9RtpPacketizerWrapper(const Napi::CallbackInfo &info);
  ~VP9RtpPacketizerWrapper();
  std::shared_ptr<rtc::VP9RtpPacketizer> getPacketizerInstance();

  // Functions
  Napi::Value getRtpPacketizationConfig(const Napi::CallbackInfo &info);
  void addToChain(const Napi::CallbackInfo &info);

  // Callbacks

private:
  std::shared_ptr<rtc::VP9RtpPacketizer> mPacketizerPtr = nullptr;
  Napi::ObjectReference mRtpConfigObject;
};

#endif // MEDIA_VP9RTPPACKETIZER_WRAPPER_H
