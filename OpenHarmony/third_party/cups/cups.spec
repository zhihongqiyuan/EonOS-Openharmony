
%global cups_serverbin %{_exec_prefix}/lib/cups

Name:    cups
Epoch:   1
Version: 2.4.12
Release: 12
Summary: CUPS is the standards-based, open source printing system for linux operating systems.
License: Apache-2.0 WITH LLVM-exception
Url:     https://openprinting.github.io/cups/
# Apple stopped uploading the new versions into github, use OpenPrinting fork
Source0: https://github.com/OpenPrinting/cups/releases/download/v%{version}/cups-%{version}-source.tar.gz

Source1: cupsprinter.png
Source2: macros.cups

Patch1:  cups-system-auth.patch
Patch2:  cups-multilib.patch
Patch3:  cups-banners.patch
Patch4:  cups-direct-usb.patch
Patch5:  cups-driverd-timeout.patch
Patch6:  cups-usb-paperout.patch
Patch7:  cups-uri-compat.patch
Patch8:  cups-freebind.patch
Patch9:  cups-ipp-multifile.patch
Patch10: cups-web-devices-timeout.patch

Patch6000: backport-CVE-2022-26691.patch
Patch6001: backport-Remove-legacy-code-for-RIP_MAX_CACHE-environment-variable.patch
Patch6002: backport-Also-fix-cupsfilter.patch
Patch6003: backport-CVE-2023-32324.patch
Patch6004: fix-httpAddrGetList-test-case-fail.patch
Patch6005: backport-CVE-2023-34241.patch
Patch6006: backport-CVE-2023-4504.patch

BuildRequires: pam-devel pkgconf-pkg-config pkgconfig(gnutls) libacl-devel openldap-devel pkgconfig(libusb-1.0)
BuildRequires: krb5-devel pkgconfig(avahi-client) systemd pkgconfig(libsystemd) pkgconfig(dbus-1) python3-cups
BuildRequires: automake zlib-devel gcc gcc-c++ libselinux-devel audit-libs-devel make
Requires: dbus systemd acl cups-filters /usr/sbin/alternatives 
Requires: %{name}-libs%{?_isa} = %{epoch}:%{version}-%{release}
Requires: %{name}-client%{?_isa} = %{epoch}:%{version}-%{release}
Requires: %{name}-filesystem = %{epoch}:%{version}-%{release}

# Requires working PrivateTmp (bug #807672)
Requires(pre): systemd
Requires(post): systemd
Requires(post): grep, sed
Requires(preun): systemd
Requires(postun): systemd

%description
CUPS is the standards-based, open source printing system developed by Apple Inc.
for UNIX®-like operating systems. CUPS uses the Internet Printing
Protocol (IPP) to support printing to local and network printers.

%package client
Summary: CUPS printing system - client programs
License: GPLv2
Requires: %{name}-libs%{?_isa} = %{epoch}:%{version}-%{release}
Provides: /usr/bin/lpq /usr/bin/lpr /usr/bin/lp /usr/bin/cancel /usr/bin/lprm /usr/bin/lpstat
Requires: /usr/sbin/alternatives
Provides: lpr

%description client
CUPS printing system provides a portable printing layer for
UNIX® operating systems. This package contains command-line client
programs.

%package devel
Summary: CUPS printing system - development environment
License: LGPLv2
Requires: %{name}-libs%{?_isa} = %{epoch}:%{version}-%{release}
Requires: gnutls-devel krb5-devel zlib-devel

%description devel
CUPS is the standards-based, open source printing system developed by Apple Inc.
for macOS® and other UNIX®-like operating systems. Developers can use this development
package to develop other printer drivers.

%package libs
Summary: CUPS libs
License: LGPLv2 and zlib

%description libs
CUPS printing system provides a portable printing layer for
UNIX® operating systems. It has been developed by Apple Inc.
to promote a standard printing solution for all UNIX vendors and users.
CUPS provides the System V and Berkeley command-line interfaces.
The cups-libs package provides libraries used by applications to use CUPS
natively, without needing the lp/lpr commands.

%package filesystem
Summary: CUPS printing system - directory layout
BuildArch: noarch

%description filesystem
CUPS printing system provides a portable printing layer for
UNIX® operating systems. This package provides some directories which are
required by other packages that add CUPS drivers (i.e. filters, backends etc.).

