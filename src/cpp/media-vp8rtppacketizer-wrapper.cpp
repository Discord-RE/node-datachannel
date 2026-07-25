#include "media-vp8rtppacketizer-wrapper.h"
#include "media-rtppacketizationconfig-wrapper.h"
#include "media-mediahandler-helper.h"

Napi::FunctionReference VP8RtpPacketizerWrapper::constructor = Napi::FunctionReference();

Napi::Object VP8RtpPacketizerWrapper::Init(Napi::Env env, Napi::Object exports)
{
  Napi::HandleScope scope(env);

  Napi::Function func = Napi::ObjectWrap<VP8RtpPacketizerWrapper>::DefineClass(env, "VP8RtpPacketizer",
    {
      // Instance Methods
      InstanceMethod("addToChain", &VP8RtpPacketizerWrapper::addToChain),
      // Accessors
      InstanceAccessor("rtpConfig", &VP8RtpPacketizerWrapper::getRtpPacketizationConfig, nullptr),
    });

  // If this is not the first call, we don't want to reassign the constructor (hot-reload problem)
  if (constructor.IsEmpty())
  {
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
  }

  exports.Set("VP8RtpPacketizer", func);
  return exports;
}

VP8RtpPacketizerWrapper::VP8RtpPacketizerWrapper(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<VP8RtpPacketizerWrapper>(info)
{
  Napi::Env env = info.Env();

  if (info.Length() < 2)
  {
    Napi::Error::New(env, "Expected 2 parameters").ThrowAsJavaScriptException();
    return;
  }

  if (!info[0].IsObject())
  {
    Napi::TypeError::New(env, "rtpConfig must be a RtpPacketizationConfig instance").ThrowAsJavaScriptException();
    return;
  }
  auto obj = info[0].As<Napi::Object>();
  if (!obj.InstanceOf(RtpPacketizationConfigWrapper::constructor.Value()))
  {
    Napi::TypeError::New(env, "rtpConfig must be a RtpPacketizationConfig instance").ThrowAsJavaScriptException();
    return;
  }
  // store original JS object so we can return it later
  mRtpConfigObject = Napi::Persistent(obj);
  mRtpConfigObject.SuppressDestruct();
  auto rtpConfig = RtpPacketizationConfigWrapper::Unwrap(obj)->getConfigInstance();

  size_t maxFragmentSize = rtc::RtpPacketizer::DefaultMaxFragmentSize;
  if (info.Length() >= 2)
  {
    if (!info[1].IsNumber())
    {
      Napi::TypeError::New(env, "maxFragmentSize must be a number").ThrowAsJavaScriptException();
      return;
    }
    maxFragmentSize = info[1].As<Napi::Number>().Uint32Value();
  }

  mPacketizerPtr = std::make_unique<rtc::VP8RtpPacketizer>(rtpConfig, maxFragmentSize);
}

VP8RtpPacketizerWrapper::~VP8RtpPacketizerWrapper()
{
  mPacketizerPtr.reset();
  mRtpConfigObject.Reset();
}

std::shared_ptr<rtc::VP8RtpPacketizer> VP8RtpPacketizerWrapper::getPacketizerInstance() { return mPacketizerPtr; }

void VP8RtpPacketizerWrapper::addToChain(const Napi::CallbackInfo &info)
{
  auto env = info.Env();
  if (info.Length() < 1 || !info[0].IsObject())
  {
    Napi::TypeError::New(env, "Expected a MediaHandler instance").ThrowAsJavaScriptException();
    return;
  }
  auto mediaHandler = asMediaHandler(info[0].As<Napi::Object>());
  if (!mediaHandler)
  {
    Napi::TypeError::New(env, "Expected a MediaHandler instance. If this error is unexpected, please report a bug!")
        .ThrowAsJavaScriptException();
    return;
  }
  mPacketizerPtr->addToChain(mediaHandler);
}

Napi::Value VP8RtpPacketizerWrapper::getRtpPacketizationConfig(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  if (!mPacketizerPtr)
  {
    Napi::Error::New(env, "getRtpPacketizationConfig() called on destroyed packetizer").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (mRtpConfigObject.IsEmpty())
    return env.Null();

  return mRtpConfigObject.Value();
}
