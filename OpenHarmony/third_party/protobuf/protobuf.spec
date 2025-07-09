# Build -python subpackage
%bcond_without python
# Build -java subpackage
%bcond_without java

#global rcver rc2

Summary:        Protocol Buffers - Google's data interchange format
Name:           protobuf
Version:        3.14.0
Release:        8
License:        BSD
URL:            https://github.com/protocolbuffers/protobuf
Source:         https://github.com/protocolbuffers/protobuf/releases/download/v%{version}%{?rcver}/%{name}-all-%{version}%{?rcver}.tar.gz
Source1:        protobuf-init.el

Patch9000:      0001-add-secure-compile-option-in-Makefile.patch
Patch9001:      0002-add-secure-compile-fs-check-in-Makefile.patch
Patch9002:      0003-fix-CVE-2021-22570.patch
Patch9003:      0004-Improve-performance-of-parsing-unknown-fields-in-Jav.patch
Patch9004:	0005-fix-CVE-2022-1941.patch
Patch9005:	0006-fix-CVE-2022-3171.patch
Patch9006:	0007-add-coverage-compile-option.patch

BuildRequires:  make autoconf automake emacs gcc-c++ libtool pkgconfig zlib-devel

%description

Protocol Buffers (a.k.a., protobuf) are Google's language-neutral,
platform-neutral, extensible mechanism for serializing structured data.
You can find protobuf's documentation on the Google Developers site.

%package compiler
Summary:	Protocol Buffers compiler
Requires:       %{name} = %{version}-%{release}
Obsoletes:      protobuf-emacs < %{version}
Obsoletes:      protobuf-emacs-el < %{version}
Obsoletes:      protobuf2-compiler
Requires:       emacs-filesystem >= %{_emacs_version}

%description compiler
This package containers Protocol Buffers compiler for all programming languages.


%package devel
Summary:        Protocol Buffers C++ headers and libraries
Requires:       %{name} = %{version}-%{release}
Requires:       %{name}-compiler = %{version}-%{release}
Requires:       zlib-devel pkgconfig vim-enhanced
Provides:       %{name}-static
Provides:       %{name}-vim
Obsoletes:      %{name}-static < %{version}
Obsoletes:      %{name}-vim < %{version}
Obsoletes:      protobuf2-devel
Obsoletes:      protobuf2-vim
Obsoletes:      protobuf2-static


%description devel
This package contains Protocol Buffers compiler for all languages and
C++ headers and libraries

%package lite
Summary:        Protocol Buffers LITE_RUNTIME libraries

%description lite
Protocol Buffers built with optimize_for = LITE_RUNTIME.

The "optimize_for = LITE_RUNTIME" option causes the compiler to generate code
which only depends libprotobuf-lite, which is much smaller than libprotobuf but
lacks descriptors, reflection, and some other features.

%package lite-devel
Summary:        Protocol Buffers LITE_RUNTIME development libraries
Requires:       %{name}-devel = %{version}-%{release}
Requires:       %{name}-lite = %{version}-%{release}
Provides:       %{name}-lite-static
Obsoletes:      %{name}-lite-static <  %{version}
Obsoletes:      protobuf2-lite-devel
Obsoletes:      protobuf2-lite-static

%description lite-devel
This package contains development libraries built with
optimize_for = LITE_RUNTIME.

The "optimize_for = LITE_RUNTIME" option causes the compiler to generate code
which only depends libprotobuf-lite, which is much smaller than libprotobuf but
lacks descriptors, reflection, and some other features.

%if %{with python}
%package -n python%{python3_pkgversion}-%{name}
Summary:        Python 3 bindings for Google Protocol Buffers
BuildArch:      noarch
BuildRequires:  python%{python3_pkgversion}-devel
BuildRequires:  python%{python3_pkgversion}-setuptools
BuildRequires:  python%{python3_pkgversion}-wheel
Requires:       python%{python3_pkgversion}-six >= 1.9
Conflicts:      %{name}-compiler > %{version}-%{release}
Conflicts:      %{name}-compiler < %{version}-%{release}
Provides:       %{name}-python3 = %{version}-%{release}
%{?python_provide:%python_provide python%{python3_pkgversion}-%{name}}

