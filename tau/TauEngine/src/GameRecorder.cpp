#include "GameRecorder.hpp"

void GameRecorder::startRecording()
{
    for(const auto& pair : _handlers)
    {
        addBlip(pair.second.initialBlip(pair.second.userParam));
    }
    _recording = true;
}

void GameRecorder::stopRecording()
{
    _recording = false;
}

void GameRecorder::addBlip(const Blip& blip) noexcept
{
    _blips.push_back(blip);
}

void GameRecorder::addBlipHandler(RunTimeType<Blip> type, blipHandler_f handler, initialBlip_f initial, void* userParam)
{
    _handlers[type] = {handler, initial, userParam};
}

static RunTimeType<GameRecorder::Blip> getRTT() noexcept
{
#if TAU_RTTI_DEBUG
    static RunTimeType<GameRecorder::Blip> _rtt("GameRecorder::Blip");
#else
    static RunTimeType<GameRecorder::Blip> _rtt = RunTimeType<GameRecorder::Blip>::define();
#endif
    return _rtt;
}

void GameRecorder::addBeginUpdate() noexcept
{
    _blips.emplace_back(BlipType::Update, getRTT(), nullptr);
}

void GameRecorder::addBeginRender() noexcept
{
    _blips.emplace_back(BlipType::Render, getRTT(), nullptr);
}

void GameRecorder::beginPlayBack()
{
    _playing = true;
    for(_playbackIndex = 0; _playbackIndex < _blips.size(); ++_playbackIndex)
    {
        auto& blip = _blips[_playbackIndex];
        if(blip.type != BlipType::Initial)
        {
            break;
        }
        if(_handlers.count(blip.blipRTT) != 0)
        {
            const auto& hg = _handlers[blip.blipRTT];
            hg.blipHandler(blip, hg.userParam);
        }
    }
    if(_playbackIndex >= _blips.size())
    {
        _playing = false;
    }
}

void GameRecorder::playUpdate() noexcept
{
    if(_blips[_playbackIndex].type == BlipType::Update && _blips[_playbackIndex].blipRTT != getRTT())
    {
        return;
    }

    for(++_playbackIndex; _playbackIndex < _blips.size(); ++_playbackIndex)
    {
        auto& blip = _blips[_playbackIndex];
        if(blip.type != BlipType::Update)
        {
            break;
        }
        if(_blips[_playbackIndex].blipRTT == getRTT())
        {
            break;
        }
        if(_handlers.count(blip.blipRTT) != 0)
        {
            const auto& hg = _handlers[blip.blipRTT];
            hg.blipHandler(blip, hg.userParam);
        }
    }
    if(_playbackIndex >= _blips.size())
    {
        _playing = false;
    }
}

void GameRecorder::playRender() noexcept
{
    if(_blips[_playbackIndex].type == BlipType::Render && _blips[_playbackIndex].blipRTT != getRTT())
    {
        return;
    }

    for(++_playbackIndex; _playbackIndex < _blips.size(); ++_playbackIndex)
    {
        auto& blip = _blips[_playbackIndex];
        if(blip.type != BlipType::Render)
        {
            break;
        }
        if(_blips[_playbackIndex].blipRTT == getRTT())
        {
            break;
        }
        if(_handlers.count(blip.blipRTT) != 0)
        {
            const auto& hg = _handlers[blip.blipRTT];
            hg.blipHandler(blip, hg.userParam);
        }
    }
    if(_playbackIndex >= _blips.size())
    {
        _playing = false;
    }
}
