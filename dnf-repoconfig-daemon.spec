Name:           dnf-repoconfig-daemon
Version:        1.0.0
Release:        1%{?dist}
Summary:        D-bus daemon for repository management

License:        GPLv2+
URL:            https://github.com/m-blaha/dnf-repoconfig-daemon
Source0:        %{url}/archive/%{version}.tar.gz

BuildRequires:  cmake >= 3.12
BuildRequires:  gcc-c++
BuildRequires:  pkgconfig(sdbus-c++)
BuildRequires:  systemd-rpm-macros


%description
Experimental d-bus daemon to list / enable / disable repositories.

%prep
%autosetup -n %{name}-%{version}

%build
%cmake .
%make_build

%install
%make_install

%files
%{_bindir}/%{name}
%{_unitdir}/dnf-repoconfig-daemon.service
%{_sysconfdir}/dbus-1/system.d/org.rpm.dnf.v1.conf
%{_datadir}/dbus-1/system-services/org.rpm.dnf.v1.rpm.RepoConf.service

%post
%systemd_post %{name}.service

%postun
%systemd_postun %{name}.service

%preun
%systemd_preun %{name}.service


%changelog
* Wed Mar 18 2020 Marek Blaha <mblaha@redhat.com> - 1.0.0-1
- Initial release