%package lpd
Summary: CUPS printing system - lpd emulation
Requires: %{name}%{?_isa} = %{epoch}:%{version}-%{release}
Requires: %{name}-libs%{?_isa} = %{epoch}:%{version}-%{release}
Provides: lpd

%description lpd
CUPS printing system provides a portable printing layer for
UNIX® operating systems. This is the package that provides standard
lpd emulation.

%package ipptool
Summary: CUPS printing system - tool for performing IPP requests
Requires: %{name}-libs%{?_isa} = %{epoch}:%{version}-%{release}
Requires: avahi

%description ipptool
Sends IPP requests to the specified URI and tests and/or displays the results.

%package printerapp
Summary: CUPS printing system - tools for printer application
Requires: %{name}-libs%{?_isa} = %{epoch}:%{version}-%{release}
Requires: avahi

%description printerapp
Provides IPP everywhere printer application ippeveprinter and tools for printing
PostScript and HP PCL document formats - ippevepcl and ippeveps. The printer
application enables older printers for IPP everywhere standard - so if older printer
is installed with a printer application, its print queue acts as IPP everywhere printer
to CUPS daemon. This solution will substitute printer drivers and raw queues in the future.

%package    help
Summary:    Documents for cups
Buildarch:  noarch

%description    help
Man pages and other related documents.

%prep
%autosetup -n %{name}-%{version} -p1

sed -i -e '1iMaxLogSize 0' conf/cupsd.conf.in
sed -i -e 's,^ErrorLog .*$,ErrorLog syslog,' -i -e 's,^AccessLog .*$,AccessLog syslog,' -i -e 's,^PageLog .*,PageLog syslog,' conf/cups-files.conf.in
perl -pi -e "s,^.SILENT:,," Makedefs.in

aclocal -I config-scripts
autoconf -f -I config-scripts

%build
export DSOFLAGS="$DSOFLAGS -L../cgi-bin -L../filter -L../ppdc -L../scheduler -Wl,-z,relro -Wl,-z,now -specs=/usr/lib/rpm/generic-hardened-ld -Wl,-z,relro,-z,now -fPIE -pie" 
export CFLAGS="$RPM_OPT_FLAGS -fstack-protector-all -DLDAP_DEPRECATED=1"
# --enable-debug to avoid stripping binaries
%configure --with-docdir=%{_datadir}/%{name}/www --enable-debug \
    --enable-lspp \
    --with-exe-file-perm=0755 \
    --with-cupsd-file-perm=0755 \
    --with-log-file-perm=0600 \
    --enable-relro \
    --with-dbusdir=%{_sysconfdir}/dbus-1 \
    --enable-avahi \
    --enable-threads \
    --enable-gnutls \
    --enable-webif \
    --with-xinetd=no \
    --with-access-log-level=actions \
    --enable-page-logging \
    --with-rundir=%{_rundir}/cups \
    --enable-sync-on-close \
    localedir=%{_datadir}/locale

%make_build

%check
make check

%install
make BUILDROOT=${RPM_BUILD_ROOT} install

rm -rf  ${RPM_BUILD_ROOT}%{_initddir} ${RPM_BUILD_ROOT}%{_sysconfdir}/{init.d,rc?.d}
install -d ${RPM_BUILD_ROOT}%{_unitdir}

find ${RPM_BUILD_ROOT}%{_datadir}/cups/model -name "*.ppd" |xargs gzip -n9f

pushd ${RPM_BUILD_ROOT}%{_bindir}
for file in cancel lp lpq lpr lprm lpstat; do
    mv $file $file.cups
done

mv ${RPM_BUILD_ROOT}%{_sbindir}/lpc ${RPM_BUILD_ROOT}%{_sbindir}/lpc.cups
cd ${RPM_BUILD_ROOT}%{_mandir}/man1
for file in cancel lp lpq lpr lprm lpstat; do
    mv $file.1 $file-cups.1
done

mv ${RPM_BUILD_ROOT}%{_mandir}/man8/lpc.8 ${RPM_BUILD_ROOT}%{_mandir}/man8/lpc-cups.8
popd

install -d ${RPM_BUILD_ROOT}%{_datadir}/pixmaps ${RPM_BUILD_ROOT}%{_sysconfdir}/X11/sysconfig \
           ${RPM_BUILD_ROOT}%{_sysconfdir}/X11/applnk/System \
           ${RPM_BUILD_ROOT}%{_rpmconfigdir}/macros.d
