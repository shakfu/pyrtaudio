#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// rtaudio
#include "RtAudio.h" 

namespace py = pybind11;


PYBIND11_MODULE(pyrtaudio, m)
{
    m.doc() = "pyrtaudio: a pybind11 wrapper around the rtaudio library.";
    m.attr("__version__") = "0.0.1";

    // -----------------------------------------------------------------------
    // librtaudio

    py::enum_<RtAudioErrorType>(m, "RtAudioErrorType")
        .value("RTAUDIO_NO_ERROR",          RtAudioErrorType::RTAUDIO_NO_ERROR,           "No error")
        .value("RTAUDIO_WARNING",           RtAudioErrorType::RTAUDIO_WARNING,            "A non-critical error")
        .value("RTAUDIO_UNKNOWN_ERROR",     RtAudioErrorType::RTAUDIO_UNKNOWN_ERROR,      "An unspecified error type.")
        .value("RTAUDIO_NO_DEVICES_FOUND",  RtAudioErrorType::RTAUDIO_NO_DEVICES_FOUND,   "No devices found on system")
        .value("RTAUDIO_INVALID_DEVICE",    RtAudioErrorType::RTAUDIO_INVALID_DEVICE,     "An invalid device ID was specified")
        .value("RTAUDIO_DEVICE_DISCONNECT", RtAudioErrorType::RTAUDIO_DEVICE_DISCONNECT,  "A device in use was disconnected")
        .value("RTAUDIO_MEMORY_ERROR",      RtAudioErrorType::RTAUDIO_MEMORY_ERROR,       "An error occurred during memory allocation.")
        .value("RTAUDIO_INVALID_PARAMETER", RtAudioErrorType::RTAUDIO_INVALID_PARAMETER,  "Tn invalid parameter was specified to a function.")
        .value("RTAUDIO_INVALID_USE",       RtAudioErrorType::RTAUDIO_INVALID_USE,        "The function was called incorrectly.")    
        .value("RTAUDIO_DRIVER_ERROR",      RtAudioErrorType::RTAUDIO_DRIVER_ERROR,       "A system driver error occurred.")
        .value("RTAUDIO_SYSTEM_ERROR",      RtAudioErrorType::RTAUDIO_SYSTEM_ERROR,       "A system error occurred.")
        .value("RTAUDIO_THREAD_ERROR",      RtAudioErrorType::RTAUDIO_THREAD_ERROR,       "A thread error occurred.")
        .export_values();

    py::class_<RtApi>(m, "RtApi");

    py::class_<RtAudio> _rta(m, "RtAudio");

    py::enum_<RtAudio::Api>(_rta, "Api")
        .value("UNSPECIFIED",   RtAudio::Api::UNSPECIFIED,    "Search for a working compiled API")
        .value("MACOSX_CORE",   RtAudio::Api::MACOSX_CORE,    "Macintosh OS-X Core Audio API")
        .value("LINUX_ALSA",    RtAudio::Api::LINUX_ALSA,     "The Advanced Linux Sound Architecture API")
        .value("UNIX_JACK",     RtAudio::Api::UNIX_JACK,      "The Jack Low-Latency Audio Server API")
        .value("LINUX_PULSE",   RtAudio::Api::LINUX_PULSE,    "The Linux PulseAudio API")
        .value("LINUX_OSS",     RtAudio::Api::LINUX_OSS,      "The Linux Open Sound System API")
        .value("WINDOWS_ASIO",  RtAudio::Api::WINDOWS_ASIO,   "The Steinberg Audio Stream I/O API")
        .value("WINDOWS_WASAPI", RtAudio::Api::WINDOWS_WASAPI, "The Microsoft WASAPI API")
        .value("WINDOWS_DS",    RtAudio::Api::WINDOWS_DS,     "The Microsoft DirectSound API")
        .value("RTAUDIO_DUMMY", RtAudio::Api::RTAUDIO_DUMMY,  "A compilable but non-functional API")
        .value("NUM_APIS",      RtAudio::Api::NUM_APIS,       "Number of values in this enum")
        .export_values();

    py::class_<RtAudio::DeviceInfo>(_rta, "DeviceInfo")
        .def(py::init<>())
        .def_readwrite("id", &RtAudio::DeviceInfo::ID)
        .def_readwrite("name", &RtAudio::DeviceInfo::name)
        .def_readwrite("output_channels", &RtAudio::DeviceInfo::outputChannels)
        .def_readwrite("input_channels", &RtAudio::DeviceInfo::inputChannels)
        .def_readwrite("duplex_channels", &RtAudio::DeviceInfo::duplexChannels)
        .def_readwrite("is_default_output", &RtAudio::DeviceInfo::isDefaultOutput)
        .def_readwrite("is_default_intput", &RtAudio::DeviceInfo::isDefaultInput)
        .def_readwrite("samplerates", &RtAudio::DeviceInfo::sampleRates)
        .def_readwrite("current_samplerate", &RtAudio::DeviceInfo::currentSampleRate)
        .def_readwrite("preferred_samplerate", &RtAudio::DeviceInfo::preferredSampleRate)
        .def_readwrite("native_formats", &RtAudio::DeviceInfo::nativeFormats)
        ;

    py::class_<RtAudio::StreamParameters>(_rta, "StreamParameters")
        .def(py::init<>())
        .def_readwrite("device_id", &RtAudio::StreamParameters::deviceId)
        .def_readwrite("nchannels", &RtAudio::StreamParameters::nChannels)
        .def_readwrite("first_channel", &RtAudio::StreamParameters::firstChannel)
        ;

    py::class_<RtAudio::StreamOptions>(_rta, "StreamOptions")
        .def(py::init<>())
        .def_readwrite("flags", &RtAudio::StreamOptions::flags)
        .def_readwrite("stream_name", &RtAudio::StreamOptions::streamName)
        .def_readwrite("priority", &RtAudio::StreamOptions::priority)
        ;

    _rta.def_static("get_version", &RtAudio::getVersion, "Return the current RtAudio version.");
    _rta.def_static("get_compiled_apis", []() {
        std::vector<RtAudio::Api> apis;
        RtAudio::getCompiledApi(apis);
        return apis;
    }, "Get the available compiled audio APIs.");

    _rta.def_static("get_api_name", &RtAudio::getApiName, "Return the name of a specified compiled audio API.");
    _rta.def_static("get_api_display_name", &RtAudio::getApiDisplayName, "Return the display name of a specified compiled audio API.");
    _rta.def_static("get_compiled_api_by_name", &RtAudio::getCompiledApiByName, "Return the compiled audio API having the given name.");
    _rta.def_static("get_compiled_api_by_display_name", &RtAudio::getCompiledApiByDisplayName, "Return the compiled audio API having the given display name.");

    // constructor
    _rta.def(py::init<RtAudio::Api>(), py::arg("api") = RtAudio::Api::UNSPECIFIED);
    // _rta.def(py::init<RtAudio::Api, RtAudioErrorCallback>(), py::arg("api") = RtAudio::Api::UNSPECIFIED, py::arg("errorCallback") = 0);

    _rta.def("get_current_api", &RtAudio::getCurrentApi, "Returns the audio API specifier for the current instance of RtAudio");
    _rta.def("get_device_count", &RtAudio::getDeviceCount, "Returns the number of audio devices available");
    _rta.def("get_device_ids", &RtAudio::getDeviceIds, "Returns a list of audio device ids");
    _rta.def("get_device_names", &RtAudio::getDeviceNames, "Returns a list of audio device names");
    _rta.def("get_device_info", &RtAudio::getDeviceInfo, "Returns a DeviceInfo instance for the given device id.");
    _rta.def("open_stream", &RtAudio::openStream, "Open a stream with the specified parameters");
    _rta.def("get_default_output_device", &RtAudio::getDefaultOutputDevice, "Returns the ID of the default output device.");
    _rta.def("get_default_intput_device", &RtAudio::getDefaultInputDevice, "Returns the ID of the default input device.");
    _rta.def("close_stream", &RtAudio::closeStream, "Closes a stream and frees any associated stream memory.");
    _rta.def("start_stream", &RtAudio::startStream, "Starts a stream.");
    _rta.def("stop_stream", &RtAudio::stopStream, "Stop a stream, allowing any samples remaining in the output queue to be played.");
    _rta.def("abort_stream", &RtAudio::abortStream, "Stop a stream, discarding any samples remaining in the input/output queue.");
    _rta.def("get_error_text", &RtAudio::getErrorText, "Retrieve the error message corresponding to the last error or warning condition.");
    _rta.def("is_stream_open", &RtAudio::isStreamOpen, "Returns true if a stream is open and false if not.");
    _rta.def("is_stream_running", &RtAudio::isStreamRunning, "Returns true if the stream is running and false if it is stopped or not open.");
    _rta.def("get_stream_time", &RtAudio::getStreamTime, "Returns the number of seconds of processed data since the stream was started.");
    _rta.def("set_stream_time", &RtAudio::setStreamTime, "Set the stream time to a time in seconds greater than or equal to 0.0.");
    _rta.def("get_stream_latency", &RtAudio::getStreamLatency, "Returns the internal stream latency in sample frames.");
    _rta.def("get_stream_samplerate", &RtAudio::getStreamSampleRate, "Returns actual sample rate in use by the (open) stream.");
    _rta.def("set_error_callback", &RtAudio::setErrorCallback, "Set a client-defined function that will be invoked when an error or warning occurs.");
    _rta.def("show_warnings", &RtAudio::showWarnings, "Specify whether warning messages should be output or not.");


}