%description -n python%{python3_pkgversion}-%{name}
This package contains Python 3 libraries for Google Protocol Buffers
%endif


%if %{with java}
%package java
Summary:        Java Protocol Buffers runtime library
BuildArch:      noarch
BuildRequires:  maven-local
BuildRequires:  mvn(com.google.code.gson:gson)
BuildRequires:  mvn(com.google.guava:guava)
BuildRequires:  mvn(junit:junit)
BuildRequires:  mvn(org.apache.felix:maven-bundle-plugin)
BuildRequires:  mvn(org.apache.maven.plugins:maven-antrun-plugin)
BuildRequires:  mvn(org.apache.maven.plugins:maven-source-plugin)
BuildRequires:  mvn(org.codehaus.mojo:build-helper-maven-plugin)
BuildRequires:  mvn(org.easymock:easymock)
Obsoletes:      %{name}-javanano < 3.6.0

%description java
This package contains Java Protocol Buffers runtime library.

%package javalite
Summary:        Java Protocol Buffers lite runtime library
BuildArch:      noarch

%description javalite
This package contains Java Protocol Buffers lite runtime library.

%package java-util
Summary:        Utilities for Protocol Buffers
BuildArch:      noarch

%description java-util
Utilities to work with protos. It contains JSON support
as well as utilities to work with proto3 well-known types.

%package javadoc
Summary:        Javadoc for %{name}-java
BuildArch:      noarch

%description javadoc
This package contains the API documentation for %{name}-java.

%package parent
Summary:        Protocol Buffer Parent POM
BuildArch:      noarch

%description parent
Protocol Buffer Parent POM.

%package bom
Summary:        Protocol Buffer BOM POM
BuildArch:      noarch

%description bom
Protocol Buffer BOM POM.

%endif

%prep
%setup -q -n %{name}-%{version}%{?rcver}
%autopatch -p1
find -name \*.cc -o -name \*.h | xargs chmod -x
chmod 644 examples/*
%if %{with java}
#%pom_remove_dep com.google.truth:truth java/pom.xml
#%pom_remove_dep org.easymock:easymockclassextension java/pom.xml java/*/pom.xml
%pom_remove_dep org.easymock:easymockclassextension java/pom.xml java/core/pom.xml java/lite/pom.xml java/util/pom.xml
%pom_remove_dep com.google.truth:truth java/pom.xml java/util/pom.xml java/lite/pom.xml java/core/pom.xml
%pom_remove_dep com.google.errorprone:error_prone_annotations java/util/pom.xml
%pom_remove_dep com.google.guava:guava-testlib java/pom.xml java/util/pom.xml

# These use easymockclassextension
rm java/core/src/test/java/com/google/protobuf/ServiceTest.java

#rm -r java/core/src/test

# These use truth or error_prone_annotations or guava-testlib
rm java/core/src/test/java/com/google/protobuf/LiteralByteStringTest.java
rm java/core/src/test/java/com/google/protobuf/BoundedByteStringTest.java
rm java/core/src/test/java/com/google/protobuf/RopeByteStringTest.java
rm java/core/src/test/java/com/google/protobuf/RopeByteStringSubstringTest.java
rm java/core/src/test/java/com/google/protobuf/TextFormatTest.java
rm java/core/src/test/java/com/google/protobuf/UnknownFieldSetTest.java
rm java/core/src/test/java/com/google/protobuf/UnknownFieldSetPerformanceTest.java
rm -r java/util/src/test/java/com/google/protobuf/util
rm -r java/util/src/main/java/com/google/protobuf/util

# Make OSGi dependency on sun.misc package optional
%pom_xpath_inject "pom:configuration/pom:instructions" "<Import-Package>sun.misc;resolution:=optional,*</Import-Package>" java/core

# Backward compatibility symlink
%mvn_file :protobuf-java:jar: %{name}/%{name}-java %{name}

# This test is incredibly slow on arm
# https://github.com/protocolbuffers/protobuf/issues/2389
%ifarch %{arm}
mv java/core/src/test/java/com/google/protobuf/IsValidUtf8Test.java \
   java/core/src/test/java/com/google/protobuf/IsValidUtf8Test.java.slow
