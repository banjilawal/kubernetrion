//
// Created by griot on 2/12/25.
//
#include "process_names.h"

const char* process_names[PROCESS_NAME_COUNT] = {
    // ------ Linux Processes (150 Names) ------
    "init", "systemd", "bash", "sshd", "cron", "nginx", "apache2", "mysqld", "postgresql", "docker",
    "kworker", "Xorg", "gnome-shell", "firefox", "chromium", "snapd", "irqbalance", "cupsd",
    "pulseaudio", "bluetoothd", "NetworkManager", "avahi-daemon", "modprobe", "dbus-daemon",
    "udisksd", "wpa_supplicant", "pms", "evolution", "top", "htop", "vmstat",
    "journalctl", "smbd", "nmbd", "lightdm", "gdm", "rfkill", "mount.ntfs", "ksoftirqd",
    "dhclient", "udisks2", "nm-applet", "packagekitd", "uaccess", "fstrim", "desktop-file-service",
    "gparted", "xfce4-session", "mate-panel", "kscreenlocker", "xfwm4", "lokalise", "thunar",
    "nemo", "konsole", "yakuake", "gnome-terminal", "xfce4-terminal", "vim", "nano",
    "zsh", "fish", "lxsession", "dpkg", "pacman", "yum", "rpm", "flatpak", "snap-store",
    "sshd-gen-key", "logrotate", "auditd", "irqbalance", "watch", "libvirtd", "gnome-system-monitor",
    "xrdp", "vsftpd", "dnf", "podman", "minikube", "kubelet", "etcd", "prometheus",
    "grafana", "kafka", "elasticsearch", "fluentd", "redis-server", "memcached", "bind9",
    "openvpn", "iptables", "certbot", "rsync", "scp", "sftp-server", "lxc-start",
    "zabbix-agent", "tshark", "tcpdump", "wireshark", "strace", "perf", "bash-completion",
    "docker-proxy", "dockerd", "clamav", "python3", "perl", "ruby", "java",
    "node", "php", "git", "terraform", "ansible", "jenkins", "kibana",
    "systemctl", "cups-browsed", "postfix", "exim4", "dovecot", "vim", "emacs",
    "gnupg", "rsyslogd", "cloud-init", "plymouthd", "udisksd", "policykitd", "cgproxy",

    // ------ Windows Processes (150 Names) ------
    "explorer.exe", "cmd.exe", "taskmgr.exe", "winlogon.exe", "svchost.exe",
    "services.exe", "conhost.exe", "lsass.exe", "csrss.exe", "dwm.exe",
    "RuntimeBroker.exe", "spoolsv.exe", "wininit.exe", "dllhost.exe", "SearchIndexer.exe",
    "smss.exe", "PerfMon.exe", "outlook.exe", "EXCEL.EXE", "WINWORD.EXE",
    "mspaint.exe", "notepad.exe", "SnippingTool.exe", "calc.exe", "regedit.exe",
    "Skype.exe", "Zoom.exe", "Teams.exe", "chrome.exe", "edge.exe",
    "firefox.exe", "wordpad.exe", "onenote.exe", "powerpoint.exe", "acrobat.exe",
    "SecurityHealthService.exe", "Defender.exe", "OneDrive.exe", "Explorer.exe",
    "SearchProtocolHost.exe", "SearchFilterHost.exe", "SystemSettings.exe", "winver.exe",
    "WUDFHost.exe", "csrss.exe", "ctfmon.exe", "FileExplorer.exe", "SkypeHost.exe",
    "SettingsHost.exe", "WmiPrvSE.exe", "SnippingTool.exe", "Solitaire.exe", "Photos.exe",
    "NetworkDiscovery.exe", "RuntimeBroker.exe", "cmd.exe", "RdpClient.exe", "Firewall.exe",
    "recoverydrive.exe", "dxdiag.exe", "optionalfeatures.exe", "diskpart.exe", "Magnify.exe",
    "CleanMgr.exe", "SysMain.exe", "dlhost.exe", "SystemProperties.exe", "virtmgmt.exe",
    "rdpclip.exe", "mstsc.exe", "hypervisor.exe", "SetUp.exe", "DeviceLog.exe", "msiexec.exe",

    // ------ macOS Processes (150 Names) -------
    "launchd", "SystemUIServer", "WindowServer", "mds", "Dock", "Finder", "hidd", "Dropbox",
    "Safari", "TextEdit", "Preview", "AddressBook", "Keynote", "Chrome", "Slack",
    "meetingd", "bluetoothprox", "accounts-daemon", "calendar", "ControlStrip",
    "cfprefsd", "backupd", "nsurlsessiond", "softwareupdated", "localsnapshot",
    "loginwindow", "syslog", "defaults", "distnoted", "MenuBarExtras", "coreservicesd",
    "kdc", "ImageCaptureExtension", "launchctl", "kextd", "airportd", "powerd",
    "usbd", "lpd", "diskimages-helper", "Therm", "PhotosAgent", "timed", "Spotlight",
    "NTFSVolume", "EventScripts", "ProxyConfigChanged", "com.apple.IconServices",
    "CameraLauncher", "AppStore", "ActivityMonitor", "iWork", "XCode", "Postfix",
    "mdworker", "wish", "sandbox-exemptions", "filecoord",

    // ------ VAX Process Names (150 Names) ------
    "LOGINOUT", "DECW$SERVER", "AUDIT_SERVER", "LATACP", "QUEUE_MANAGER",
    "LOCK_SERVER", "SCHED_CONTROL", "PROCESS_MONITOR", "PASSWORD_SERVER", "VMS_HELPER",
    "OPCOM", "SWAPPER", "SYS$NETACP", "PHY_LOCK_SERVER", "BUGCHECK_DUMP",
    "DIRECTORY_SERVER", "HISTORY_SERVER", "TIME_SERVER", "NFS_SERVER", "SMISERVER",
    "JOB_CONTROL", "LAT_MASTER", "AUDIT_JOURNAL", "SMB_SERVER", "BACKGROUND_JOB",
    "TELNET_SERVER", "CACHE_SERVER", "PRINT_SERVER", "RES_MONITOR", "DEVICE_MONITOR",
    "MULTINET", "MAIL_SERVER", "SECURITY_SERVER", "DECW$DTLOGIN", "CMS_SERVER",
    "TCPIP_SERVER", "SYSTEM_ACCESS_CONTROL", "MEMORY_MANAGER", "ALERT_SERVICE",
    "BACKUP_SERVICE", "VIRTUAL_MEMORY_HELPER", "IP_SERVICE", "TOKEN_MANAGER",
    "CLUSTER_SERVER", "RESOURCE_CONTROL", "ALLOCATION_MANAGER", "HARDWARE_SCAN",
    "LOGICAL_NAME_MANAGER", "SECURITY_AUDIT", "SYS$TAILOR", "ACCOUNTING_LOG",
    "REMOTE_SERVICE", "DECNET_LINK_CONTROL", "MESSAGE_ROUTER", "JOB_MANAGER",
    "GENESIS_SERVER", "JOURNAL_MANAGER", "SYS$SEND", "ERROR_LOGGER",
    "TAPE_SERVER", "SHARED_MEMORY_ACCESS", "TIMEOUT_HANDLER", "LAT_TARGET",
    "BIND_SERVER", "BUGCHECK_HANDLER", "CPU_RESOURCE_MANAGER", "VIRTUAL_DISPATCHER",
    "PROCESS_DISPATCH", "SHADOW_SERVER", "SNMP_SERVER", "KERNEL_POLICIES",
    "LOGICAL_NAME_RESOLVER", "SYSTEM_INVOCATION", "SCRIPT_MANAGER", "SERVICE_ALLOCATOR",
    "TASK_MANAGER", "RESOURCE_ALLOCATION", "NET$MONITOR", "SYS$ACME", "API_DISPATCHER",
    "SYS$TRACE", "RESOURCE_ACQUISITION", "VIRTUAL_RESOURCE", "LOCK_DISPATCHER",
    "DISK_DISPATCHER", "CPU_MANAGEMENT", "ALARM_SERVICE", "SYSTEM_JOB_MANAGER",
    "FILE_WATCHER", "EVENT_LOGGER", "TERMINAL_MONITOR", "NETWORK_FLOW",
    "DECWINDOWS_MANAGEMENT", "SYS$SPECIAL_SERVICES", "VMS_USER_MONITOR",
    "MULTICLUS_MANAGER", "DEC_VMS_NET_ROUTE", "PRINT_QUEUE_SERVER",
    "PROCESS_MONITOR_HELPER", "VIRTUAL_CPU_DISPATCHER", "SYS$DELAY",
    "BACKUP_HELPER", "FAILURE_ANALYZER", "RESOURCE_LOOKUP", "TIMER_MANAGER",
    "NODE_SUPERVISOR", "DATABASE_SERVICE", "DATA_CORRUPTION_MONITOR",
    "LAT_SERVER_HELPER", "MULTINET_HELPER", "CLUSTER_ACCESS", "RESOURCE_DISPATCH",
    "TRAP_SERVICE", "VMS_AGENT_PRESSURE", "FILE_BLOCK_HELPER",
    "LOGFILE_MONITOR", "SYSTEM_SHUTDOWN", "DECWINDOW_CONTROLLER",
    "SYSTEM_ERROR_DISPATCHER", "PROCESS_NODE_MONITOR", "VIRTUAL_DISPATCH_SYSTEM",
    "UPGRADE_MANAGER", "LOG_BACKUP", "TAPE_EXTRACT_MANAGER", "AUDIT_MANAGER"
};

