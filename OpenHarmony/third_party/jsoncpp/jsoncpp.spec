Name:           jsoncpp
Version:        1.9.5
Release:        5
Summary:        JSON C++ library
License:        Public Domain or MIT
URL:            https://github.com/open-source-parsers/jsoncpp
Source0:        https://github.com/open-source-parsers/jsoncpp/archive/%{version}/%{name}-%{version}.tar.gz
BuildRequires:  gcc-c++ cmake >= 3.1 python3-devel

Patch0001:      0001-Parse-large-floats-as-infinity-1349-1353.patch
Patch0002:      0001-Use-default-rather-than-hard-coded-8-for-maximum-agg.patch

%description
JsonCpp is a C++ library that allows manipulating JSON values,
including serialization and deserialization to and from strings.
It can also preserve existing comment in unserialization/serialization steps,
making it a convenient format to store user input files.

%package        devel
Summary:        Development files for jsoncpp
Requires:       %{name} = %{version}-%{release}

%description    devel
Document files contain library and head files for jsoncpp.

%package        help
Summary:        Document for jsoncpp
BuildRequires:  doxygen graphviz hardlink
BuildArch:      noarch
Provides:       jsoncpp-doc = %{version}-%{release}
Obsoletes:      jsoncpp-doc < %{version}-%{release}

%description    help
Help document for jsoncpp.

%prep
%autosetup -p 1
doxygen -s -u doc/doxyfile.in
sed -i -e 's!^DOT_FONTNAME.*=.*!DOT_FONTNAME =!g' doc/doxyfile.in

%build
install -d %{_vpath_builddir}
cd %{_vpath_builddir}
%cmake -DBUILD_STATIC_LIBS=OFF -DJSONCPP_WITH_WARNING_AS_ERROR=OFF \
       -DBUILD_OBJECT_LIBS:BOOL=OFF \
       -DJSONCPP_WITH_PKGCONFIG_SUPPORT=ON -DJSONCPP_WITH_CMAKE_PACKAGE=ON \
       -DJSONCPP_WITH_POST_BUILD_UNITTEST=OFF -DPYTHON_EXECUTABLE="%{__python3}" \
       ..
cd ..
%make_build -C %{_vpath_builddir}
cp -p %{_vpath_builddir}/version .
%{__python3} doxybuild.py --with-dot --doxygen %{_bindir}/doxygen
rm -f version

%install
%make_install -C %{_vpath_builddir}
mkdir -p %{buildroot}%{_docdir}/%{name}
cp README.md %{buildroot}%{_docdir}/%{name}
chmod 644 %{buildroot}%{_docdir}/%{name}/README.md
touch %{buildroot}%{_docdir}/%{name}//README.md
mkdir -p %{buildroot}%{_docdir}/%{name}/html
cp dist/doxygen/*/*.{html,png} %{buildroot}%{_docdir}/%{name}/html
chmod 644 %{buildroot}%{_docdir}/%{name}/html/*.{html,png}
touch %{buildroot}%{_docdir}/%{name}/html
hardlink -cfv %{buildroot}%{_docdir}/%{name}

# When the .so file name changes during version upgrade, you need to copy the lower version of .so file
# cp -a %{_libdir}/libjsoncpp.so.* $RPM_BUILD_ROOT%{_libdir}

%check
%make_build -C %{_vpath_builddir} jsoncpp_check

%post
/sbin/ldconfig

%postun
/sbin/ldconfig

%files
%license AUTHORS LICENSE
%doc %dir %{_docdir}/%{name}
%exclude %{_docdir}/%{name}/html
%{_libdir}/lib%{name}.so.*

%files devel
%{_libdir}/lib%{name}.so
%{_includedir}/json
%{_libdir}/cmake/*
%{_libdir}/pkgconfig/%{name}.pc

%files help
%doc %{_docdir}/%{name}
%doc %{_docdir}/%{name}/README.md


%changelog
* Thu Nov 16 2023 liubo <liubo1@xfusion.com> - 1.9.5-5
- Use default rather than hard-coded 8 for maximum aggregate member alignment

* Wed Oct 18 2023 liubo <liubo1@xfusion.com> - 1.9.5-4
- Parse large floats as infinity

* Mon Jan 10 2022 shixuantong <shixuantong@huawei.com> - 1.9.5-3
- Delete so files of lower versions

* Wed Jan 05 2022 shangyibin <shangyibin1@huawei.com> - 1.9.5-2
- copy the .so file of the old version.

* Wed Dec 22 2021 shangyibin <shangyibin1@huawei.com> - 1.9.5-1
- upgrade version to 1.9.5

* Wed Feb 3 2021 liudabo <liudabo1@huawei.com> - 1.9.4-1
- upgrade version to 1.9.4

* Sat Aug 08 2020 lingsheng<lingsheng@huawei.com> - 1.9.3-2
- Remove old version so

* Fri Jul 31 2020 wenzhanli<wenzhanli2@huawei.com> - 1.9.3-1
- Type:NA
- ID:NA
- SUG:NA
- DESC:update version 1.9.3

* Mon Nov 25 2019 zhujunhao <zhujunhao5@huawei.com> - 1.8.4-6
- Package init
