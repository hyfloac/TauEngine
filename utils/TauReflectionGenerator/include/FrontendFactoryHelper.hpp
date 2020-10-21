#pragma once

#include <clang/Tooling/Tooling.h>
#include <tuple>

namespace clang { namespace helperExt {

template<int... _Is>
struct Index { };

template<int _N, int... _Is>
struct GenSeq : GenSeq<_N - 1, _N - 1, _Is...> { };

template<int... _Is>
struct GenSeq<0, _Is...> : Index<_Is...> { };

template<typename _T, typename... _Args>
class ComplexFrontendActionFactory : public ::clang::tooling::FrontendActionFactory
{
private:
    ::std::tuple<_Args...> _args;
public:
    ComplexFrontendActionFactory(_Args&&... args) noexcept
        : _args(::std::forward<_Args>(args)...)
    { }

    ::std::unique_ptr<::clang::FrontendAction> create() override
    {
        return createForward(GenSeq<sizeof...(_Args)> { });
    }
private:
    template<int... _Is>
    ::std::unique_ptr<::clang::FrontendAction> createForward(Index<_Is...>) noexcept
    {
        return ::std::make_unique<_T>(::std::get<_Is>(_args)...);
    }
};

template<typename _T, typename... _Args>
::std::unique_ptr<::clang::tooling::FrontendActionFactory> newFrontendActionFactory(_Args&&... args)
{
    return ::std::unique_ptr<::clang::tooling::FrontendActionFactory>(new ComplexFrontendActionFactory<_T, _Args...>(::std::forward<_Args>(args)...));
}

}}