install -p -m 644 %{SOURCE1} ${RPM_BUILD_ROOT}%{_datadir}/pixmaps
install -m 0644 %{SOURCE2} ${RPM_BUILD_ROOT}%{_rpmconfigdir}/macros.d

touch ${RPM_BUILD_ROOT}%{_sysconfdir}/cups/{printers,classes,client,subscriptions}.conf
touch ${RPM_BUILD_ROOT}%{_sysconfdir}/cups/lpoptions

install -d ${RPM_BUILD_ROOT}%{_datadir}/ppd

# Remove unshipped files.
rm -rf %{buildroot}%{_mandir}/cat? %{buildroot}%{_mandir}/*/cat?
rm -f %{buildroot}%{_datadir}/applications/cups.desktop
rm -rf %{buildroot}%{_datadir}/icons
# there are pdf-banners shipped with cups-filters (#919489)
rm -rf %{buildroot}%{_datadir}/cups/banners
rm -f %{buildroot}%{_datadir}/cups/data/testprint

install -d ${RPM_BUILD_ROOT}%{_tmpfilesdir}
cat > ${RPM_BUILD_ROOT}%{_tmpfilesdir}/cups.conf <<EOF
d /run/cups 0755 root lp -
d /run/cups/certs 0511 lp sys -
d /var/spool/cups/tmp - - - 30d
EOF

cat > ${RPM_BUILD_ROOT}%{_tmpfilesdir}/cups-lp.conf <<EOF
c /dev/lp0 0660 root lp - 6:0
c /dev/lp1 0660 root lp - 6:1
c /dev/lp2 0660 root lp - 6:2
c /dev/lp3 0660 root lp - 6:3
EOF

find ${RPM_BUILD_ROOT} -type f -o -type l | sed '
s:.*\('%{_datadir}'/\)\([^/_]\+\)\(.*\.po$\):%lang(\2) \1\2\3:
/^%lang(C)/d
/^\([^%].*\)/d
' > %{name}.lang

%post
%systemd_post %{name}.path %{name}.socket %{name}.service

install -d ${RPM_BUILD_ROOT}%{_localstatedir}/run/cups/certs

/bin/sed -i -e "s,^PageLogFormat,#PageLogFormat,i" %{_sysconfdir}/cups/cups-files.conf

%post client
/usr/sbin/alternatives --install %{_bindir}/lpr print %{_bindir}/lpr.cups 40 \
	 --slave %{_bindir}/lp print-lp %{_bindir}/lp.cups \
	 --slave %{_bindir}/lpq print-lpq %{_bindir}/lpq.cups \
	 --slave %{_bindir}/lprm print-lprm %{_bindir}/lprm.cups \
	 --slave %{_bindir}/lpstat print-lpstat %{_bindir}/lpstat.cups \
	 --slave %{_bindir}/cancel print-cancel %{_bindir}/cancel.cups \
	 --slave %{_sbindir}/lpc print-lpc %{_sbindir}/lpc.cups \
	 --slave %{_mandir}/man1/cancel.1.gz print-cancelman %{_mandir}/man1/cancel-cups.1.gz \
	 --slave %{_mandir}/man1/lp.1.gz print-lpman %{_mandir}/man1/lp-cups.1.gz \
	 --slave %{_mandir}/man8/lpc.8.gz print-lpcman %{_mandir}/man8/lpc-cups.8.gz \
	 --slave %{_mandir}/man1/lpq.1.gz print-lpqman %{_mandir}/man1/lpq-cups.1.gz \
	 --slave %{_mandir}/man1/lpr.1.gz print-lprman %{_mandir}/man1/lpr-cups.1.gz \
	 --slave %{_mandir}/man1/lprm.1.gz print-lprmman %{_mandir}/man1/lprm-cups.1.gz \
	 --slave %{_mandir}/man1/lpstat.1.gz print-lpstatman %{_mandir}/man1/lpstat-cups.1.gz || :
 
%post lpd
%systemd_post cups-lpd.socket
 
%ldconfig_scriptlets libs
 
%preun
%systemd_preun %{name}.path %{name}.socket %{name}.service
 
%preun client
if [ $1 -eq 0 ] ; then
	/usr/sbin/alternatives --remove print %{_bindir}/lpr.cups || :
fi
 
%preun lpd
%systemd_preun cups-lpd.socket

%postun
%systemd_postun_with_restart %{name}.path %{name}.socket %{name}.service
 
%postun lpd
%systemd_postun_with_restart cups-lpd.socket

%triggerin -- samba-client
ln -sf %{_libexecdir}/samba/cups_backend_smb %{_exec_prefix}/lib/cups/backend/smb || :
exit 0

%triggerun -- samba-client
[ $2 = 0 ] || exit 0
rm -f %{_exec_prefix}/lib/cups/backend/smb

%files -f %{name}.lang
%{_bindir}/cupstestppd
%{_bindir}/ppd*
%{_sbindir}/*
# client subpackage
%exclude %{_sbindir}/lpc.cups
%dir %{cups_serverbin}/daemon
%{cups_serverbin}/daemon/cups-deviced
%{cups_serverbin}/daemon/cups-driverd
%{cups_serverbin}/daemon/cups-exec
%{cups_serverbin}/backend/*
%{cups_serverbin}/cgi-bin
%{cups_serverbin}/filter/*
%{cups_serverbin}/monitor
%{cups_serverbin}/notifier
%{_datadir}/cups/drv/sample.drv
%{_datadir}/cups/examples
%{_datadir}/cups/mime/mime.types
%{_datadir}/cups/mime/mime.convs
%{_datadir}/cups/ppdc/*.defs
%{_datadir}/cups/ppdc/*.h
%dir %{_datadir}/cups/templates
%{_datadir}/cups/templates/*.tmpl
%dir %{_datadir}/cups/templates/de
%{_datadir}/cups/templates/de/*.tmpl
%dir %{_datadir}/cups/templates/da
%{_datadir}/cups/templates/da/*.tmpl
%dir %{_datadir}/cups/templates/es
%{_datadir}/cups/templates/es/*.tmpl
%dir %{_datadir}/cups/templates/fr
%{_datadir}/cups/templates/fr/*.tmpl
%dir %{_datadir}/cups/templates/ja
%{_datadir}/cups/templates/ja/*.tmpl
%dir %{_datadir}/cups/templates/pt_BR
%{_datadir}/cups/templates/pt_BR/*.tmpl
%dir %{_datadir}/cups/templates/ru
%{_datadir}/cups/templates/ru/*.tmpl
%dir %{_datadir}/%{name}/usb
%{_datadir}/%{name}/usb/org.cups.usb-quirks
%dir %{_datadir}/%{name}/www
%{_datadir}/%{name}/www/images
%{_datadir}/%{name}/www/*.css
%dir %{_datadir}/%{name}/www/de
%dir %{_datadir}/%{name}/www/da
%dir %{_datadir}/%{name}/www/es
%dir %{_datadir}/%{name}/www/fr
%dir %{_datadir}/%{name}/www/ja
%dir %{_datadir}/%{name}/www/pt_BR
%dir %{_datadir}/%{name}/www/ru
%{_datadir}/pixmaps/cupsprinter.png
%dir %attr(1770,root,lp) %{_localstatedir}/spool/cups/tmp
%dir %attr(0710,root,lp) %{_localstatedir}/spool/cups
%dir %attr(0755,root,lp) %{_localstatedir}/log/cups
# client subpackage
%exclude %{_mandir}/man1/lp*.1.gz
%exclude %{_mandir}/man1/cancel-cups.1.gz
%exclude %{_mandir}/man8/lpc-cups.8.gz
# devel subpackage
%exclude %{_mandir}/man1/cups-config.1.gz
# ipptool subpackage
%exclude %{_mandir}/man1/ipptool.1.gz
%exclude %{_mandir}/man5/ipptoolfile.5.gz
# lpd subpackage
%exclude %{_mandir}/man8/cups-lpd.8.gz
# printerapp
%exclude %{_mandir}/man1/ippeveprinter.1.gz
%exclude %{_mandir}/man7/ippevepcl.7.gz
%exclude %{_mandir}/man7/ippeveps.7.gz
%dir %attr(0755,root,lp) %{_rundir}/cups
%dir %attr(0511,lp,sys) %{_rundir}/cups/certs
%dir %attr(0755,root,lp) %{_sysconfdir}/cups
%attr(0640,root,lp) %{_sysconfdir}/cups/cupsd.conf.default
%verify(not md5 size mtime) %config(noreplace) %attr(0640,root,lp) %{_sysconfdir}/cups/cupsd.conf
%verify(not md5 size mtime) %config(noreplace) %attr(0640,root,lp) %{_sysconfdir}/cups/cups-files.conf
%attr(0640,root,lp) %{_sysconfdir}/cups/cups-files.conf.default
%verify(not md5 size mtime) %config(noreplace) %attr(0644,root,lp) %{_sysconfdir}/cups/client.conf
%verify(not md5 size mtime) %config(noreplace) %attr(0600,root,lp) %{_sysconfdir}/cups/classes.conf
%verify(not md5 size mtime) %config(noreplace) %attr(0600,root,lp) %{_sysconfdir}/cups/printers.conf
%verify(not md5 size mtime) %config(noreplace) %attr(0644,root,lp) %{_sysconfdir}/cups/snmp.conf
%attr(0640,root,lp) %{_sysconfdir}/cups/snmp.conf.default
%verify(not md5 size mtime) %config(noreplace) %attr(0640,root,lp) %{_sysconfdir}/cups/subscriptions.conf
%verify(not md5 size mtime) %config(noreplace) %attr(0644,root,lp) %{_sysconfdir}/cups/lpoptions
%dir %attr(0755,root,lp) %{_sysconfdir}/cups/ppd
%dir %attr(0700,root,lp) %{_sysconfdir}/cups/ssl
%config(noreplace) %{_sysconfdir}/dbus-1/system.d/cups.conf
%config(noreplace) %{_sysconfdir}/pam.d/cups
%{_tmpfilesdir}/cups.conf
%{_tmpfilesdir}/cups-lp.conf
%attr(0644, root, root)%{_unitdir}/%{name}.service
%attr(0644, root, root)%{_unitdir}/%{name}.socket
%attr(0644, root, root)%{_unitdir}/%{name}.path
 
%files client
%{_bindir}/cancel*
%{_bindir}/lp*
%{_sbindir}/lpc.cups
%{_mandir}/man1/cancel-cups.1.gz
%{_mandir}/man1/lp*.1.gz
%{_mandir}/man8/lpc-cups.8.gz
 
%files libs
%{license} LICENSE
%{license} NOTICE
%{_libdir}/libcups.so.2
%{_libdir}/libcupsimage.so.2
 
%files filesystem
%dir %{cups_serverbin}
%dir %{cups_serverbin}/backend
%dir %{cups_serverbin}/driver
%dir %{cups_serverbin}/filter
%dir %{_datadir}/cups
%dir %{_datadir}/cups/data
%dir %{_datadir}/cups/drv
%dir %{_datadir}/cups/mime
%dir %{_datadir}/cups/model
%dir %{_datadir}/cups/ppdc
%dir %{_datadir}/ppd
 
%files devel
%{_bindir}/cups-config
%{_includedir}/cups
%{_libdir}/*.so
%{_rpmconfigdir}/macros.d/macros.cups
%{_prefix}/lib/pkgconfig/cups.pc
 
%files lpd
%{cups_serverbin}/daemon/cups-lpd
%{_mandir}/man8/cups-lpd.8.gz
%attr(0644, root, root)%{_unitdir}/cups-lpd.socket
%attr(0644, root, root)%{_unitdir}/cups-lpd@.service
 
%files ipptool
%{_bindir}/ippfind
%{_bindir}/ipptool
%dir %{_datadir}/cups/ipptool
%{_datadir}/cups/ipptool/*
%{_mandir}/man1/ipptool.1.gz
%{_mandir}/man5/ipptoolfile.5.gz
 
%files printerapp
%{_bindir}/ippeveprinter
%dir %{cups_serverbin}/command
%{cups_serverbin}/command/ippevepcl
%{cups_serverbin}/command/ippeveps
%{_mandir}/man1/ippeveprinter.1.gz
%{_mandir}/man7/ippevepcl.7.gz
%{_mandir}/man7/ippeveps.7.gz

%files help
%{_mandir}/man[1578]/*
%{_mandir}/man1/cups-config.1.gz
%doc README.md CREDITS.md CHANGES.md
%doc %{_datadir}/%{name}/www/index.html
%doc %{_datadir}/%{name}/www/help
%doc %{_datadir}/%{name}/www/robots.txt
%doc %{_datadir}/%{name}/www/de/index.html
%doc %{_datadir}/%{name}/www/da/index.html
%doc %{_datadir}/%{name}/www/es/index.html
%doc %{_datadir}/%{name}/www/fr/index.html
%doc %{_datadir}/%{name}/www/ja/index.html
%doc %{_datadir}/%{name}/www/ru/index.html
%doc %{_datadir}/%{name}/www/pt_BR/index.html
%doc %{_datadir}/%{name}/www/apple-touch-icon.png

%changelog
* Thu May 22 2025 guoshengbang <guoshengbang@huawei.com> - 2.4.12-12
- DESC:update to cups-2.4.12

* Wed Jun 12 2024 baiguo <baiguo@kylinos.cn> - 1:2.4.0-11
- fix CVE-2024-35235

* Fri Sep 22 2023 zhouwenpei <zhouwenpei1@h-partners.com> - 1:2.4.0-10
- fix CVE-2023-4504

* Wed Jul 19 2023 haomimi <haomimi@uniontech.com> - 1:2.4.0-9
- DESC:The license is changed to apache 2.0

* Mon Jun 26 2023 zhouwenpei <zhouwenpei@h-partners.com> - 1:2.4.0-8
- fix CVE-2023-34241

* Fri Jun 9 2023 zhangpan <zhangpan103@h-partners.com> - 1:2.4.0-7
- fix build error

* Sat Jun 3 2023 zhouwenpei <zhouwenpei@h-partners.com> - 1:2.4.0-6
- fix CVE-2023-32324

* Tue Feb 21 2023 zhouwenpei <zhouwenpei@h-partners.com> - 2.4.0-5
- fix update conflict of devel and help

* Wed Jun 15 2022 hanhui <hanhui15@h-partners.com> - 2.4.0-4
- Remove legacy code for RIP_MAX_CACHE environment variable

* Thu Jun 9 2022 hanhui <hanhui15@h-partners.com> - 2.4.0-3
- fix CVE-2022-26691

* Thu May 12 2022 zhanzhimin <zhanzhimin@h-partners.com> - 2.4.0-2
- fix official website address

* Thu Dec 9 2021 hanhui <hanhui15@huawei.com> - 2.4.0-1
- DESC:update to cups-2.4.0

* Thu Nov 04 2021 wangkerong <wangkerong@huawei.com> - 2.3.3-8
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:fix cups-devel requires error

* Fri Sep 24 2021 liuyumeng <liuyumeng5@huawei.com> - 2.3.3-7
- Type:CVE
- ID:CVE-2020-10001
- SUG:NA
- DESC:Fix CVE-2020-10001

* Tue Jun 08 2021 wangkerong <wangkeorng@huawei.com> - 2.3.3-6
- Type:CVE
- ID:CVE-2021-25317
- SUG:NA
- DESC:Fix CVE-2021-25317

* Tue May 25 2021 liuyumeng <liuyumeng@huawei.com> - 2.3.3-5
- remove unused ncp backend

* Mon May 24 2021 liuyumeng <liuyumeng@huawei.com> - 2.3.3-4
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:Compilation optimization

* Tue Sep 29 2020 hanhui <hanhui15@huawei.com> - 2.3.3-3
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:fix build bug

* Wed Sep 9 2020 hanhui <hanhui15@huawei.com> - 2.3.3-2
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:modify source url

* Mon Jul 20 2020 wangye <wang70@huawei.com> - 2.3.3-1
- Type:enhancement
- ID:NA
- SUG:NA
- DESC:upgrade to 2.3.3

* Fri Jun 12 2020 hanhui <hanhui15@huawei.com> - 2.2.13-1
- Type:enhancement
- ID:NA
- SUG:NA
- DESC:upgrade to 2.2.13

* Thu Mar 26 2020 gaihuiying <gaihuiying1@huawei.com> - 2.2.8-9
- Type:cves
- ID:CVE-2019-2228
- SUG:restart
- DESC:fix CVE-2019-2228

* Sat Jan 11 2020 openEuler Buildteam <buildteam@openeuler.org> - 2.2.8-8
- Type:enhancement
- ID:NA
- SUG:NA
- DESC: delete patches

* Wed Sep 25 2019 gaoguanghui <gaoguanghui1@huawei.com> - 2.2.8-7
- Type:cves
- ID:CVE-2019-8675 CVE-2019-8696
- SUG:restart
- DESC:fix CVE-2019-8675 CVE-2019-8696

* Wed Sep 18 2019 Guan Yanjie <guanyanjie@huawei.com> - 2.2.8-6
- Package init
