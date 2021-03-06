Name:       bps-bp-manager-tizen
Summary:    BestPractice Tizen native API
Version:    0.0.1
Release:    0
Group:      System/API
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
Source1:    bps-bp-manager-tizen.service
Source1001:     bps-bp-manager-tizen.manifest
BuildRequires:  cmake
BuildRequires:  tidl
BuildRequires:  pkgconfig(rpc-port)
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(bundle)
BuildRequires:  pkgconfig(dbus-1)
BuildRequires:  pkgconfig(eina)
BuildRequires:  pkgconfig(ecore)
#BuildRequires:  pkgconfig(gmock)
%if 0%{?gcov:1}
BuildRequires:  lcov
BuildRequires:  zip
%endif

%description
An best practice tizen native library in Tizen C API

%package devel
Summary:  An best practice tizen native library in Tizen C API (Development)
Group:    System/API
Requires: %{name} = %{version}-%{release}

%description devel
An best practice tizen native library in Tizen C API (Development) package.

#################################################
# bps-bp-manager-tizen-unittests
#################################################
%package -n bps-bp-manager-tizen-unittests
Summary:    GTest for bp-tizen API
Group:      Development/Libraries
Requires:   %{name}

%description -n bps-bp-manager-tizen-unittests
GTest for bp-tizen API

#################################################
# bps-bp-manager-tizen-gcov
#################################################
%if 0%{?gcov:1}
%package gcov
Summary:    Best practice tizen native API(gcov)
Group:      Application Framework/Testing

%description gcov
Best practice tizen gcov objects
%endif

#################################################
# bps-bp-manager-tizen build
#################################################

%prep
%setup -q
cp %{SOURCE1001} .
tidlc -p -l C -i ./src/tidl/remote_service.tidl -o remote_service_proxy
mv remote_service_proxy.c src/
mv remote_service_proxy.h src/ 
tidlc -s -l C -i ./src/tidl/stub_service.tidl -o stub_service_stub
mv stub_service_stub.c src/ 
mv stub_service_stub.h src/

%build
%if 0%{?gcov:1}
export CFLAGS+=" -fprofile-arcs -ftest-coverage"
export CXXFLAGS+=" -fprofile-arcs -ftest-coverage"
export FFLAGS+=" -fprofile-arcs -ftest-coverage"
export LDFLAGS+=" -lgcov"
%endif

MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
%cmake . -DFULLVER=%{version} -DMAJORVER=${MAJORVER} \
	-DBIN_INSTALL_DIR:PATH=%{_bindir}
#	-DTZ_SYS_ETC=%{TZ_SYS_ETC} 
#	-D_APPFW_FEATURE_ALARM_MANAGER_MODULE_LOG:BOOL=${_APPFW_FEATURE_ALARM_MANAGER_MODULE_LOG} \
#	-DALARM_CONF_DIR=%{_datadir}/alarm-manager \
#	-DBUILD_GTESTS=%{?gtests:1}%{!?gtests:0} \
#	-DBUILD_GCOV=%{?gcov:1}%{!?gcov:0} \

%__make %{?jobs:-j%jobs}

%if 0%{?gcov:1}
mkdir -p gcov-obj
find . -name '*.gcno' -exec cp '{}' gcov-obj ';'
%endif

%check
ctest --output-on-failure %{?_smp_mflags}
%if 0%{?gcov:1}
lcov -c --ignore-errors graph --no-external -q -d . -o bp-tizen.info
genhtml bp-tizen.info -o bp-tizen.out
zip -r bp-tizen.zip bp-tizen.out bp-tizen.info
install -m 0644 bp-tizen.zip %{buildroot}%{_datadir}/gcov/
%endif

%install
rm -rf %{buildroot}
%make_install

mkdir -p %{buildroot}%{_unitdir}/multi-user.target.wants
mkdir -p %{buildroot}%{_unitdir_user}/sockets.target.wants

# TODO(vincent): Need to copy service file for systemd
#################################################
# Install 
# packaging/bps-bp-manager-tizen.service
# /usr/lib/systemd/system/bps-bp-manager-tizen.service
#################################################
install -m 0644 %SOURCE1 %{buildroot}%{_unitdir}/bps-bp-manager-tizen.service

#################################################
# Install 
# packaging/bps-bp-manager-tizen.service for auto launch
# /usr/lib/systemd/system/multi-user.target.wants/bps-bp-manager-tizen.service
#################################################
ln -s bps-bp-manager-tizen.service %{buildroot}%{_unitdir}/multi-user.target.wants/bps-bp-manager-tizen.service