mv java/core/src/test/java/com/google/protobuf/DecodeUtf8Test.java \
  java/core/src/test/java/com/google/protobuf/DecodeUtf8Test.java.slow
%endif
%endif

rm -f src/solaris/libstdc++.la

%build
iconv -f iso8859-1 -t utf-8 CONTRIBUTORS.txt > CONTRIBUTORS.txt.utf8
mv CONTRIBUTORS.txt.utf8 CONTRIBUTORS.txt
export PTHREAD_LIBS="-lpthread"
./autogen.sh
%configure	
%make_build CXXFLAGS="%{build_cxxflags} -Wno-error=type-limits"

%if %{with python}
pushd python
%py3_build
popd
%endif

%if %{with java}
%mvn_build -s -- -f java/pom.xml
%endif

%{_emacs_bytecompile} editors/protobuf-mode.el


%check
# Java tests fail on s390x
%ifarch s390x
fail=0
%else
fail=1
%endif
%make_build check CXXFLAGS="%{build_cxxflags} -Wno-error=type-limits" || exit $fail


%install
%make_install %{?_smp_mflags} STRIPBINARIES=no INSTALL="%{__install} -p" CPPROG="cp -p"
find %{buildroot} -type f -name "*.la" -exec rm -f {} \;

%if %{with python}
pushd python
#python ./setup.py install --root=%{buildroot} --single-version-externally-managed --record=INSTALLED_FILES --optimize=1
%py3_install
find %{buildroot}%{python3_sitelib} -name \*.py |
  xargs sed -i -e '1{\@^#!@d}'
popd
%endif
install -p -m 644 -D editors/proto.vim %{buildroot}%{_datadir}/vim/vimfiles/syntax/proto.vim

%if %{with java}
%mvn_install
%endif

mkdir -p %{buildroot}%{_emacs_sitelispdir}/%{name}
install -p -m 0644 editors/protobuf-mode.el %{buildroot}%{_emacs_sitelispdir}/%{name}
install -p -m 0644 editors/protobuf-mode.elc %{buildroot}%{_emacs_sitelispdir}/%{name}
mkdir -p %{buildroot}%{_emacs_sitestartdir}
install -p -m 0644 %{SOURCE1} %{buildroot}%{_emacs_sitestartdir}

%ldconfig_scriptlets
%ldconfig_scriptlets lite
%ldconfig_scriptlets compiler

%files
%doc CHANGES.txt CONTRIBUTORS.txt README.md
%license LICENSE
%{_libdir}/libprotobuf.so.25*

%files compiler
%{_bindir}/protoc
%{_libdir}/libprotoc.so.25*
%{_emacs_sitelispdir}/%{name}/
%{_emacs_sitestartdir}/protobuf-init.el
%license LICENSE
%doc README.md


%files devel
%dir %{_includedir}/google
%{_includedir}/google/protobuf/
%{_libdir}/libprotobuf.so
%{_libdir}/libprotoc.so
%{_libdir}/pkgconfig/protobuf.pc
%doc examples/add_person.cc examples/addressbook.proto examples/list_people.cc examples/Makefile examples/README.md
%{_libdir}/libprotobuf.a
%{_libdir}/libprotoc.a
%{_datadir}/vim/vimfiles/syntax/proto.vim
 
%files lite
%{_libdir}/libprotobuf-lite.so.25*

%files lite-devel
%{_libdir}/libprotobuf-lite.so
%{_libdir}/pkgconfig/protobuf-lite.pc
%{_libdir}/libprotobuf-lite.a

%if %{with python}
%files -n python%{python3_pkgversion}-protobuf
%dir %{python3_sitelib}/google
%{python3_sitelib}/google/protobuf/
%{python3_sitelib}/protobuf-%{version}%{?rcver}-py3.*.egg-info/
%{python3_sitelib}/protobuf-%{version}%{?rcver}-py3.*-nspkg.pth
%doc python/README.md
%doc examples/add_person.py examples/list_people.py examples/addressbook.proto
%endif

%if %{with java}
%files java -f .mfiles-protobuf-java
%doc examples/AddPerson.java examples/ListPeople.java
%doc java/README.md
%license LICENSE

%files java-util -f .mfiles-protobuf-java-util

