// Copyright (C) 2012 The Libphonenumber Authors

#include "phonenumbers/geocoding/phonenumber_offline_geocoder.h"
#include "phonenumbers/phonenumberutil.h"
#include <string>
#include <unicode/locid.h>

using i18n::phonenumbers::PhoneNumberOfflineGeocoder;

PhoneNumberOfflineGeocoder* offlineGeocoder = NULL;
i18n::phonenumbers::PhoneNumberUtil* util = NULL;
extern "C" int exposeLocationName(const char* pNumber, const char* locale, char* res, const int resLength);
