/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "calendar_manager_napi.h"
#include <optional>

using namespace OHOS::AppExecFwk;
using namespace OHOS::DataShare;
static const int INVALID_EVENT_ID = -1;
static const int ARGS_INDEX_ONE = 1;
static const int ARGS_INDEX_TWO = 2;
static const int ARGS_INDEX_THREE = 3;

namespace {
    const std::string CALENDAR_MANAGER_CLASS_NAME = "CalendarManager";
    static thread_local napi_ref g_constructorRef = nullptr;
    constexpr uint32_t INITIAL_REFCOUNT = 1;
}
namespace OHOS::CalendarApi {
napi_value CalendarManagerNapi::CreateCalendar(napi_env env, napi_callback_info info)
{
    LOG_INFO("napi CreateCalendar called");
    struct CreateCalendarContext : public ContextBase {
        CalendarAccount account;
        CalendarNapi *calendar;
        int id;
        napi_ref ref = nullptr;
    };
    auto ctxt = std::make_shared<CreateCalendarContext>();
    auto input = [env, ctxt](size_t argc, napi_value* argv) {
        // required 1 arguments :: <CalendarAccount>
        CHECK_ARGS_RETURN_VOID(ctxt, argc == 1, "invalid arguments!");
        NapiUtil::GetValue(env, argv[0], ctxt->account);
        CHECK_STATUS_RETURN_VOID(ctxt, "invalid arg[0], i.e. invalid keys!");
        ctxt->ref = NapiUtil::NewWithRef(env, argc, argv, reinterpret_cast<void**>(&ctxt->calendar),
            CalendarNapi::Constructor(env));
    };
    ctxt->GetCbInfo(env, info, input);

    auto execute = [ctxt]() {
        auto nativteCalendar = Native::CalendarManager::GetInstance().CreateCalendar(ctxt->account);
        ctxt->status = (nativteCalendar != nullptr) ? napi_ok : napi_generic_failure;
        CHECK_STATUS_RETURN_VOID(ctxt, "CreateCalendar failed!");
        ctxt->calendar->SetNative(nativteCalendar);
        ctxt->id = nativteCalendar->GetId();
    };
    auto output = [env, ctxt](napi_value& result) {
        ctxt->status = napi_get_reference_value(env, ctxt->ref, &result);
        CHECK_STATUS_RETURN_VOID(ctxt, "CreateCalendar output get ref value failed");
        ctxt->status = NapiUtil::SetNamedProperty(env, "id", ctxt->id, result);
        CHECK_STATUS_RETURN_VOID(ctxt, "CreateCalendar SetNamedProperty id failed");
        ctxt->status = napi_delete_reference(env, ctxt->ref);
        CHECK_STATUS_RETURN_VOID(ctxt, "CreateCalendar output del ref failed");
    };
    return NapiQueue::AsyncWork(env, ctxt, std::string(__FUNCTION__), execute, output);
}

napi_value CalendarManagerNapi::DeleteCalendar(napi_env env, napi_callback_info info)
{
    LOG_INFO("napi DeleteCalendar called");
    struct DelCalendarContext : public ContextBase {
        CalendarAccount account;
        CalendarNapi *calendar;
        bool delResult = false;
    };
    auto ctxt = std::make_shared<DelCalendarContext>();
    auto input = [env, ctxt](size_t argc, napi_value* argv) {
        // required 1 arguments :: <Calendar>
        CHECK_ARGS_RETURN_VOID(ctxt, argc == 1, "invalid arguments!");
        napi_valuetype type = napi_undefined;
        napi_typeof(env, argv[0], &type);
        CHECK_ARGS_RETURN_VOID(ctxt, type == napi_object, "type error!");
        ctxt->status = CalendarNapi::ToJson(env, argv[0], ctxt->calendar);
        CHECK_STATUS_RETURN_VOID(ctxt, "invalid arg[0], i.e. invalid keys!");
    };
    ctxt->GetCbInfo(env, info, input);

    auto execute = [ctxt]() {
        CHECK_RETURN_VOID(ctxt->calendar, "calendar is nullptr");
        auto nativeCalendar = ctxt->calendar->GetNative();
        CHECK_RETURN_VOID(nativeCalendar, "calendar is nullptr");
        ctxt->delResult = Native::CalendarManager::GetInstance()
            .DeleteCalendar(*(nativeCalendar.get()));
        CHECK_RETURN_VOID(ctxt->delResult, "DeleteCalendar failed!");
    };
    auto output = [env, ctxt](napi_value& result) {
        NapiUtil::SetValue(env, ctxt->delResult, result);
        CHECK_STATUS_RETURN_VOID(ctxt, "output del ref failed");
    };
    return NapiQueue::AsyncWork(env, ctxt, std::string(__FUNCTION__), execute, output);
}

struct GetCalendarContext : public ContextBase {
    std::optional<CalendarAccount> account;
    CalendarNapi *calendar = nullptr;
    napi_ref ref = nullptr;
    int id = -1;