%files javadoc -f .mfiles-javadoc
%license LICENSE

%files parent -f .mfiles-protobuf-parent
%license LICENSE

%files bom -f .mfiles-protobuf-bom
%license LICENSE

%files javalite -f .mfiles-protobuf-javalite
%license LICENSE
%endif

%changelog
* Wed Dec 06 2023 konglidong <konglidong@uniontech.com> - 3.14.0-8
- obsolets protobuf2 for fix install conflict

* Mon Nov 21 2022 chengzeruizhi <chengzeruizhi@huawei.com> - 3.14.0-7
- Type:enhancement
- ID:NA                  
- SUG:NA                                                             
- DESC: add compile option for coverage

* Tue Oct 18 2022 chengzeruizhi <chengzeruizhi@huawei.com> - 3.14.0-6
- Type:bugfix
- ID:NA
- SUG:NA
- DESC: fix CVE-2022-3171

* Tue Oct 11 2022 chengzeruizhi <chengzeruizhi@huawei.com> - 3.14.0-5
- Type:bugfix
- ID:NA
- SUG:NA
- DESC: fix CVE-2022-1941

* Wed Apr 27 2022 wangxiaochao <wangxiaochao2@huawei.com> - 3.14.0-4
- Type:bugfix
- ID:NA
- SUG:NA
- DESC: Improve performance of parsing unknown fields in Java

* Fri Mar 18 2022 wangxiaochao <wangxiaochao2@huawei.com> - 3.14.0-3
- Type:bugfix
- ID:NA
- SUG:NA
- DESC: fix CVE-2021-22570

* Thu Mar 10 2022 wangxiaochao <wangxiaochao2@huawei.com> - 3.14.0-2
- Type:bugfix
- ID:NA
- SUG:NA
- DESC: fix mainline compile failed

* Fri Jul 30 2021 liyanan <liyanan32@huawei.com> - 3.14.0-1
- update to 3.14.0

* Mon Apr 26 2021 haozi007 <liuhao27@huawei.com> - 3.12.3-16
- Type:enhancement
- ID:NA
- SUG:NA
- DESC: split compiler from devel

* Sat Feb 20 2021 haozi007 <liuhao27@huawei.com> - 3.12.3-15
- Type:enhancement
- ID:NA
- SUG:NA
- DESC: add fstack check

* Tue Sep 1 2020 wutao <wutao61@huawei.com> - 3.12.3-14
- Type:enhancement
- ID:NA
- SUG:NA
- DESC: enhance java function and fix build errors

* Sat Aug 29 2020 openEuler Buildteam <buildteam@openeuler.org> - 3.12.3-13
- Type:bugfix
- ID:NA
- SUG:NA
- DESC: remove Conflicts

* Fri Aug 28 2020 openEuler Buildteam <buildteam@openeuler.org> - 3.12.3-12
- Type:bugfix
- ID:NA
- SUG:NA
- DESC: invalid version of Conflicts

* Thu Jul 23 2020 openEuler Buildteam <buildteam@openeuler.org> - 3.12.3-11
- Type:enhancement
- ID:NA
- SUG:NA
- DESC: drop python2-protobuf and refactor .spec file

* Thu Jul 16 2020 openEuler Buildteam <buildteam@openeuler.org> - 3.12.3-10
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:upgrade from 3.9.0 to 3.12.3

* Wed Apr 08 2020 openEuler Buildteam <buildteam@openeuler.org> - 3.9.0-9
- Type:enhancement
- ID:NA
- SUG:NA
- DESC: remove unnecessary files

* Thu Dec 12 2019 openEuler Buildteam <buildteam@openeuler.org> - 3.9.0-8.h3
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:add bind now secure compile option

* Wed Nov 27 2019 openEuler Buildteam <buildteam@openeuler.org> - 3.9.0-8.h2
- Type:enhancement
- ID:NA
- SUG:NA
- DESC:compatible to centos 7.5

* Tue Nov 26 2019 openEuler Buildteam <buildteam@openeuler.org> - 3.9.0-8.h1
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:upgrade from 3.5.0 to 3.9.0

* Fri Nov 01 2019 openEuler Buildteam <buildteam@openeuler.org> - 3.5.0-8.h1
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:change patch's names according to new rules
