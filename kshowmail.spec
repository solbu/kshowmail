Name:    kshowmail
Summary: POP3 mail checker
Version: 4.1
Release: 1
License: GNU General Public License version 2.0 (GPLv2)                   
Group:   Productivity/Networking/Email/Clients
URL:     http://kshowmail.sourceforge.net/            
Source0: file://%{name}-%{version}.tar.bz2                      
BuildRoot: %{_tmppath}/%{name}-%{version}-build

#build dependencies
#------------------

#Suse
%if 0%{?suse_version}
%kde4_runtime_requires
%kde4_pimlibs_requires
%{?kde4_knotification_requires}
BuildRequires: libkde4-devel 
BuildRequires: libkdepim4-devel 
BuildRequires: libkdepimlibs4-devel
BuildRequires: update-desktop-files
%endif

#Fedora
%if 0%{?fedora_version}
BuildRequires: gcc-c++
BuildRequires: kdelibs-devel >= %{kde_version}
BuildRequires: kdepim-devel >= %{kde_version}
BuildRequires: kdepimlibs-devel >= %{kde_version}
BuildRequires: desktop-file-utils
%endif


%description
KShowmail is a POP3 mail checker for the KDE with these features: 
show number, size and more information about mails 
on pop3 servers in a list view, show the mail headers 
or complete mails, delete unwanted mail from server 
by configurable filters.                   

Authors:
--------
  Ulrich Weigelt <ulrich.weigelt@gmx.de>
  Eggert Ehmke <eggert.ehmke@berlin.de




%prep
%setup -q 

%build

# SUSE
#--------------------
%if 0%{?suse_version}

%cmake_kde4 -d build  
%make_jobs

%endif

# FEDORA
#----------------------
%if 0%{?fedora_version}

%cmake_kde4 .
make %{?_smp_mflags}

%endif


%install

# SUSE
#--------------------
%if 0%{?suse_version}

pushd build  
%kde4_makeinstall
popd

%suse_update_desktop_file %name Network Email
%find_lang %{name}
%kde_post_install

gzip $RPM_BUILD_ROOT/%{_datadir}/man/man1/*

%endif

# FEDORA
#----------------------
%if 0%{?fedora_version}

rm -rf %{buildroot}
make DESTDIR=%{buildroot} INSTALL="install -p" CP="cp -p" install

%find_lang %{name}

gzip $RPM_BUILD_ROOT/%{_datadir}/man/man1/*

%endif


#make desktop files




%clean
rm -fr %buildroot

%files -f %{name}.lang

%defattr(-,root,root)

#handbook
%if 0%{?suse_version}
%{_kde4_htmldir}/*/kshowmail
%endif
%if 0%{?fedora_version}
%{_kde4_docdir}/HTML/*/kshowmail
%endif

#the binary
%{_kde4_bindir}/kshowmail

#documents
%doc ChangeLog README

#the programm icon
%if 0%{?suse_version}
%_kde4_applicationsdir/kshowmail.desktop
%endif
%if 0%{?fedora_version}
%{_kde4_prefix}/share/applications/kde4
%endif


#all things in the apps dir
#all pictures, sounds, etc.
%{_kde4_appsdir}/kshowmail

#the desktop files of the config modules
%if 0%{?suse_version}
%{_kde4_servicesdir}/kshowmailconfigaccounts.desktop
%{_kde4_servicesdir}/kshowmailconfigactions.desktop
%{_kde4_servicesdir}/kshowmailconfigdisplay.desktop
%{_kde4_servicesdir}/kshowmailconfiggeneral.desktop
%{_kde4_servicesdir}/kshowmailconfigfilter.desktop
%{_kde4_servicesdir}/kshowmailconfiglog.desktop
%{_kde4_servicesdir}/kshowmailconfigspamcheck.desktop
%endif
%if 0%{?fedora_version}
%{_kde4_sharedir}/kde4/services/kshowmailconfigaccounts.desktop
%{_kde4_sharedir}/kde4/services/kshowmailconfigactions.desktop
%{_kde4_sharedir}/kde4/services/kshowmailconfigdisplay.desktop
%{_kde4_sharedir}/kde4/services/kshowmailconfiggeneral.desktop
%{_kde4_sharedir}/kde4/services/kshowmailconfigfilter.desktop
%{_kde4_sharedir}/kde4/services/kshowmailconfiglog.desktop
%{_kde4_sharedir}/kde4/services/kshowmailconfigspamcheck.desktop
%endif

#the libraries of the config modules
%if 0%{?suse_version}
%{_kde4_modulesdir}/kcm_kshowmailconfigaccounts.so
%{_kde4_modulesdir}/kcm_kshowmailconfigactions.so
%{_kde4_modulesdir}/kcm_kshowmailconfigdisplay.so
%{_kde4_modulesdir}/kcm_kshowmailconfigfilter.so
%{_kde4_modulesdir}/kcm_kshowmailconfiglog.so
%{_kde4_modulesdir}/kcm_kshowmailconfiggeneral.so
%{_kde4_modulesdir}/kcm_kshowmailconfigspamcheck.so
%endif
%if 0%{?fedora_version}
%{_kde4_libdir}/kde4/kcm_kshowmailconfigaccounts.so
%{_kde4_libdir}/kde4/kcm_kshowmailconfigactions.so
%{_kde4_libdir}/kde4/kcm_kshowmailconfigdisplay.so
%{_kde4_libdir}/kde4/kcm_kshowmailconfigfilter.so
%{_kde4_libdir}/kde4/kcm_kshowmailconfiglog.so
%{_kde4_libdir}/kde4/kcm_kshowmailconfiggeneral.so
%{_kde4_libdir}/kde4/kcm_kshowmailconfigspamcheck.so
%endif

#the icons
%{_datadir}/icons/hicolor/*x*/apps/%{name}.png

#the localization files
%{_datadir}/locale/de/LC_MESSAGES/kshowmail.mo
%{_datadir}/locale/it/LC_MESSAGES/kshowmail.mo
%{_datadir}/locale/es/LC_MESSAGES/kshowmail.mo
%{_datadir}/locale/nl/LC_MESSAGES/kshowmail.mo

#Manual
%{_datadir}/man/man1/*


%changelog

* Sun May 29 2011 4.1
- dutch and norwegian translation
- reply function in show mail dialog
- log shows manually deleted mails
- in the mail view and in the log view a tooltip shows the filter which has catched the mail
- You can set up an action for a double click on a mail list entry
- mail list: an icon in front of the sender shows whether the sender is listed on the whitelist
- Bugfix: Kshowmail has crashed while showing an empty mail.
- You can export and import the filter settings
- handbook was written by Otmar Mak (english and german) and our translators have translated it into spanish, italien and dutch
- Some providers need the sequence \r\n at the end of a transmission
- The column order of the views will be saved.

* Tue Jul 27 2010 4.0.1
- Bug fixed: KShowmail has chrashed when it has deleted mails.

* Mon Jul 05 2010 4.0
- First release for KDE 4