    void GetCbInfo(napi_env env, napi_callback_info info)
    {
        auto input = [env, this](size_t argc, napi_value* argv) {
            // required at least 1 arguments :: <CalendarAccount>
            CHECK_ARGS_RETURN_VOID(this, argc <= 1, "invalid arguments!");
            if (argc == 0) {
                this->account = std::nullopt;
            } else {
                CalendarAccount tmpAccount;
                NapiUtil::GetValue(env, argv[0], tmpAccount);
                this->account = tmpAccount;
            }
            CHECK_STATUS_RETURN_VOID(this, "invalid arg[0], i.e. invalid keys!");
            ref = NapiUtil::NewWithRef(env, argc, argv, reinterpret_cast<void**>(&calendar),
                CalendarNapi::Constructor(env));
        };
        LOG_DEBUG("call ContextBase::GetCbInfo");
        ContextBase::GetCbInfo(env, info, input);
    }
};

napi_value CalendarManagerNapi::GetCalendar(napi_env env, napi_callback_info info)
{
    LOG_DEBUG("GetCalendar in");
    auto ctxt = std::make_shared<GetCalendarContext>();
    ctxt->GetCbInfo(env, info);

    auto execute = [ctxt]() {
        auto nativteCalendar = Native::CalendarManager::GetInstance().GetCalendar(ctxt->account);
        ctxt->status = (nativteCalendar != nullptr) ? napi_ok : napi_generic_failure;
        CHECK_STATUS_RETURN_VOID(ctxt, "GetCalendar error!");
        if (nativteCalendar->GetId() == -1) {
            ctxt->status = napi_generic_failure;
            CHECK_STATUS_RETURN_VOID(ctxt, "GetCalendar failed!");
        }
        if (ctxt->calendar != nullptr) {
            ctxt->calendar->SetNative(nativteCalendar);
        }
        ctxt->id = nativteCalendar->GetId();
    };
    auto output = [env, ctxt](napi_value& result) {
        ctxt->status = napi_get_reference_value(env, ctxt->ref, &result);
        CHECK_STATUS_RETURN_VOID(ctxt, "GetCalendar output get ref value failed");
        ctxt->status = NapiUtil::SetNamedProperty(env, "id", ctxt->id, result);
        CHECK_STATUS_RETURN_VOID(ctxt, "GetCalendar SetNamedProperty id failed");
        ctxt->status = napi_delete_reference(env, ctxt->ref);
        CHECK_STATUS_RETURN_VOID(ctxt, "GetCalendar output del ref failed");
    };
    return NapiQueue::AsyncWork(env, ctxt, std::string(__FUNCTION__), execute, output);
}

napi_value CalendarManagerNapi::GetAllCalendars(napi_env env, napi_callback_info info)
{
    LOG_DEBUG("napi GetAllCalendars called");
    struct GetAllCalendarContext : public ContextBase {
        napi_callback_info info;
        std::vector<napi_ref> refs;
    };

    auto ctxt = std::make_shared<GetAllCalendarContext>();
    auto input = [env, ctxt](size_t argc, napi_value* argv) {
        CHECK_ARGS_RETURN_VOID(ctxt, argc == 0, "invalid arguments!");
        auto nativteCalendars = Native::CalendarManager::GetInstance().GetAllCalendars();
        for (auto &calendar : nativteCalendars) {
            CalendarNapi *calendarNapi = nullptr;
            auto ref = NapiUtil::NewWithRef(env, argc, argv, reinterpret_cast<void**>(&calendarNapi),
                CalendarNapi::Constructor(env));
            CHECK_RETURN_VOID(calendarNapi != nullptr, "new CalendarNapi failed!");
            calendarNapi->SetNative(calendar);
            napi_value value;
            ctxt->status = napi_get_reference_value(env, ref, &value);
            CHECK_STATUS_RETURN_VOID(ctxt, "napi_get_reference_value failed");
            ctxt->status = NapiUtil::SetNamedProperty(env, "id", calendar->GetId(), value);
            CHECK_STATUS_RETURN_VOID(ctxt, "SetNamedProperty id failed");
            ctxt->refs.emplace_back(ref);
        }
    };
    ctxt->GetCbInfo(env, info, input);

    auto execute = [env, ctxt]()->void {
    };

    auto output = [env, ctxt](napi_value& result) {
        ctxt->status = napi_create_array_with_length(env, ctxt->refs.size(), &result);
        CHECK_STATUS_RETURN_VOID(ctxt, "create array failed!");
        int index = 0;
        for (auto& ref : ctxt->refs) {
            napi_value value;
            ctxt->status = napi_get_reference_value(env, ref, &value);
            CHECK_STATUS_RETURN_VOID(ctxt, "get ref value failed!");
            ctxt->status = napi_set_element(env, result, index++, value);
            CHECK_STATUS_RETURN_VOID(ctxt, "napi_set_element failed!");
            ctxt->status = napi_delete_reference(env, ref);
            CHECK_STATUS_RETURN_VOID(ctxt, "napi_delete_reference failed!");
        }
    };
    return NapiQueue::AsyncWork(env, ctxt, std::string(__FUNCTION__), execute, output);
}

napi_value GetCalendarManager(napi_env env, napi_callback_info info)
{
    LOG_INFO("napi GetCalendarManager called");
    const int argsOne = 1;
    napi_value result = nullptr;
    napi_value cons = nullptr;
    size_t requireArgc = argsOne;
    size_t argc = argsOne;
    napi_value args[argsOne] = {nullptr};
    if (napi_get_cb_info(env, info, &argc, args, nullptr, nullptr) != napi_ok) {
        return nullptr;
    }

    if (argc > requireArgc || napi_get_reference_value(env, g_constructorRef, &cons) != napi_ok) {
        return nullptr;
    }

    if (napi_new_instance(env, cons, argsOne, args, &result) != napi_ok) {
        return nullptr;
    }
    CalendarManagerNapi *calendarManager = nullptr;
    if (napi_unwrap(env, result, (void **)&calendarManager) != napi_ok) {
        LOG_ERROR("Faild to get fileAccessHelper");
        return nullptr;
    }

    if (calendarManager == nullptr) {
        LOG_ERROR("fileAccessHelper is nullptr");
        return nullptr;
    }
    return result;
}

napi_value CalendarManagerNapi::EditEvent(napi_env env, napi_callback_info info)
{
    LOG_INFO("editEvent called");
    napi_value eventId = nullptr;
    NapiUtil::SetValue(env, INVALID_EVENT_ID, eventId);
    auto ctxt = std::make_shared<EditEventContext>();
    auto input = [env, ctxt](size_t argc, napi_value* argv) {
        CHECK_ARGS_RETURN_VOID(ctxt, argc == ARGS_INDEX_THREE, "invalid arguments!");
        ctxt-> _jsContext = argv[0];
        NapiUtil::GetValue(env, argv[ARGS_INDEX_ONE], ctxt->event);
        NapiUtil::GetValue(env, argv[ARGS_INDEX_TWO], ctxt->caller);
    };
    ctxt->GetCbInfo(env, info, input, true);
    
    bool isStageMode = false;
    auto jsContext = ctxt->_jsContext;
    auto status = OHOS::AbilityRuntime::IsStageContext(env, jsContext, isStageMode);
    if (status != napi_ok || !isStageMode) {
        LOG_ERROR("editEvent No support FA Model");
        return eventId;
    }
    auto stageContext = OHOS::AbilityRuntime::GetStageModeContext(env, jsContext);
    if (stageContext == nullptr) {
        LOG_ERROR("editEvent stageContext == nullptr.");
        return eventId;
    }
    auto abilityContext = OHOS::AbilityRuntime::Context::ConvertTo<AbilityRuntime::AbilityContext>(stageContext);
    if (abilityContext == nullptr) {
        LOG_ERROR("editEvent only support for UIAbility Context.");
        return eventId;
    }
    ctxt->_uiContent = abilityContext->GetUIContent();
    return LaunchEditorPage(env, ctxt);
}

napi_value CalendarManagerNapi::LaunchEditorPage(napi_env env, std::shared_ptr<EditEventContext> ctxt)
{
    napi_value promise = nullptr;
    napi_deferred deferred = nullptr;
    napi_create_promise(env, &deferred, &promise);
    AAFwk::Want want;
    want.SetElementName("com.ohos.calendardata", "EditorUIExtensionAbility");
    const std::string uiExtType = "sys/commonUI";
    want.SetParam("ability.want.params.uiExtensionType", uiExtType);
    want.SetParam("event", ctxt->event);
    want.SetParam("caller", ctxt->caller);
    Ace::ModalUIExtensionCallbacks callbacks;
    callbacks = {
        .onRelease = [env, ctxt, deferred](int32_t code) {
            LOG_INFO("editEvent onRelease callback.");
            ctxt->_uiContent->CloseModalUIExtension(ctxt->_sessionId);
            napi_resolve_deferred(env, deferred, ctxt->id);
            LOG_INFO("editEvent onRelease done.");
        },
        .onResult = [env, ctxt, deferred](int32_t code, const AAFwk::Want &wantRes) {
            auto eventId = wantRes.GetIntParam("eventId", INVALID_EVENT_ID);
            LOG_INFO("editEvent onResult. eventId=%{public}d", eventId);
            NapiUtil::SetValue(env, eventId, ctxt->id);
        },
        .onReceive = [env, ctxt](const AAFwk::WantParams &wantParams) {
            LOG_INFO("editEvent onReceive.");
        },
        .onError = [env, ctxt, deferred](int32_t code, const std::string &event, const std::string &msg) {
            LOG_ERROR("editEvent onError.%{public}s", msg.c_str());
            ctxt->_uiContent->CloseModalUIExtension(ctxt->_sessionId);
            napi_reject_deferred(env, deferred, ctxt->id);
        },
        .onRemoteReady = [env, ctxt, deferred](const std::shared_ptr<Ace::ModalUIExtensionProxy> &proxy) {
            LOG_INFO("editEvent onRemoteReady.");
        },
        .onDestroy = [env, ctxt, deferred]{
            LOG_INFO("editEvent onDestroy.");
        },
    };
    Ace::ModalUIExtensionConfig config;
    config = {
        .isProhibitBack = false,
    };
    ctxt->_sessionId = ctxt->_uiContent->CreateModalUIExtension(want, callbacks, config);
    LOG_INFO("editEvent CreateModalUI sessionId=%{public}d", ctxt->_sessionId);
    return promise;
}

napi_value CalendarManagerNapi::New(napi_env env, napi_callback_info info)
{
    auto ctxt = std::make_shared<ContextBase>();
    auto input = [env, ctxt](size_t argc, napi_value* argv) {
        CHECK_ARGS_RETURN_VOID(ctxt, argc == 1, "invalid arguments!");
        CalendarEnvNapi::GetInstance().Init(env, argv[0]);
    };
    ctxt->GetCbInfoSync(env, info, input);
    NAPI_ASSERT(env, ctxt->status == napi_ok, "invalid arguments!");

    auto calendarManager = new (std::nothrow) CalendarManagerNapi();
    NAPI_ASSERT(env, calendarManager != nullptr, "no memory for calendarManager");
    auto finalize = [](napi_env env, void *data, void *hint) {
        CalendarManagerNapi *objectInfo = static_cast<CalendarManagerNapi *>(data);
        if (objectInfo != nullptr) {
            delete objectInfo;
            objectInfo = nullptr;
        }
    };
    if (napi_wrap(env, ctxt->self, calendarManager, finalize, nullptr, nullptr) != napi_ok) {
        finalize(env, calendarManager, nullptr);
        return nullptr;
    }
    return ctxt->self;
}

napi_value CalendarManagerNapi::Init(napi_env env, napi_value exports)
{
    napi_property_descriptor properties[] = {
        DECLARE_NAPI_FUNCTION("createCalendar", CreateCalendar),
        DECLARE_NAPI_FUNCTION("deleteCalendar", DeleteCalendar),
        DECLARE_NAPI_FUNCTION("getCalendar", GetCalendar),
        DECLARE_NAPI_FUNCTION("getAllCalendars", GetAllCalendars),
        DECLARE_NAPI_FUNCTION("editEvent", EditEvent),
    };
    napi_value cons = nullptr;
    NAPI_CALL(env,
        napi_define_class(env,
            CALENDAR_MANAGER_CLASS_NAME.c_str(),
            NAPI_AUTO_LENGTH,
            New,
            nullptr,
            sizeof(properties) / sizeof(*properties),
            properties,
            &cons));
    NAPI_CALL(env, napi_create_reference(env, cons, INITIAL_REFCOUNT, &g_constructorRef));
    NAPI_CALL(env, napi_set_named_property(env, exports, CALENDAR_MANAGER_CLASS_NAME.c_str(), cons));

    napi_property_descriptor export_properties[] = {
        DECLARE_NAPI_FUNCTION("getCalendarManager", GetCalendarManager),
    };
    NAPI_CALL(env, napi_define_properties(env, exports, sizeof(export_properties) / sizeof(export_properties[0]),
        export_properties));
    return exports;
}
}