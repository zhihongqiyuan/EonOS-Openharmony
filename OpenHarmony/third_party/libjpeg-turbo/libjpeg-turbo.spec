Name:           libjpeg-turbo
Version:        2.1.1
Release:        5
Summary:        MMX/SSE2/SIMD accelerated libjpeg-compatible JPEG codec library
License:        IJG
URL:            http://sourceforge.net/projects/libjpeg-turbo
Source0:        http://downloads.sourceforge.net/libjpeg-turbo/libjpeg-turbo-%{version}.tar.gz
%ifarch loongarch64
Patch0001:      Add-loongarch64.patch
%endif

BuildRequires:  gcc cmake libtool nasm

Obsoletes:      libjpeg < 6b-47 turbojpeg < %{version}-%{release}  
Provides:       libjpeg = 6b-47 turbojpeg = %{version}-%{release}  

%description
libjpeg-turbo is a JPEG image codec that uses SIMD instructions (MMX, SSE2, NEON, AltiVec)
to accelerate baseline JPEG compression and decompression on x86, x86-64, and ARM systems.

%package        devel
Summary:        Development files for the libjpeg-turbo library
Requires:       libjpeg-turbo = %{version}-%{release}
Provides:       libjpeg-turbo-static = 1.3.1 turbojpeg-devel = %{version}-%{release}
Obsoletes:      libjpeg-turbo-static < 1.3.1 turbojpeg-devel < %{version}-%{release}
Provides:       libjpeg-devel = 6b-47 libjpeg-devel%{?_isa} = %{version}-%{release}
Obsoletes:      libjpeg-devel < 6b-47


%description devel
Development files for the libjpeg-turbo library.

%package utils
Summary:        Utilities for manipulating JPEG images
Requires:       libjpeg-turbo%{?_isa} = %{version}-%{release}
 
%description utils
The libjpeg-turbo-utils package contains simple client programs for accessing
the libjpeg functions. It contains cjpeg, djpeg, jpegtran, rdjpgcom and
wrjpgcom. Cjpeg compresses an image file into JPEG format. Djpeg decompresses a
JPEG file into a regular image file. Jpegtran can perform various useful
transformations on JPEG files. Rdjpgcom displays any text comments included in a
JPEG file. Wrjpgcom inserts text comments into a JPEG file.

%package -n turbojpeg
Summary:        TurboJPEG library
 
%description -n turbojpeg
The turbojpeg package contains the TurboJPEG shared library.

%package -n turbojpeg-devel
Summary:        Headers for the TurboJPEG library
Requires:       turbojpeg%{?_isa} = %{version}-%{release}
 
%description -n turbojpeg-devel
This package contains header files necessary for developing programs which will
manipulate JPEG files using the TurboJPEG library.

%package_help

%prep
%autosetup -n %{name}-%{version} -p1

%build
%{cmake} -DCMAKE_INSTALL_LIBDIR=%{_libdir} -DCMAKE_SKIP_RPATH:BOOL=YES -DCMAKE_SKIP_INSTALL_RPATH:BOOL=YES -DENABLE_STATIC:BOOL=NO .

%make_build V=1

%install
%make_install
%delete_la

chmod -x README.md

%ifarch x86_64
  %global wordsize "64"
%else
  %global wordsize ""
%endif

if test -n "$wordsize"
then
  pushd $RPM_BUILD_ROOT%{_includedir}
    mv jconfig.h jconfig-$wordsize.h
    cat > jconfig.h <<EOF
#ifndef JCONFIG_H_MULTILIB
#define JCONFIG_H_MULTILIB

#include <bits/wordsize.h>

#if __WORDSIZE == 32
# include "jconfig-32.h"
#elif __WORDSIZE == 64
# include "jconfig-64.h"
#else
# error "unexpected value for __WORDSIZE macro"
#endif

#endif
EOF
fi
    
%check
LD_LIBRARY_PATH=%{buildroot}%{_libdir} make test %{?_smp_mflags}

%ldconfig_scriptlets

%files
%defattr(-,root,root)
%doc README.*
%license LICENSE.md
%{_libdir}/libjpeg.so.62*
%exclude /usr/share/doc/libjpeg-turbo/*

%files devel
%doc coderules.txt jconfig.txt libjpeg.txt structure.txt example.txt 
%exclude %{_includedir}/turbojpeg.h
%{_includedir}/*.h
%{_libdir}/libjpeg.so
%{_libdir}/pkgconfig/libjpeg.pc
%{_libdir}/cmake/%{name}/%{name}*.cmake

%files utils
%doc usage.txt wizard.txt
%{_bindir}/cjpeg
%{_bindir}/djpeg
%{_bindir}/jpegtran
%{_bindir}/rdjpgcom
%{_bindir}/wrjpgcom
%{_bindir}/tjbench

%files -n turbojpeg
%license LICENSE.md
%{_libdir}/libturbojpeg.so.0*

%files -n turbojpeg-devel
%doc tjexample.c
%{_includedir}/turbojpeg.h
%{_libdir}/libturbojpeg.so
%{_libdir}/pkgconfig/libturbojpeg.pc

%files help
%doc ChangeLog.md
%{_mandir}/man1/*.1*

%changelog
* Mon Apr 24 2023 guoqinglan <guoqinglan@kylinsec.com.cn> - 2.1.1-5
- fix build error for sw_64

* Thu Nov 17 2022 doupengda <doupengda@loongson.cn> - 2.1.1-4
- add loongarch64 support

* Wed Jul 13 2022 wangkerong <wangkerong@h-partners.com> - 2.1.1-3
- utils provide by libjpeg-turbo-utils subpackage,the main package shouldn't provide it

* Wed Mar 30 2022 liuyumeng <liuyumeng5@h-partners.com> - 2.1.1-2
- enable tests

* Sat Nov 27 2021 wangkerong <wangkerong@huawei.com> - 2.1.1-1
- update to 2.1.1
- splite utils turbojpeg turbojpeg-devel subpackages

* Wed Jan 27 2021 hanhui <hanhui15@huawei.com> - 2.0.6-1
- Type: enhancement
- ID:   NA
- SUG:  NA
- DESC: update to 2.0.6

* Wed Jul 22 2020 openEuler Buildteam <buildteam@openeuler.org> - 2.0.5-1
- Type:enhancement
- Id:NA
- SUG:NA
- DESC:upgrade to 2.0.5

* Tue Jan 14 2020 openEuler Buildteam <buildteam@openeuler.org> - 2.0.0-4
- Type:bugfix
- Id:NA
- SUG:NA
- DESC:remove the libjpeg-devel with arch in obsoletes

* Fri Nov 1 2019 openEuler Buildteam <buildteam@openeuler.org> - 2.0.0-3
- Type:bugfix
- Id:NA
- SUG:NA
- DESC:add the libjpeg-devel and jconfig.h

* Sat Sep 21 2019 Lijin Yang <yanglijin@huawei.com> - 2.0.0-2
- Package init
