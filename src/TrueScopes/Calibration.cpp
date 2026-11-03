#include "Calibration.h"
#include "CalibrationMath.h"
#include "CalibrationSession.h"
#include "ScopeIdent.h"
#include "FallbackAnchorStore.h"
#include <nlohmann/json.hpp>
#include <format>
#include <cctype>
#include <algorithm>
#include <mutex>

namespace TrueScopes::Calibration
{
    namespace {
        std::mutex lock;
        Session session;
        DWORD gameThread = 0;
        std::string anchorKey;
        std::string stableForm(std::uint32_t id){
            const auto* form=RE::TESForm::GetFormByID(id);
            const auto* file=form?form->GetFile(0):nullptr;
            if(!file||id>=0xff000000)return {};
            std::string name(file->GetFilename());
            for(auto& c:name)c=static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            return std::format("{}:{:06x}",name,id&(file->IsLight()?0xfff:0xffffff));
        }
        std::string stableKey(const State& state){
            const auto weapon=stableForm(state.weapon);if(weapon.empty())return {};
            std::vector<std::string> mods;
            for(unsigned i=0;i<state.modCount;++i){auto mod=stableForm(state.mods[i]);if(mod.empty())return {};mods.push_back(mod);}
            std::sort(mods.begin(),mods.end());
            return nlohmann::json::array({weapon,"fallback",mods}).dump();
        }

        std::uint32_t __cdecl Read(State* out)
        {
            if (!out || out->bytes != sizeof(State)) return 0;
            const auto tid = ::GetCurrentThreadId();
            if (!gameThread) gameThread = tid;
            if (gameThread != tid) return 0;
            FallbackAnchor::service();
            std::uint32_t weapon = 0;
            char optic[128]{};
            State next{};
            next.probe = ScopeIdent::ProbeCount();
            if (ScopeIdent::CurrentScopeKey(weapon, optic)) {
                const auto info = ScopeIdent::Get();
                // Overflow cannot safely identify a receiver configuration. Do not guess.
                if (info.probed && !info.faulted && !info.modOverflow && info.weaponFormID == weapon) {
                    next.ready = 1; next.weapon = weapon;
                    std::memcpy(next.optic, optic, sizeof(next.optic));
                    next.modCount = (std::min)(info.modCount, 24u);
                    for (std::uint32_t i = 0; i < next.modCount; ++i) next.mods[i] = info.mods[i].formID;
                }
            }
            const std::scoped_lock guard(lock);
            const auto previous=session.state().epoch;
            session.observe(next);
            *out = session.state();
            if(out->epoch!=previous || (!out->ready && !anchorKey.empty()))
                anchorKey=out->ready && std::strcmp(out->optic,"fallback")==0?stableKey(*out):std::string{};
            return 1;
        }
        std::uint32_t __cdecl Set(std::uint64_t epoch, const Values* values)
        {
            if (!values || !Valid(*values) || gameThread != ::GetCurrentThreadId()) return 0;
            State fresh{};
            if (!Read(&fresh) || !fresh.ready || fresh.epoch != epoch) return 0;
            const std::scoped_lock guard(lock);
            const bool accepted = session.set(epoch, *values);
            if (accepted) logger::info(FMT_STRING("LIVE CALIBRATION epoch {}: right={:.3f} forward={:.3f} up={:.3f} size={:.3f} pitch={:.3f} yaw={:.3f}"),
                epoch, values->right, values->forward, values->up, values->size, values->pitch, values->yaw);
            return accepted ? 1u : 0u;
        }
        const API api{sizeof(API), kVersion, Read, Set};
    }
    bool ForRender(std::uint32_t weapon, const char* optic, std::uint32_t probe, Values& out)
    {
        const std::scoped_lock guard(lock);
        return session.render(weapon, optic, probe, out);
    }
    bool FallbackContext(std::uint32_t weapon,const char* optic,std::uint32_t probe,Values&out,std::string& key){
        const std::scoped_lock guard(lock);const auto state=session.state();
        if(!state.ready||state.weapon!=weapon||state.probe!=probe||!optic||
           std::strcmp(optic,"fallback")!=0||std::strcmp(optic,state.optic)!=0||anchorKey.empty())return false;
        out=state.values;key=anchorKey;return true;
    }
}

extern "C" __declspec(dllexport) const TrueScopes::Calibration::API* __cdecl
TrueScopes_GetCalibrationAPI(std::uint32_t version, std::uint32_t bytes)
{
    using namespace TrueScopes::Calibration;
    return version == kVersion && bytes == sizeof(API) ? &api : nullptr;
}
