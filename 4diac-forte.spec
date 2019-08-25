Name:     4diac-forte
Version:  1.9.0
Release:  1%{?dist}
Summary:  IEC 61499 runtime environment
License:  EPL
URL:      http://eclipse.org/4diac
Source0:  https://git.eclipse.org/c/4diac/org.eclipse.4diac.forte.git/snapshot/org.eclipse.4diac.forte.git-%{version}.tar.gz

%{?systemd_requires}
BuildRequires: cmake, gcc-c++
BuildRequires: systemd

%if %{with opcua}
BuildRequires: open62541-devel >= 0.2
%endif

%if %{with lua}
BuildRequires: lua-devel >= 5.1
%endif

%description
The 4DIAC runtime environment (4DIAC-RTE, FORTE) is a small portable
implementation of an IEC 61499 runtime environment targeting small
embedded control devices (16/32 Bit), implemented in C++. It supports
online-reconfiguration of its applications and the real-time capable
execution of all function block types provided by the IEC 61499 standard.

%prep
%setup -q

%build
mkdir -p bin/posix
cd bin/posix
%cmake -DFORTE_ARCHITECTURE=Posix \
       -DFORTE_COM_ETH=ON \
       -DFORTE_COM_FBDK=ON \
       -DFORTE_COM_LOCAL=ON \
       %{?_with_opcua: -DFORTE_COM_OPC_UA=ON -DFORTE_COM_OPC_UA_INCLUDE_DIR=%{_includedir}/open62541 -DFORTE_COM_OPC_UA_LIB_DIR=%{_libdir}} \
       -DFORTE_MODULE_CONVERT=ON \
       -DFORTE_MODULE_IEC61131=ON \
       %{?_with_sysfs: -DFORTE_MODULE_SysFs=ON } \
       -DFORTE_MODULE_UTILS=ON \
       %{?_with_lua: -DFORTE_USE_LUATYPES=Lua } \
       -DFORTE_TESTS=OFF \
       ../..
make %{?_smp_mflags}

%install
mkdir -p %{buildroot}%{_unitdir}
install -p systemd/4diac-forte.service %{buildroot}%{_unitdir}

mkdir -p %{buildroot}%{_sysconfdir}/sysconfig
install -p systemd/4diac-forte-sysconfig %{buildroot}%{_sysconfdir}/sysconfig/4diac-forte

cd bin/posix
%make_install

%post
%systemd_post 4diac-forte.service

%preun
%systemd_preun 4diac-forte.service

%postun
%systemd_postun_with_restart 4diac-forte.service

%files
%license epl-20.html
%{_bindir}/forte
%{_unitdir}/4diac-forte.service
%{_sysconfdir}/sysconfig/4diac-forte

%changelog
* Fri Sep 01 2017 Jens Reimann <jreimann@redhat.com> - 1.9.0-1
- Initial version of the package

