#pragma once

#include <vector>
#include <unordered_map>
#include <RunTimeType.hpp>

class GameRecorder final
{
public:
    enum class BlipType
    {
        Initial,
        Update,
        Render,
        Event
    };

    struct Blip final
    {
        BlipType type;
        RunTimeType<Blip> blipRTT;
        void* data;

        Blip(const BlipType type, const RunTimeType<Blip>& blipRtt, void* const data)
            : type(type),
              blipRTT(blipRtt),
              data(data)
        {
        }
    };

    typedef bool(*__cdecl blipHandler_f)(Blip& blip, void* userParam);
    typedef Blip(*__cdecl initialBlip_f)(void* userParam);
    struct HandlerGroup final
    {
        blipHandler_f blipHandler;
        initialBlip_f initialBlip;
        void* userParam;
    };
private:
    std::vector<Blip> _blips;
    std::unordered_map<RunTimeType<Blip>, HandlerGroup> _handlers;
    bool _recording;
    bool _playing;
    std::size_t _playbackIndex;
public:
    GameRecorder() noexcept
        : _recording(false), _playing(false), _playbackIndex(0)
    { }

    [[nodiscard]] bool recording() const noexcept { return _recording; }
    [[nodiscard]] bool playing() const noexcept { return _playing; }

    void startRecording();

    void stopRecording();

    void addBlip(const Blip& blip) noexcept;

    void addBlipHandler(RunTimeType<Blip> type, blipHandler_f handler, initialBlip_f initial, void* userParam);

    void addBeginUpdate() noexcept;
    void addBeginRender() noexcept;

    void beginPlayBack();

    void playUpdate() noexcept;
    void playRender() noexcept;
};
