Name:           libpng
Epoch:          2
Version:        1.6.38
Release:        2
Summary:        A library of functions for manipulating PNG image format files
License:        zlib
URL:            http://www.libpng.org/pub/png/libpng.html
Source0:        https://github.com/glennrp/%{name}/archive/v%{version}/%{name}-%{version}.tar.gz
Source1:        pngusr.dfa

Patch0:         libpng-multilib.patch
Patch1:         libpng-fix-arm-neon.patch
Patch2:         CVE-2019-6129.patch

BuildRequires:  zlib-devel autoconf automake libtool

%description
The libpng package contains libraries used by other programs for reading and writing PNG format files.
The PNG format was designed as a replacement for GIF and, to a lesser extent, TIFF,
with many improvements and extensions and lack of patent problems.

%package        devel
Summary:        Development files for libpng
Requires:       %{name}%{?_isa} = %{epoch}:%{version}-%{release} zlib-devel%{?_isa} pkgconfig%{?_isa}

%description    devel
The libpng-devel package contains libraries and header files for developing
applications that using the PNG library.

%package static
Summary:       Static PNG image format file library
Requires:      %{name}-devel%{?_isa} = %{epoch}:%{version}-%{release}
 
%description static
The libpng-static package contains the statically linkable version of libpng.
Linking to static libraries is discouraged for most applications, but it is
necessary for some boot packages.
 
%package tools
Summary:       Tools for PNG image format file library
Requires:      %{name}%{?_isa} = %{epoch}:%{version}-%{release}
 
%description tools
The libpng-tools package contains tools used by the authors of libpng.

%package        help
Summary:        Help documents for libpng

%description    help
This package contain the license files and help documents for libpng.

%prep
%autosetup -n %{name}-%{version} -p1
cp -p %{SOURCE1} .

%build
autoreconf -vif
%configure
%make_build DFA_XTRA=pngusr.dfa

%install
%make_install
rm -f %{buildroot}%{_libdir}/*.la

%check
make check

%ldconfig_post
%ldconfig_postun

%files
%{!?_licensedir:%global license %%doc}
%license LICENSE
%{_libdir}/libpng16.so.*

%files devel
%{_includedir}/*
%{_libdir}/libpng*.so
%{_libdir}/pkgconfig/libpng*.pc

%files static
%{_libdir}/libpng*.a
 
%files tools
%{_bindir}/*

%files help
%doc libpng-manual.txt TODO CHANGES
%{_mandir}/man*/*

%changelog
* Thu Dec 15 2022 zhouwenpei <zhouwenpei1@h-partners.com> - 1.6.38-2
- remove example.c from help

* Thu Nov 03 2022 zhouwenpei <zhouwenpei1@h-partners.com> - 1.6.38-1
- update 1.6.38

* Tue Mar 29 2022 liuyumeng <liuyumeng5@h-partners.com> - 1.6.37-3
- enable test and fix failed tests

* Sat Dec 25 2021 hanhui <hanhui15@huawei.com> - 1.6.37-2
- DESC:split tools and static packages

* Mon May 11 2020 openEuler Buildteam <buildteam@openeuler.org> - 1.6.37-1
- Type:requirement
- ID:NA
- SUG:NA
- DESC:update 1.6.37

* Sun Jan 12 2020 openEuler Buildteam <buildteam@openeuler.org> - 1.6.36-4
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:fix cve

* Tue Dec 31 2019 openEuler Buildteam <buildteam@openeuler.org> - 1.6.36-3
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:optimization the spec

* Wed Sep 18 2019 chenzhenyu <chenzhenyu13@huawei.com> - 1.6.36-2
- Package init
