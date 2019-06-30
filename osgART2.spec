#
# spec file for package OpenSceneGraph
#
# Copyright (c) 2013 SUSE LINUX Products GmbH, Nuernberg, Germany.
#
# All modifications and additions to the file contributed by third parties
# remain the property of their copyright owners, unless otherwise agreed
# upon. The license for this file, and modifications and additions to the
# file, is the same license as for the pristine package itself (unless the
# license for the pristine package is not an Open Source License, in which
# case the license is the MIT License). An "Open Source License" is a
# license that conforms to the Open Source Definition (Version 1.9)
# published by the Open Source Initiative.

# Please submit bugfixes or comments via http://bugs.opensuse.org/
#

Name:           osgART2
Version:        2.0.RC4
Release:        7.1


Summary:        Adapter to use the ARToolKit inside OpenSceneGraph
License:        GPL-3 
Packager:	Alessandro de Oliveira Faria (A.K.A CABELO) <cabelo@opensuse.org>
Group:          Productivity/Graphics/Other
Url:            http://www.osgart.org/index.php/Main_Page
Source:         %{name}-%{version}.tar.gz
BuildRequires: -post-build-checks -rpmlint-Factory -rpmlint-mini  -rpmlint
BuildRequires: cmake
BuildRequires: gcc-c++
BuildRequires: freeglut-devel Mesa-libGL-devel doxygen
BuildRequires: gstreamer-0_10-devel glu-devel gstreamer-0_10-plugins-good 
BuildRequires: libOpenSceneGraph-devel == 3.2.1 
BuildRequires: libOpenThreads-devel == 3.2.1
BuildRequires: OpenSceneGraph-plugins == 3.2.1 
BuildRequires: libOpenSceneGraph-devel == 3.2.1
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
Requires: libosgART2
Requires: OpenSceneGraph == 3.2.1  
Requires: OpenSceneGraph-plugins == 3.2.1
Requires: libxine2
Requires: gstreamer-0_10

%description
OSGART is a library that simplifies the development of Augmented Reality or Mixed Reality applications by combining the well-known ARToolKit tracking library with OpenSceneGraph. But rather than acting just as a simple nodekit, the library offers 3 main functionalities: high level integration of video input (video object, shaders), spatial registration (marker-based, multiple trackers), and photometric registration (occlusion, shadow).


%package -n lib%{name}
Summary:        Development files for using the osgART2 library
Group:          Development/Libraries/C and C++

%description -n lib%{name}
OSGART is a library that simplifies the development of Augmented Reality or Mixed Reality applications by combining the well-known ARToolKit tracking library with OpenSceneGraph. But rather than acting just as a simple nodekit, the library offers 3 main functionalities: high level integration of video input (video object, shaders), spatial registration (marker-based, multiple trackers), and photometric registration (occlusion, shadow).

%package devel
Summary:        Development files for using the OpenCV library
Group:          Development/Libraries/C and C++
Requires:       lib%{name}

%description devel
OSGART is a library that simplifies the development of Augmented Reality or Mixed Reality applications by combining the well-known ARToolKit tracking library with OpenSceneGraph. But rather than acting just as a simple nodekit, the library offers 3 main functionalities: high level integration of video input (video object, shaders), spatial registration (marker-based, multiple trackers), and photometric registration (occlusion, shadow).

%prep
%setup -q
#%patch0 -p1

%build
#export CFLAGS="%{optflags} -fno-strict-aliasing -D__STDC_CONSTANT_MACROS -UGTK_DISABLE_DEPRECATED"
#export CXXFLAGS="%{optflags} -fno-strict-aliasing -D__STDC_CONSTANT_MACROS -UGTK_DISABLE_DEPRECATED"
cd ../ARToolKit
./Configure<keyboard
make %{?_smp_mflags}
cd -
cd build
cmake ..\
	-DCMAKE_INSTALL_PREFIX=%{_prefix} \
	-DCMAKE_CXX_FLAGS=-fpermissive \
	-DCMAKE_MODULE_LINKER_FLAGS=-lgstreamer-0.10 \
	-DCMAKE_SHARED_LINKER_FLAGS=-lgstreamer-0.10
make %{?_smp_mflags}

%install
cd build
make DESTDIR=%{buildroot} install
mkdir -p %{buildroot}/usr/local/bin/data
mkdir -p %{buildroot}/usr/share/applications/
mkdir -p %{buildroot}/usr/share/pixmaps/
mkdir -p %{buildroot}%{_libdir}/osgPlugins-3.2.1
mv %{buildroot}/usr/lib/osgPlugins-3.2.1/* %{buildroot}%{_libdir}/osgPlugins-3.2.1/
mv %{buildroot}/usr/bin/* %{buildroot}/usr/local/bin/
mv %{buildroot}/usr/lib/lib* %{buildroot}%{_libdir}
mv $RPM_SOURCE_DIR/patt.openSUSE %{buildroot}/usr/local/bin/data/
mv $RPM_SOURCE_DIR/patt* %{buildroot}/usr/share/osgART/patterns/
mv $RPM_SOURCE_DIR/osgartviewer.cfg %{buildroot}/usr/local/bin/
mv $RPM_SOURCE_DIR/steve.ive %{buildroot}/usr/local/bin/
mv $RPM_SOURCE_DIR/OSAR %{buildroot}/usr/local/bin/
mv $RPM_SOURCE_DIR/osar.desktop %{buildroot}/usr/share/applications/osar.desktop
mv $RPM_SOURCE_DIR/OSARico.png %{buildroot}/usr/share/pixmaps/OSARico.png
chmod 755 %{buildroot}/usr/local/bin/OSAR
#install -dm 755 %{buildroot}%{_libdir}/pkgconfig
#install -m 644 packaging/pkgconfig/*.pc \
#	%{buildroot}%{_libdir}/pkgconfig


%post -n lib%{name} -p /sbin/ldconfig

%postun -n lib%{name} -p /sbin/ldconfig


%files
%defattr(-,root,root)
#%doc AUTHORS.txt LICENSE.txt README.txt ChangeLog
#%{_prefix}/local/bin/*
%{_prefix}/share/osgART/*
%{_prefix}/share/osgART/patterns/*
%{_prefix}/share/applications/*
%{_prefix}/share/pixmaps/*
%{_prefix}/local/bin/osgartpick
%{_prefix}/local/bin/osgartsimple
%{_prefix}/local/bin/osgartsimplestats
%{_prefix}/local/bin/osgartvideobackground
%{_prefix}/local/bin/osgartviewer
%{_prefix}/local/bin/osgartviewer.cfg
%{_prefix}/local/bin/steve.ive
%{_prefix}/local/bin/OSAR
%{_prefix}/local/bin/data/*

%files -n lib%{name}
%defattr(-,root,root)
%{_libdir}/osgPlugins-3.2.1/*
%{_libdir}/lib*


%files devel
%defattr(-,root,root)
%{_includedir}/osgART/*

%changelog
* Mon Jun 26 2019 cabelo@opensuse.org
- New version and stable.
* Wed Apr  3 2013 cabelo@opensuse.org
- Created OpenSceneGraph-libgif6.patch
  Used Raymond Wooninck kdelibs patch as a template
