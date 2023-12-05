from libcpp.string cimport string
from libcpp.vector cimport vector

cdef extern from "rtaudio/RtAudio.h":

    ctypedef unsigned long RtAudioFormat
    cdef RtAudioFormat RTAUDIO_SINT8 = 0x1
    cdef RtAudioFormat RTAUDIO_SINT16 = 0x2
    cdef RtAudioFormat RTAUDIO_SINT24 = 0x4
    cdef RtAudioFormat RTAUDIO_SINT32 = 0x8
    cdef RtAudioFormat RTAUDIO_FLOAT32 = 0x10
    cdef RtAudioFormat RTAUDIO_FLOAT64 = 0x20


    ctypedef unsigned int RtAudioStreamFlags
    cdef RtAudioStreamFlags RTAUDIO_NONINTERLEAVED = 0x1
    cdef RtAudioStreamFlags RTAUDIO_MINIMIZE_LATENCY = 0x2
    cdef RtAudioStreamFlags RTAUDIO_HOG_DEVICE = 0x4
    cdef RtAudioStreamFlags RTAUDIO_SCHEDULE_REALTIME = 0x8
    cdef RtAudioStreamFlags RTAUDIO_ALSA_USE_DEFAULT = 0x10
    cdef RtAudioStreamFlags RTAUDIO_JACK_DONT_CONNECT = 0x20

    ctypedef unsigned int RtAudioStreamStatus
    cdef RtAudioStreamStatus RTAUDIO_INPUT_OVERFLOW = 0x1
    cdef RtAudioStreamStatus RTAUDIO_OUTPUT_UNDERFLOW = 0x2

    ctypedef int (*RtAudioCallback)( void *outputBuffer, void *inputBuffer,
                                    unsigned int nFrames,
                                    double streamTime,
                                    RtAudioStreamStatus status,
                                    void *userData )

    ctypedef enum RtAudioErrorType:
        RTAUDIO_NO_ERROR = 0
        RTAUDIO_WARNING
        RTAUDIO_UNKNOWN_ERROR
        RTAUDIO_NO_DEVICES_FOUND
        RTAUDIO_INVALID_DEVICE
        RTAUDIO_DEVICE_DISCONNECT
        RTAUDIO_MEMORY_ERROR
        RTAUDIO_INVALID_PARAMETER
        RTAUDIO_INVALID_USE
        RTAUDIO_DRIVER_ERROR
        RTAUDIO_SYSTEM_ERROR
        RTAUDIO_THREAD_ERROR

    # ctypedef std::function[void(RtAudioErrorType type, const string &errorText)] RtAudioErrorCallback

    cdef cppclass RtApi

    cdef cppclass RtAudio

    cdef cppclass RtAudio:

        ctypedef enum Api:
            UNSPECIFIED
            MACOSX_CORE
            LINUX_ALSA
            UNIX_JACK
            LINUX_PULSE
            LINUX_OSS
            WINDOWS_ASIO
            WINDOWS_WASAPI
            WINDOWS_DS
            RTAUDIO_DUMMY
            NUM_APIS

        struct DeviceInfo:
            unsigned int ID
            string name
            unsigned int outputChannels
            unsigned int inputChannels
            unsigned int duplexChannels
            bint isDefaultOutput
            bint isDefaultInput
            vector[unsigned int] sampleRates
            unsigned int currentSampleRate
            unsigned int preferredSampleRate
            RtAudioFormat nativeFormats
  
        struct StreamParameters:
            unsigned int deviceId
            unsigned int nChannels
            unsigned int firstChannel

        struct StreamOptions:
            RtAudioStreamFlags flags
            unsigned int numberOfBuffers
            string streamName
            int priority

        @staticmethod
        string getVersion()

        @staticmethod       
        void getCompiledApi( vector[Api] &apis )

        @staticmethod
        string getApiName( Api api_ )

        @staticmethod
        string getApiDisplayName( Api api_ )

        @staticmethod
        Api getCompiledApiByName( const string &name )

        @staticmethod
        Api getCompiledApiByDisplayName( const string &name )

        # RtAudio( Api api=UNSPECIFIED, RtAudioErrorCallback&& errorCallback=0 )
        RtAudio() except +
    #   ~RtAudio()
        Api getCurrentApi()
        unsigned int getDeviceCount()
        vector[unsigned int] getDeviceIds()
        vector[string] getDeviceNames()
        DeviceInfo getDeviceInfo( unsigned int deviceId )
        unsigned int getDefaultOutputDevice()
        unsigned int getDefaultInputDevice()
        RtAudioErrorType openStream(
            StreamParameters *outputParameters,
            StreamParameters *inputParameters,
            RtAudioFormat format, unsigned int sampleRate,
            unsigned int *bufferFrames, RtAudioCallback callback,
            void *userData, StreamOptions *options)
        void closeStream()
        RtAudioErrorType startStream()
        RtAudioErrorType stopStream()
        RtAudioErrorType abortStream()
        const string getErrorText()
        bint isStreamOpen() const
        bint isStreamRunning() const
        double getStreamTime()
        void setStreamTime( double time )
        long getStreamLatency()
        unsigned int getStreamSampleRate()
        # void setErrorCallback( RtAudioErrorCallback errorCallback )
        void showWarnings(bint value)

