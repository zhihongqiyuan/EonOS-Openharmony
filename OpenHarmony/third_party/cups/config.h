/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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
 */
/* config.h.  Generated from config.h.in by configure.  */

#ifndef _CUPS_CONFIG_H_
#define _CUPS_CONFIG_H_

#define CUPS_SVERSION	"CUPS v2.4.12"
#define CUPS_MINIMAL	"CUPS/2.4.12"
#define CUPS_LITE	0
#define CUPS_DEFAULT_USER	"cups"
#define CUPS_DEFAULT_GROUP	"cups"
#define CUPS_DEFAULT_SYSTEM_GROUPS	"print"
#define CUPS_DEFAULT_PRINTOPERATOR_AUTH	"@cups"
#define CUPS_DEFAULT_SYSTEM_AUTHKEY	""
#define CUPS_DEFAULT_CONFIG_FILE_PERM	0640
#define CUPS_DEFAULT_LOG_FILE_PERM	0644
#define CUPS_DEFAULT_LOG_LEVEL	"debug2"
#define CUPS_DEFAULT_ACCESS_LOG_LEVEL "none"
#define CUPS_DEFAULT_MAX_LOG_SIZE "1m"
#define CUPS_DEFAULT_FATAL_ERRORS	"config"
#define CUPS_DEFAULT_BROWSING	0
#define CUPS_DEFAULT_BROWSE_LOCAL_PROTOCOLS	""
#define CUPS_DEFAULT_DEFAULT_SHARED 0
#define CUPS_DEFAULT_IPP_PORT	1631
#define CUPS_DEFAULT_PRINTCAP	"/data/service/el1/public/print_service/cups/printcap"
#define CUPS_DEFAULT_ERROR_POLICY "stop-printer"
#define CUPS_DEFAULT_MAX_COPIES	9999
#define CUPS_DEFAULT_WEBIF 0
#define HAVE_POSIX_SPAWN 1
#define HAVE_LIBZ 1
#define HAVE_INFLATECOPY 1
#define HAVE_LIBPAM 0
#define HAVE_CRYPT_H 1
#define HAVE_STDINT_H 1

#define HAVE_LONG_LONG 1
#define CUPS_LLFMT	"%lld"
#define CUPS_LLCAST	(long long)

#define HAVE_STRTOLL 1

#ifndef HAVE_STRTOLL
#  define strtoll(nptr, endptr, base) strtol((nptr), (endptr), (base))
#endif /* !HAVE_STRTOLL */

#define HAVE_STRDUP 1
#define HAVE_GETEUID 1
#define HAVE_SETPGID 1
#define HAVE_SNPRINTF 1
#define HAVE_VSNPRINTF 1
#define HAVE_SIGACTION 1
#define HAVE_WAITPID 1
#define HAVE_WAIT3 1
#define HAVE_MALLOC_H 1
#define HAVE_LANGINFO_H 1
#define HAVE_TLS 1
#define HAVE_OPENSSL 1
#define HAVE_SYS_IOCTL_H 1
#define HAVE_TM_GMTOFF 1
#define HAVE_RRESVPORT_AF 1
#define HAVE_GETADDRINFO 1
#define HAVE_GETNAMEINFO 1
#define HAVE_GETIFADDRS 1
#define HAVE_HSTRERROR 1
#define HAVE_RES_INIT 1
#define HAVE_RESOLV_H 1
#define HAVE_PTHREAD_H 1
#define HAVE_ONDEMAND 1
#define HAVE_GETGROUPLIST 1
#define CUPS_DEFAULT_GSSSERVICENAME	"host"
#define HAVE_POLL 1
#define HAVE_EPOLL 1
#define HAVE_SYS_PARAM_H 1

#ifdef HAVE_ARC4RANDOM
#  define CUPS_RAND() arc4random()
#  define CUPS_SRAND(v) arc4random_stir()
#else
#  define CUPS_RAND() rand()
#  define CUPS_SRAND(v) srand(v)
#endif /* HAVE_ARC4RANDOM */

#define HAVE_LIBUSB 1
#define HAVE_OPENHARMONY 1
#define HAVE_STATFS 1
#define HAVE_STATVFS 1
#define HAVE_SYS_MOUNT_H 1
#define HAVE_SYS_STATFS_H 1
#define HAVE_SYS_STATVFS_H 1
#define HAVE_SYS_VFS_H 1
#define HAVE_SNAPD_CLIENT_RUN_SNAPCTL2_SYNC 1

#endif /* !_CUPS_CONFIG_H_ */
