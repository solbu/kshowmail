Name:    kshowmail
Summary: POP3 mail checker
Version: 4.0.1
Release: 1
License: GNU General Public License version 2.0 (GPLv2)                   
Group:   Productivity/Networking/Email/Clients
URL:     http://kshowmail.sourceforge.net/            
Source0: %{name}-%{version}.tar.gz                      
BuildRoot: %{_tmppath}/%{name}-%{version}-build
Requires: kdepimlibs4 libkdepim4 libkdepimlibs4
BuildRequires:  libkde4-devel libkdepim4-devel libkdepimlibs4-devel


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
%cmake_kde4 -d build  
make %{?_smp_mflags}


%install
cd build  
%makeinstall
cd ..

%kde_post_install

%find_lang %{name}

%clean
rm -fr %buildroot

%files lang -f %{name}.lang

%files 
%defattr(-,root,root)
%doc 
%{_datadir}/doc/kde/HTML/en/kshowmail/index.cache.bz2
%{_datadir}/doc/kde/HTML/en/kshowmail/index.docbook
%{_bindir}/kshowmail
%{_datadir}/applications/kde4/kshowmail.desktop
%{_datadir}/icons/hicolor/*x*/apps/%{name}.png
%dir %{_datadir}/kde4
%{_datadir}/kde4/apps/kshowmail/kshowmailui.rc
%{_datadir}/kde4/services/kshowmailconfigaccounts.desktop
%{_datadir}/kde4/services/kshowmailconfigactions.desktop
%{_datadir}/kde4/services/kshowmailconfigdisplay.desktop
%{_datadir}/kde4/services/kshowmailconfiggeneral.desktop
%{_datadir}/kde4/services/kshowmailconfigfilter.desktop
%{_datadir}/kde4/services/kshowmailconfiglog.desktop
%{_datadir}/kde4/services/kshowmailconfigspamcheck.desktop
%{_datadir}/kde4/apps/kshowmail/pics/accountActive.png
%{_datadir}/kde4/apps/kshowmail/pics/accountNotActive.png
%{_datadir}/kde4/apps/kshowmail/pics/button_whitelist.png
%{_datadir}/kde4/apps/kshowmail/pics/button_blacklist.png
%{_datadir}/kde4/apps/kshowmail/pics/mail-unread-new.png
%{_datadir}/kde4/apps/kshowmail/pics/account.svgz
%{_datadir}/kde4/apps/kshowmail/pics/trayIcon.png
%{_datadir}/kde4/apps/kshowmail/pics/newMailPic.png
%{_datadir}/kde4/apps/kshowmail/pics/deletedFilter.png
%{_datadir}/kde4/apps/kshowmail/pics/deletedManual.png
%{_datadir}/kde4/apps/kshowmail/pics/export.png
%{_datadir}/kde4/apps/kshowmail/pics/import.png
%{_datadir}/kde4/apps/kshowmail/pics/senderOnWhitelist.png

%{_datadir}/kde4/apps/kshowmail/sounds/neuepost.ogg
%{_datadir}/kde4/apps/kshowmail/sounds/newmail.ogg
%{_datadir}/locale/de/LC_MESSAGES/kshowmail.mo
%{_datadir}/locale/it/LC_MESSAGES/kshowmail.mo
%{_datadir}/locale/es/LC_MESSAGES/kshowmail.mo
%{_datadir}/locale/nl/LC_MESSAGES/kshowmail.mo
%{_libdir}/kde4/kcm_kshowmailconfigaccounts.so
%{_libdir}/kde4/kcm_kshowmailconfigactions.so
%{_libdir}/kde4/kcm_kshowmailconfigdisplay.so
%{_libdir}/kde4/kcm_kshowmailconfigfilter.so
%{_libdir}/kde4/kcm_kshowmailconfiglog.so
%{_libdir}/kde4/kcm_kshowmailconfiggeneral.so
%{_libdir}/kde4/kcm_kshowmailconfigspamcheck.so

%changelog