#################################################
# Install 
# bps-bp-manager.conf needed (missing)
# /usr/lib/tmpfiles.d/alarm-manager.conf
#################################################
# TODO(vincent): missing this module's conf file (aka alarm-manager.conf)
# install -m 0644 %source4 /buildroot/tmpfilesdir/alarm-manager.conf 
# /usr/lib/tmpfiles.d/alarm-manager.conf
# [contents]
# d /run/alarm_agent 0777 root users -
# d /var/log/appfw/alarmmgr_log 0775 app_fw app_fw -

# TODO(vincent): if necessary udev register
# mkdir %{buildroot}%{_libdir}/udev/rules.d
# /lib/udev/rules.d/99-rtc.rules

%if 0%{?gcov:1}
mkdir -p %{buildroot}%{_datadir}/gcov/obj
install -m 0644 gcov-obj/* %{buildroot}%{_datadir}/gcov/obj
%endif

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

echo %{_sysconfdir}

#rm -f src/remote_service_proxy.c
#rm -f src/remote_service_stub.c
#rm -f src/remote_service_proxy.h
#rm -f src/remote_service_stub.h

# TODO(vincent): what is -n option?
%files -n bps-bp-manager-tizen

# TODO(vincent): what is this manifest do? SMACK?
%manifest bps-bp-manager-tizen.manifest

# TODO(vincent): _bindir is /usr/bin. This put the exec file.
#                May cmake create this in /usr/bin (may make install?)
%{_bindir}/bps-bp-manager-tizen*

#################################################
# packaging/bps-bp-manager-tizen.service
# /usr/lib/systemd/system/bps-bp-manager-tizen.service
# purpose: systemd service info
#################################################
# TODO(vincent): _unitdir is /usr/lib/systemd/systemd
# TODO(vincent): Purpose: All services must be defined in this folder to be handled by systemd.
#                Line 115 install it
%attr(0644,root,root) %{_unitdir}/bps-bp-manager-tizen.service

#################################################
# packaging/bps-bp-manager-tizen.service
# /usr/lib/systemd/system/bps-bp-manager-tizen.service at boot
# purpose: systemd service info auto boot
#################################################
# TODO(vincent): Add to /usr/lib/systemd/systemd/multi-user.target.wants to start this service at boot.
%{_unitdir}/multi-user.target.wants/bps-bp-manager-tizen.service

#################################################
# src/org.tizen.bp.manager.tizen.service.in
# /usr/share/dbus-1/system-service/org.tizen.bp.manager.tizen.service
# purpose: d-bus service info used for DBus launch
#################################################
# TODO(vincent): {_datadir} is /usr/share and it describes d-bus service name, systemservice info
#                 Why we need it? for what? why it is org.tizen.xxx format?
#                 How /usr/share/dbus-1/system-service/org.tizen.bp.manager.tizen.service installed? in src/CMakeFiles.txt
#                 INSTALL(FILES ${DBUS_INTERFACE}.service DESTINATION ${SHARE_INSTALL_PREFIX}/dbus-1/system-services/)
#                 if applications want to be activated on the system and session buses, then service files
#                 should be installed 
# [Contents]
#
# [D-BUS Service]
#  Name=@DBUS_INTERFACE@
#  Exec=/bin/false
#  /usr/lib/systemd/system/bps-bp-manager-tizen.service
#  SystemdService=@SERVER@.service
%attr(0644,root,root) %{_datadir}/dbus-1/system-services/org.tizen.bp.manager.tizen.service

%license LICENSE

#################################################
# src/bps-bp-manager-tizen.conf.in
# /etc/dbus-1/system.d/bps-bp-manager-tizen.conf
# purpose: allow, deny DBus API set
#################################################
# TODO(vincent): Allow file
# dbus-daemon --fork --config-file=/etc/dbus-1/system.d/bps-bp-manager-tizen.conf
%config %{_sysconfdir}/dbus-1/system.d/bps-bp-manager-tizen.conf

#%post -n bps-bp-manager-tizen-unittests
#%if 0%{?gcov:1}
#%{_bindir}/bp-manager-tizen_unittests
#%endif

#################################################
# bps-bp-manager-tizen-unittests
#################################################
#%files -n bps-bp-manager-tizen-unittests
#%{_bindir}/bp-manager-tizen_unittests

#################################################
# bps-bp-manager-tizen-gcov
#################################################
%if 0%{?gcov:1}
%files gcov
%{_datadir}/gcov/*
%endif
