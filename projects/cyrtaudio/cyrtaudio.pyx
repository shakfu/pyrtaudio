# distutils: language = c++

from libcpp.string cimport string
from libcpp.vector cimport vector

cimport rtaudio as ra

cdef class RtAudio:
    cdef ra.RtAudio *ptr

    def __init__(self):
        self.ptr = new ra.RtAudio()

    @staticmethod
    def get_version():
        return ra.RtAudio.getVersion().decode()

    @staticmethod
    def get_compiled_apis():
        cdef vector[ra.RtAudio.Api] apis
        ra.RtAudio.getCompiledApi(apis)
        return apis

    @staticmethod
    def get_api_name(api_: int):
        return ra.RtAudio.getApiName(api_)

    @staticmethod
    def get_api_display_name(api_: int):
        return ra.RtAudio.getApiDisplayName(api_)

    @staticmethod
    def get_compiled_api_by_name(name: str):
        return ra.RtAudio.getCompiledApiByName(name.encode('utf8'))

    @staticmethod
    def get_compiled_api_by_display_name(name: str):
        return ra.RtAudio.getCompiledApiByDisplayName(name.encode('utf8'))

    def get_current_api(self):
        return self.ptr.getCurrentApi()

    def get_device_count(self):
        return self.ptr.getDeviceCount()

    def get_device_ids(self):
        return self.ptr.getDeviceIds()

    def get_device_names(self):
        return self.ptr.getDeviceNames()

    def get_device_info(self, device_id: int):
        cdef ra.RtAudio.DeviceInfo info
        info = self.ptr.getDeviceInfo(device_id)
        return dict(
            id=info.ID,
            name=info.name.decode(),
            output_channels=info.outputChannels,
            input_channels=info.inputChannels,
            duplex_channels=info.duplexChannels,
            is_default_output=info.isDefaultOutput,
            is_default_intput=info.isDefaultInput,
            samplerates=info.sampleRates,
            current_samplerate=info.currentSampleRate,
            preferred_samplerate=info.preferredSampleRate,
        )

    # unsigned int getDefaultOutputDevice()
    # unsigned int getDefaultInputDevice()
    # RtAudioErrorType openStream(
    #     StreamParameters *outputParameters,
    #     StreamParameters *inputParameters,
    #     RtAudioFormat format, unsigned int sampleRate,
    #     unsigned int *bufferFrames, RtAudioCallback callback,
    #     void *userData, StreamOptions *options)
    # void closeStream()
    # RtAudioErrorType startStream()
    # RtAudioErrorType stopStream()
    # RtAudioErrorType abortStream()
    # const string getErrorText()
    # bint isStreamOpen() const
    # bint isStreamRunning() const
    # double getStreamTime()
    # void setStreamTime( double time )
    # long getStreamLatency()
    # unsigned int getStreamSampleRate()
    # # void setErrorCallback( RtAudioErrorCallback errorCallback )
    # void showWarnings( bint value = true )