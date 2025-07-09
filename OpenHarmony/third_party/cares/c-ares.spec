Name:           c-ares
Version:        1.18.1
Release:        7
Summary:        A C library for asynchronous DNS requests

License:        MIT
URL:            https://github.com/c-ares/c-ares
Source0:        https://github.com/c-ares/c-ares/releases/download/cares-1_18_1/%{name}-%{version}.tar.gz

BuildRequires:  gcc make autoconf automake libtool g++
# Patch0 from Redhat is applied for stopping overriding AC_CONFIG_MACRO_DIR
Patch0:         0000-Use-RPM-compiler-options.patch
Patch1:         backport-disable-live-tests.patch
Patch2:         backport-add-str-len-check-in-config_sortlist-to-avoid-stack-overflow.patch
Patch3:         backport-CVE-2023-32067.patch
Patch4:         backport-001-CVE-2023-31130.patch
Patch5:         backport-002-CVE-2023-31130.patch
Patch6:         backport-003-CVE-2023-31130.patch
Patch7:         backport-001-CVE-2023-31147.patch
Patch8:         backport-002-CVE-2023-31124_CVE-2023-31147.patch
Patch9:         backport-003-CVE-2023-31147.patch
Patch10:        backport-004-CVE-2023-31147.patch
Patch11:        backport-005-CVE-2023-31147.patch
Patch12:        backport-CVE-2023-31124.patch
Patch13:        backport-CVE-2024-25629.patch

%description
This is c-ares, an asynchronous resolver library. It is intended for applications
which need to perform DNS queries without blocking, or need to perform multiple

%package        devel
Summary:        C-ares development files
Requires:       %{name} = %{version}-%{release} pkgconfig

%description    devel
The headers and libraries files that c-ares required to compile applications or
shared objects are contained in %{name}-devel package.

%package_help

%prep
%autosetup -n %{name}-%{version} -p1

%build
autoreconf -if
%configure --enable-shared --disable-static --disable-dependency-tracking
make %{?_smp_mflags}

%install
%make_install
%delete_la
%ldconfig_scriptlets

%check
%make_build -C test
./test/arestest

%files
%doc CHANGES LICENSE.md
%{_libdir}/*.so.*

%files devel
%{_libdir}/pkgconfig/*.pc
%{_libdir}/libcares.so
%{_includedir}/*.h

%files help
%doc NEWS README.cares README.md
%{_mandir}/man3/*

%changelog
* Mon May 29 2023 xinghe <xinghe2@h-partners.com> - 1.18.1-7
- Type:CVE
- CVE:CVE-2023-31124 CVE-2023-31147
- SUG:NA
- DESC:fix CVE-2023-31124 CVE-2023-31147

* Sun May 28 2023 hehuazhen <hehuazhen@huawei.com> - 1.18.1-6
- Type:CVE
- CVE:CVE-2023-32067 CVE-2023-31130
- SUG:restart
- DESC:fix CVE-2023-32067 CVE-2023-31130

* Sat Feb 25 2023 yanglu <yanglu72@h-partners.com> - 1.18.1-5
- Type:bugfix
- CVE:NA
- SUG:NA
- DESC:delete a static library file
       add BuildRequires make to fix build problem

* Fri Feb 10 2023 xingwei <xingwei14@h-partners.com> - 1.18.1-4
- Type:cves
- CVE:CVE-2022-4904
- SUG:NA
- DESC:fix CVE-2022-4904

* Thu Jun 09 2022 gaihuiying <eaglegai@163.com> - 1.18.1-3
- Type:bugfix
- Id:NA
- SUG:NA
- DESC:enable test

* Mon Apr 18 2022 quanhongfei <quanhongfei@h-partners.com> - 1.18.1-2
- Type:bugfix
- Id:NA
- SUG:NA
- DESC:add libcares.a to c-ares-devel

* Thu Dec 02 2021 quanhongfei <quanhongfei@huawei.com> - 1.18.1-1
- Type:requirements
- Id:NA
- SUG:NA
- DESC:update c-ares to 1.18.1

* Thu Aug 12 2021 gaihuiying <gaihuiying1@huawei.com> - 1.17.1-2
- fix CVE-2021-3672

* Sat Jan 30 2021 xihaochen <xihaochen@huawei.com> - 1.17.1-1
- Type:requirements
- Id:NA
- SUG:NA
- DESC:update c-ares to 1.17.1

* Tue Sep 8 2020 lunankun <lunankun@huawei.com> - 1.16.1-2
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:fix source0 url

* Wed Jul 22 2020 gaihuiying <gaihuiying1@huawei.com> - 1.16.1-1
- Type:requirement
- ID:NA
- SUG:NA
- DESC:update c-ares version to 1.16.1

* Mon Sep 09 2019 openEuler Buildteam <buildteam@openeuler.org> - 1.15.0-1
- Package Init
