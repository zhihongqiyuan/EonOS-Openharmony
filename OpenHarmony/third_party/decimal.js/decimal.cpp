/*!
 *  decimal.js v10.4.3
 *  An arbitrary-precision Decimal type for JavaScript.
 *  https://github.com/MikeMcl/decimal.js
 *  Copyright (c) 2024 Huawei Device Co., Ltd.
 *  MIT Licence
 */

#include "native_engine/native_engine.h"

extern const char _binary_decimal_mjs_start[];
extern const char _binary_decimal_mjs_end[];
extern const char _binary_decimal_abc_start[];
extern const char _binary_decimal_abc_end[];

// Napi get mjs code function
extern "C" __attribute__((visibility("default"))) 
void NAPI_arkts_math_Decimal_GetMJSCode(const char **buf, int *buflen)
{
    if (buf != nullptr) {
        *buf = _binary_decimal_mjs_start;
    }
    if (buflen != nullptr) {
        *buflen = _binary_decimal_mjs_end - _binary_decimal_mjs_start;
    }
}

// Napi get abc code function
extern "C" __attribute__((visibility("default"))) 
void NAPI_arkts_math_Decimal_GetABCCode(const char **buf, int *buflen)
{
    if (buf != nullptr) {
        *buf = _binary_decimal_abc_start;
    }
    if (buflen != nullptr) {
        *buflen = _binary_decimal_abc_end - _binary_decimal_abc_start;
    }
}

/*
 * Module define
 */
static napi_module DecimalModule = {
    .nm_filename = nullptr,
    .nm_modname = "arkts.math.Decimal",
};
extern "C" __attribute__((constructor)) void DecimalRegisterModule(void)
{
    napi_module_register(&DecimalModule);
}
