# 
# Do NOT Edit the Auto-generated Part!
# Generated by: spectacle version 0.25
# 

Name:       libmlocale

# >> macros
# << macros

Summary:    Contains classes MLocale and friends originally from libmeegotouch
Version:    0.5.0
Release:    1
Group:      System/Libraries
License:    LGPLv2
URL:        https://meego.gitorious.org/meegotouch/libmlocale
Source0:    %{name}-%{version}.tar.bz2
Source100:  libmlocale.yaml
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  pkgconfig(icu-uc)
BuildRequires:  pkgconfig(QtCore)
BuildRequires:  gconf-devel

%description
Contains classes MLocale and friends originally from libmeegotouch. 
With that separation one can use those localization facilities without 
having to bring on board also the MeeGo Touch GUI toolkit.


%package devel
Summary:    Devel files for limlocale
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}

%description devel
%{summary}.

%package tests
Summary:    Tests for limlocale
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}

%description tests
%{summary}.

%package benchmarks
Summary:    Benchmarks files for limlocale
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}

%description benchmarks
%{summary}.


%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
export QT_SELECT=4
# << build pre

%configure --disable-static
make %{?jobs:-j%jobs}

# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
export QT_SELECT=4
# << install pre
%make_install

# >> install post
# << install post


%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%doc LICENSE.LGPL
%{_libdir}/*.so.*
%{_datadir}/mlocale/icu
# >> files
# << files

%files devel
%defattr(-,root,root,-)
%{_datadir}/qt4/mkspecs/features/*.prf
%{_libdir}/*.so
%{_libdir}/*.prl
%{_includedir}/mlocale/*.h
%{_includedir}/mlocale/M*
%{_libdir}/pkgconfig/*.pc
# >> files devel
# << files devel

%files tests
%defattr(-,root,root,-)
%{_libdir}/libmlocale-tests
%{_datadir}/libmlocale-tests/tests.xml
# >> files tests
# << files tests

%files benchmarks
%defattr(-,root,root,-)
%{_libdir}/libmlocale-benchmarks
%{_datadir}/libmlocale-benchmarks/tests.xml
# >> files benchmarks
# << files benchmarks
